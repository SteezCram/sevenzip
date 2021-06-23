#pragma once

#include <codecvt>
#include <iostream>
#include <locale>
#include <memory>




/// <summary>
/// Convert a wstring to a string
/// </summary>
/// 
/// <param name="wstring">wstring to convert</param>
/// 
/// <returns>string from wstring</returns>
inline std::string wstring_to_string(std::wstring wstring)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstring);
}

/// <summary>
/// Convert a u16string to a wstring
/// </summary>
/// 
/// <param name="u16_str">u16string to convert</param>
/// 
/// <returns>wstring from u16string</returns>
inline std::wstring u16string_to_wstring(std::u16string u16_str)
{
    std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>, wchar_t> converter;
    return converter.from_bytes(reinterpret_cast<const char*> (&u16_str[0]), reinterpret_cast<const char*> (&u16_str[0] + u16_str.size()));
}

/// <summary>
/// Convert a Napi array to a wide string vector
/// </summary>
/// 
/// <param name="napi_array">Napi array to convert</param>
/// 
/// <returns>wide string vector from Napi array</returns>
inline std::vector<std::wstring> napi_array_to_vector_wstring(Napi::Array napi_array)
{
    std::vector<std::wstring> wstr_vec;
    long arrayCount = napi_array.Length();

    for (long i = 0; i < arrayCount; i++) {
        Napi::Value val = napi_array[i];
        wstr_vec.push_back(u16string_to_wstring(val.ToString().Utf16Value()));
    }


    return wstr_vec;
}