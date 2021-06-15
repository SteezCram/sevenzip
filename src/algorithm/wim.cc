#pragma once

#include <napi.h>
#include <string>
#include <vector>


#include "bitcompressionlevel.hpp"
#include "bitcompressor.hpp"
#include "bitexception.hpp"
#include "bitextractor.hpp"




/// <summary>
/// Wim compressor async worker
/// </summary>
class WimCompressorWorker : public Napi::AsyncWorker 
{
public:
    WimCompressorWorker(Napi::Function& callback, std::wstring dll, std::wstring dir, std::vector<std::wstring> files, std::wstring destination)
    : AsyncWorker(callback), dll(dll), dir(dir), files(files), destination(destination) {}
    ~WimCompressorWorker() {}

    /// <summary>
    /// Code to execute in async
    /// </summary>
    void Execute() override 
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
            bit7z::BitCompressor compressor { lib, bit7z::BitFormat::Wim };


            // Two choice to compress, whole directory or selected files
            if (this->dir != L"")
                compressor.compressDirectory(this->dir, this->destination);
            else if (this->files.size() != 0)
                compressor.compressFiles(this->files, this->destination);
        }
        catch (const bit7z::BitException& ex) {
            this->exception = ex.what();
        }


        //this->exception = wstring_to_string(this->dll);
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
        Callback().Call({Napi::String::New(Env(), this->exception)});
    }

private:
    std::wstring dll;
    std::wstring dir;
    std::vector<std::wstring> files;
    std::wstring destination;
    std::string exception;
};


/// <summary>
/// Wim extractor async worker
/// </summary>
class WimExtractWorker : public Napi::AsyncWorker 
{
public:
    WimExtractWorker(Napi::Function& callback, std::wstring dll, std::wstring archive, std::wstring destination) 
    : AsyncWorker(callback), dll(dll), archive(archive), destination(destination) {}
    ~WimExtractWorker() {}

    /// <summary>
    /// Code to execute in async
    /// </summary>
    void Execute() override 
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
            bit7z::BitExtractor extractor { lib, bit7z::BitFormat::Wim };


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
        Callback().Call({Napi::String::New(Env(), this->exception)});
    }

private:
    std::wstring dll;
    std::wstring archive;
    std::wstring destination;
    std::string exception;
};