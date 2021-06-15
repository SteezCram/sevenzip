#pragma once

#include <napi.h>
#include <filesystem>
#include <string>


#include "convert.h"
#include "algorithm/7z.cc"
#include "algorithm/tar.cc"
#include "algorithm/zip.cc"
#include "algorithm/wim.cc"




/// <summary>
/// Implementation of the SevenZipCompress method
/// </summary>
/// 
/// <param name="info">Parameters passed</param>
/// 
/// <returns>Undefined value</returns>
inline Napi::Value Compress (const Napi::CallbackInfo& info)
{
    std::string algorithm = info[0].ToString();

    Napi::Object config = info[1].As<Napi::Object>();
    std::wstring dll = (config.Has("dll")) ? u16string_to_wstring(config.Get("dll").ToString().Utf16Value()) : L"7z.dll";
    std::wstring dir = (config.Has("dir")) ? u16string_to_wstring(config.Get("dir").ToString().Utf16Value()) : L"";
    std::vector<std::wstring> files;
    if (config.Has("files"))
        files = napi_array_to_vector_wstring(config.Get("files").As<Napi::Array>());
    std::wstring destination = (config.Has("destination")) ? u16string_to_wstring(config.Get("destination").ToString().Utf16Value()) : L"";
    int level = (config.Has("level")) ? config.Get("level").ToNumber().Int32Value() : -1;
    

    if (std::filesystem::exists(destination))
        std::filesystem::remove(destination);

    if (level != -1 && level != 0 && level != 1 && level != 3 && level != 5 && level != 7 && level != 9)
        level = -1;

    if (algorithm == "7z")
    {
        std::wstring password = (config.Has("password")) ? u16string_to_wstring(config.Get("password").ToString().Utf16Value()) : L"";

        SevenZipCompressorWorker* wk = new SevenZipCompressorWorker(info[2].As<Napi::Function>(), dll, dir, files, destination, level, password);
        wk->Queue();
    }
    if (algorithm == "tar")
    {
        TarCompressorWorker* wk = new TarCompressorWorker(info[2].As<Napi::Function>(), dll, dir, files, destination);
        wk->Queue();
    }
    else if (algorithm == "zip")
    {
        bool is64 = (config.Has("is64")) ? config.Get("is64").ToBoolean() : false;

        ZipCompressorWorker* wk = new ZipCompressorWorker(info[2].As<Napi::Function>(), dll, dir, files, destination, level, is64);
        wk->Queue();
    }
    if (algorithm == "wim")
    {
        WimCompressorWorker* wk = new WimCompressorWorker(info[2].As<Napi::Function>(), dll, dir, files, destination);
        wk->Queue();
    }


    return info.Env().Undefined();
}

/// <summary>
/// Implementation of the SevenZipExtract method
/// </summary>
/// 
/// <param name="info">Parameters passed</param>
/// 
/// <returns>Undefined value</returns>
inline Napi::Value Extract (const Napi::CallbackInfo& info)
{
    std::string algorithm = info[0].ToString();

    Napi::Object config = info[1].As<Napi::Object>();
    std::wstring dll = (config.Has("dll")) ? u16string_to_wstring(config.Get("dll").ToString().Utf16Value()) : L"7z.dll";
    std::wstring archive = (config.Has("archive")) ? u16string_to_wstring(config.Get("archive").ToString().Utf16Value()) : L"";
    std::wstring destination = (config.Has("destination")) ? u16string_to_wstring(config.Get("destination").ToString().Utf16Value()) : L"";


    if (algorithm == "7z")
    {
        std::wstring password = (config.Has("password")) ? u16string_to_wstring(config.Get("password").ToString().Utf16Value()) : L"";

        SevenZipExtractWorker* wk = new SevenZipExtractWorker(info[2].As<Napi::Function>(), dll, archive, destination, password);
        wk->Queue();
    }
    else if (algorithm == "tar")
    {
        TarExtractWorker* wk = new TarExtractWorker(info[2].As<Napi::Function>(), dll, archive, destination);
        wk->Queue();
    }
    else if (algorithm == "zip")
    {
        ZipExtractWorker* wk = new ZipExtractWorker(info[2].As<Napi::Function>(), dll, archive, destination);
        wk->Queue();
    }
    else if (algorithm == "wim")
    {
        WimExtractWorker* wk = new WimExtractWorker(info[2].As<Napi::Function>(), dll, archive, destination);
        wk->Queue();
    }


    return info.Env().Undefined();
}

/// <summary>
/// Init the SevenZip Node addon
/// </summary>
/// 
/// <param name="env">Environment to use</param>
/// <param name="exports">Object to exports</param>
/// 
/// <returns></returns>
Napi::Object Init(Napi::Env env, Napi::Object exports) 
{
    exports.Set(Napi::String::New(env, "__compress"), Napi::Function::New(env, Compress));
    exports.Set(Napi::String::New(env, "__extract"), Napi::Function::New(env, Extract));

    return exports;
}

NODE_API_MODULE(addon, Init)