#pragma once

#include <napi.h>
#include <string>
#include <vector>


#include "ProgressCallback.hpp"
#include "bitcompressionlevel.hpp"
#include "bitcompressor.hpp"
#include "bitexception.hpp"
#include "bitextractor.hpp"




/// <summary>
/// Parse the algorithm to a bitInOutFormat
/// </summary>
/// 
/// <param name="algorithm">Algorithm to parse</param>
/// 
/// <returns>Correct BitInOutFormat</returns>
inline bit7z::BitInOutFormat algorithm_to_bitInOutFormat(std::string algorithm) 
{
    if (algorithm == "tar")
        return bit7z::BitFormat::Tar;
    if (algorithm == "wim")
        return bit7z::BitFormat::Wim;
    if (algorithm == "zip")
        return bit7z::BitFormat::Zip;

    return bit7z::BitFormat::SevenZip;
}

/// <summary>
/// Parse the algorithm to a compression method
/// </summary>
/// 
/// <param name="algorithm">Algorithm to parse</param>
/// <param name="is64">If we use the 64 bits version</param>
/// 
/// <returns>Correct BitCompressionMethod</returns>
inline bit7z::BitCompressionMethod algorithm_to_bitCompressionMethod(std::string algorithm, bool is64)
{
    if (algorithm == "zip")
    {
        if (is64)
            return bit7z::BitCompressionMethod::Deflate64;
        else
            return bit7z::BitCompressionMethod::Deflate;
    }

    return bit7z::BitCompressionMethod::Lzma2;
}




/// <summary>
/// Compress async worker
/// </summary>
class CompressWorker : public Napi::AsyncProgressQueueWorker<ProgressCallback>
{
public:
    CompressWorker(Napi::Function& callback, Napi::Function& progressCallback, std::string& algorithm, std::wstring& dll, std::wstring& dir, std::vector<std::wstring>& files, std::wstring& destination, int level, std::wstring& password, bool is64)
    : AsyncProgressQueueWorker(callback), algorithm(algorithm), dll(dll), dir(dir), files(files), destination(destination), password(password), level(level), is64(is64),
      total(0), progress(0), ratio(0), fileProcessed(L"")
    {
        this->progressCallback.Reset(progressCallback, 1);
    }
    ~CompressWorker() {}

    /// <summary>
    /// Code to execute in async
    /// </summary>
    void Execute(const ExecutionProgress& progress) override
    {
        // Error handling
        if (this->dir == L"" && this->files.size() == 0) {
            this->exception = "No files or dir to compress";
            return;
        }
        if (this->destination == L"") {
            this->exception = "No destination path selected";
            return;
        }


        try
        {
            bit7z::Bit7zLibrary lib { this->dll };
            bit7z::BitCompressor compressor { lib, algorithm_to_bitInOutFormat(this->algorithm) };

            bit7z::TotalCallback totalCallback = [this](uint64_t total) -> void { this->total = total; };
            bit7z::ProgressCallback progressCallback = [this, &progress](uint64_t completed) -> void {
                this->progress = static_cast<int>((100.0 * completed) / this->total);

                ProgressCallback* obj = new ProgressCallback {
                    this->total,
                    this->progress,
                    this->ratio,
                    wstring_to_string(this->fileProcessed)
                };

                progress.Send(obj, 1);
            };
            bit7z::RatioCallback ratioCallback = [this](uint64_t in_size, uint64_t out_size) -> void { this->ratio = static_cast<int>((100.0 * out_size) / in_size); };
            bit7z::FileCallback fileCallback = [this](std::wstring fileProcessed) -> void { this->fileProcessed = fileProcessed; };
            compressor.setTotalCallback(totalCallback);
            compressor.setProgressCallback(progressCallback);
            compressor.setRatioCallback(ratioCallback);
            compressor.setFileCallback(fileCallback);


            if (this->algorithm == "7z" || this->algorithm == "zip")
            {
                // Set compression parameters, optionnal
                compressor.setCompressionMethod(algorithm_to_bitCompressionMethod(this->algorithm, this->is64));
                compressor.setCompressionLevel(this->level != -1 ? (static_cast<bit7z::BitCompressionLevel>(this->level)) : bit7z::BitCompressionLevel::NORMAL);
            }

            if (this->algorithm == "7z" && this->password != L"")
                compressor.setPassword(this->password);


            // Two choice to compress, whole directory or selected files
            if (this->dir != L"")
                compressor.compressDirectory(this->dir, this->destination);
            else if (this->files.size() != 0)
                compressor.compressFiles(this->files, this->destination);
        }
        catch (const bit7z::BitException& ex) {
            this->exception = ex.what();
        }
    }

    /// <summary>
    /// Triggered when a runtime error has been triggered
    /// </summary>
    /// 
    /// <param name="e">Runtime error triggered</param>
    void OnError(const Napi::Error& e) override 
    {
        Napi::HandleScope scope(Env());
        Callback().Call({Napi::String::New(Env(), e.Message())});
    }

