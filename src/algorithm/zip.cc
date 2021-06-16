#pragma once

#include <napi.h>
#include <string>
#include <vector>


#include "bitcompressionlevel.hpp"
#include "bitcompressor.hpp"
#include "bitexception.hpp"
#include "bitextractor.hpp"




/// <summary>
/// Zip compressor async worker
/// </summary>
class ZipCompressorWorker : public Napi::AsyncWorker 
{
public:
    ZipCompressorWorker(Napi::Function& callback, std::wstring dll, std::wstring dir, std::vector<std::wstring> files, std::wstring destination, int level, bool is64) 
    : AsyncWorker(callback), dll(dll), dir(dir), files(files), destination(destination), level(level), is64(is64) {}
    ~ZipCompressorWorker() {}

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
            bit7z::BitCompressor compressor { lib, bit7z::BitFormat::Zip };
            // Set compression parameters, optionnal
            compressor.setCompressionMethod((this->is64) ? bit7z::BitCompressionMethod::Deflate64 : bit7z::BitCompressionMethod::Deflate);
            compressor.setCompressionLevel(this->level > -1 ? (static_cast<bit7z::BitCompressionLevel>(this->level)) : bit7z::BitCompressionLevel::NORMAL);


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

private:
    std::wstring dll;
    std::wstring dir;
    std::vector<std::wstring> files;
    std::wstring destination;
    int level;
    bool is64;
    std::string exception;
};


/// <summary>
/// Zip extractor async worker
/// </summary>
class ZipExtractWorker : public Napi::AsyncWorker 
{
public:
    ZipExtractWorker(Napi::Function& callback, std::wstring dll, std::wstring archive, std::wstring destination) 
    : AsyncWorker(callback), dll(dll), archive(archive), destination(destination) {}
    ~ZipExtractWorker() {}

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
            bit7z::BitExtractor extractor { lib, bit7z::BitFormat::Zip };


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

private:
    std::wstring dll;
    std::wstring archive;
    std::wstring destination;
    std::string exception;
};