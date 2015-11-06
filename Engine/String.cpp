#include "Common.h"
#include "String.h"
#include <codecvt>

String::String()
    : String(u"", 0)
{
}

String::String(const char_type *str)
    : String(str, std::char_traits<char_type>::length(str))
{
}

String::String(const char_type *str, size_t length)
    : owned(false)
{
    data.referenced.str = str;
    data.referenced.len = length;
}

String::String(std::basic_string<char_type> &&str)
    : owned(false) // start at false in case basic_string's constructor throws
{
    new (&data.owned_str) std::basic_string<char_type>(std::move(str));
    owned = true;
}

String::String(const std::basic_string<char_type> &str)
    : String(std::basic_string<char_type>{str})
{
}

String::String(String &&moved)
    : String()
{
    swap(moved);
}

String::~String()
{
    if (owned)
    {
        data.owned_str.~basic_string();
    }
}

String String::from_utf8(const char *cstr)
{
    return from_utf8(cstr, std::char_traits<char>::length(cstr));
}

String String::from_utf8(const char *cstr, size_t len)
{
    // Well this is just awful D:
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    auto result = converter.from_bytes(cstr, cstr + len);
    return{ std::move((std::basic_string<char_type> &)result) };
}

String String::from_path(const fs::path &path)
{
    return{ path.generic_u16string() };
}

String String::clone() const
{
    return String::into_owned({ c_str(), size() });
}

const String::char_type *String::c_str() const
{
    if (owned)
    {
        return data.owned_str.c_str();
    }
    else
    {
        return data.referenced.str;
    }
}

const wchar_t *String::wc_str() const
{
    return (const wchar_t *)c_str();
}

size_t String::length() const
{
    return size();
}

size_t String::size() const
{
    if (owned)
    {
        return data.owned_str.size();
    }
    else
    {
        return data.referenced.len;
    }
}

void String::append(const String &rhs)
{
    convert_to_owned();
    data.owned_str.append(rhs.c_str(), rhs.size());
}

void String::swap(String &other)
{
    alignas(alignof(String)) char temp[sizeof(String)];
    memcpy(temp, this, sizeof(String));
    memcpy(this, &other, sizeof(String));
    memcpy(&other, temp, sizeof(String));
}

fs::path String::to_path() const
{
    auto str = c_str();
    auto len = size();
    return fs::path(str, str + len);
}

std::string String::to_string() const
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    auto str = wc_str();
    return converter.to_bytes(str, str + size());
}

std::wstring String::to_wstring() const
{
    return std::wstring(wc_str(), size());
}

String String::into_owned(String &&str)
{
    str.convert_to_owned();
    return std::move(str);
}

void String::convert_to_owned()
{
    if (!owned)
    {
        new (&data.owned_str) std::basic_string<char_type>(data.referenced.str, data.referenced.len);
        owned = true;
    }
}
