#pragma once

#include "Filesystem.h" // fs = <experimental/filesystem>;

// UTF-16 clone-on-write strings
class String
{
public:
    typedef char16_t char_type;

    //////////////////////////
    // Creation

    // Creates an empty string
    String();
    // Creates a referenced string
    String(const char_type *str);
    // Creates a referenced string with an already known length
    String(const char_type *str, size_t length);
    // Take ownership of an STL string
    String(std::basic_string<char_type> &&str);
    // Make a copy from an STL string
    String(const std::basic_string<char_type> &str);
    // Move Constructor
    String(String &&moved);
    // Properly destroy it
    ~String();
    // Creates a String instance from a utf-8 formatted string
    static String from_utf8(const char *cstr);
    // Creates a String instance from a utf-8 formatted string
    static String from_utf8(const char *cstr, size_t len);
    // Create a string from a path
    static String from_path(const fs::path &path);

    //////////////////////////
    // Basic string stuff

    // Create a copy of the string
    String clone() const;
    // Get the c-style string value
    const char_type *c_str() const;
    // For COM functions that require wchar_ts
    const wchar_t *wc_str() const;
    // Get the length in characters
    size_t length() const;
    // Get the length in characters
    size_t size() const;
    // Append another string
    void append(const String &rhs);
    // Swap two instances
    void swap(String &other);
    // Use this string as a path
    fs::path to_path() const;
    // When you have something that really needs a good-old-fasioned std::string
    std::string to_string() const;
    // In case you really want your own copy of a utf-16 wstring
    std::wstring to_wstring() const;

    //////////////////////////
    // Helpers

    // Consuming an instance, the new instance is guaranteed to be owned
    static String into_owned(String &&str);
    // Converts the current instance into an owned instance if it is not already
    void convert_to_owned();

private:
    bool owned;
    union Data
    {
        Data() {}
        ~Data() {}
        struct Ref
        {
            const char_type *str;
            size_t len;
        } referenced;
        std::basic_string<char_type> owned_str;
    } data;
};

namespace std
{
    template <>
    struct hash<String>
    {
        size_t operator()(const String &s) const
        {
            // The same implementation used in std::hash<std::wstring>
            return std::_Hash_seq((const unsigned char *)s.c_str(), s.size() * sizeof(String::char_type));
        }
    };
}