    /// <summary>
    /// Triggered when the execution exit normally
    /// </summary>
    void OnOK() override 
    {
        Napi::HandleScope scope(Env());
        // Return the bit7z error since it doesn't trigger the OnError event
        Callback().Call({this->exception == "" ? Env().Undefined() : Napi::String::New(Env(), this->exception)});
    }

    /// <summary>
    /// Triggered when the operation has been progressed
    /// </summary>
    void OnProgress(const ProgressCallback* data, size_t /* count */) 
    {
        Napi::HandleScope scope(Env());

        Napi::Object obj = Napi::Object::New(Env());
        obj.Set("total", data->total);
        obj.Set("progress", data->progress);
        obj.Set("ratio", data->ratio);
        obj.Set("file", data->fileProcessed);
        
        if (!this->progressCallback.IsEmpty()) {
            this->progressCallback.Call(Receiver().Value(), {obj});
        }
    }

private:
    std::string algorithm;  
    std::wstring dll;
    std::wstring dir;
    std::vector<std::wstring> files;
    std::wstring destination;
    std::wstring password;
    int level;
    bool is64;
    std::string exception;
    Napi::FunctionReference progressCallback;
    uint64_t total;
    int progress;
    int ratio;
    std::wstring fileProcessed;
};


/// <summary>
/// Extract async worker
/// </summary>
class ExtractWorker : public Napi::AsyncProgressQueueWorker<ProgressCallback>
{
public:
    ExtractWorker(Napi::Function& callback, Napi::Function& progressCallback, std::string& algorithm, std::wstring& dll, std::wstring& archive, std::wstring& destination, std::wstring& password) 
    : AsyncProgressQueueWorker(callback), algorithm(algorithm), dll(dll), archive(archive), destination(destination), password(password),
      total(0), progress(0), ratio(0), fileProcessed(L"") 
    {
        this->progressCallback.Reset(progressCallback, 1);
    }
    ~ExtractWorker() {}

    /// <summary>
    /// Code to execute in async
    /// </summary>
    void Execute(const ExecutionProgress& progress) override
    {
        // Error handling
        if (this->archive == L"") {
            this->exception = "No archive to extract";
            return;
        }
        if (this->destination == L"") {
            this->exception = "No destination path selected";
            return;
        }


        try
        {
            bit7z::Bit7zLibrary lib { this->dll };
            bit7z::BitExtractor extractor { lib, algorithm_to_bitInOutFormat(this->algorithm) };

            bit7z::TotalCallback totalCallback = [this](uint64_t total) -> void { this->total = total; };
            bit7z::ProgressCallback progressCallback = [this, &progress](uint64_t completed) -> void {
                this->progress = static_cast<int>((100.0 * completed) / this->total);

                ProgressCallback* obj = new ProgressCallback {
                    this->total,
                    this->progress,
                    this->ratio,
                    wstring_to_string(this->fileProcessed)
                };

                progress.Send(obj, 1);
            };
            bit7z::RatioCallback ratioCallback = [this](uint64_t in_size, uint64_t out_size) -> void { this->ratio = static_cast<int>((100.0 * out_size) / in_size); };
            bit7z::FileCallback fileCallback = [this](std::wstring fileProcessed) -> void { this->fileProcessed = fileProcessed; };
            extractor.setTotalCallback(totalCallback);
            extractor.setProgressCallback(progressCallback);
            extractor.setRatioCallback(ratioCallback);
            extractor.setFileCallback(fileCallback);


            if (this->algorithm == "7z" && this->password != L"")
                extractor.setPassword(this->password);


            extractor.extract(this->archive, this->destination);
        }
        catch (const bit7z::BitException& ex) {
            this->exception = ex.what();
        }
    }

    /// <summary>
    /// Triggered when a runtime error has been triggered
    /// </summary>
    /// 
    /// <param name="e">Runtime error triggered</param>
    void OnError(const Napi::Error& e) override 
    {
        Napi::HandleScope scope(Env());
        Callback().Call({Napi::String::New(Env(), e.Message())});
    }

    /// <summary>
    /// Triggered when the execution exit normally
    /// </summary>
    void OnOK() override 
    {
        Napi::HandleScope scope(Env());
        // Return the bit7z error since it doesn't trigger the OnError event
        Callback().Call({this->exception == "" ? Env().Undefined() : Napi::String::New(Env(), this->exception)});
    }

    /// <summary>
    /// Triggered when the operation has been progressed
    /// </summary>
    void OnProgress(const ProgressCallback* data, size_t /* count */)
    {
        Napi::HandleScope scope(Env());

        Napi::Object obj = Napi::Object::New(Env());
        obj.Set("total", data->total);
        obj.Set("progress", data->progress);
        obj.Set("ratio", data->ratio);
        obj.Set("file", data->fileProcessed);
        
        if (!this->progressCallback.IsEmpty()) {
            this->progressCallback.Call(Receiver().Value(), {obj});
        }
    }

private:
    std::string algorithm;
    std::wstring dll;
    std::wstring archive;
    std::wstring destination;
    std::wstring password;
    std::string exception;
    Napi::FunctionReference progressCallback;
    uint64_t total;
    int progress;
    int ratio;
    std::wstring fileProcessed;
};