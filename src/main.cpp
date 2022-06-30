#pragma once

#include <napi.h>
#include <filesystem>
#include <string>


#include "convert.hpp"
#include "worker.hpp"




/// <summary>
/// Implementation of the Compress method
/// </summary>
/// 
/// <param name="info">Parameters passed</param>
/// 
/// <returns>Undefined value</returns>
inline Napi::Value Compress(const Napi::CallbackInfo& info)
{
    std::string algorithm = info[0].ToString();

    Napi::Object config = info[1].As<Napi::Object>();
    std::wstring dll = u16string_to_wstring(config.Get("dll").ToString().Utf16Value());
    std::wstring dir = (config.Has("dir")) ? u16string_to_wstring(config.Get("dir").ToString().Utf16Value()) : L"";
    std::vector<std::wstring> files;
    if (config.Has("files"))
        files = napi_array_to_vector_wstring(config.Get("files").As<Napi::Array>());
    std::wstring destination = (config.Has("destination")) ? u16string_to_wstring(config.Get("destination").ToString().Utf16Value()) : L"";
    int level = (config.Has("level")) ? config.Get("level").ToNumber().Int32Value() : -1;
    std::wstring password = (config.Has("password")) ? u16string_to_wstring(config.Get("password").ToString().Utf16Value()) : L"";
    int is64 = (config.Has("is64")) ? config.Get("is64").ToBoolean() : false;
    

    if (std::filesystem::exists(destination))
        std::filesystem::remove(destination);

    if (level != -1 && level != 0 && level != 1 && level != 3 && level != 5 && level != 7 && level != 9)
        level = -1;

    CompressWorker* wk = new CompressWorker(info[2].As<Napi::Function>(), info[3].As<Napi::Function>(), algorithm, dll, dir, files, destination, level, password, is64);
    wk->Queue();


    return info.Env().Undefined();
}

/// <summary>
/// Implementation of the SevenZipExtract method
/// </summary>
/// 
/// <param name="info">Parameters passed</param>
/// 
/// <returns>Undefined value</returns>
inline Napi::Value Extract(const Napi::CallbackInfo& info)
{
    std::string algorithm = info[0].ToString();

    Napi::Object config = info[1].As<Napi::Object>();
    std::wstring dll = u16string_to_wstring(config.Get("dll").ToString().Utf16Value());
    std::wstring archive = (config.Has("archive")) ? u16string_to_wstring(config.Get("archive").ToString().Utf16Value()) : L"";
    std::wstring destination = (config.Has("destination")) ? u16string_to_wstring(config.Get("destination").ToString().Utf16Value()) : L"";
    std::wstring password = (config.Has("password")) ? u16string_to_wstring(config.Get("password").ToString().Utf16Value()) : L"";


    ExtractWorker* wk = new ExtractWorker(info[2].As<Napi::Function>(), info[3].As<Napi::Function>(), algorithm, dll, archive, destination, password);
    wk->Queue();


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