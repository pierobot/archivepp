#pragma once

#include <archivepp/secure_allocator.hpp>

#ifdef ARCHIVEPP_USE_WSTRING
#   include <utf8.h>
#endif

#include <iterator>
#include <string>

#ifdef ARCHIVEPP_USE_WSTRING
#   define ARCHIVEPP_STR(s) L##s
#else
#   define ARCHIVEPP_STR(s) s
#endif

namespace archivepp
{
    using secure_string = std::basic_string<char, std::char_traits<char>, secure_allocator<char>>;
#ifdef ARCHIVEPP_USE_WSTRING
    using string = std::wstring;
    using secure_string = std::basic_string<wchar_t, std::char_traits<wchar_t>, secure_allocator<wchar_t>>;
#else
    using string = std::string;
#endif

#ifdef ARCHIVEPP_USE_WSTRING
    inline std::string to_utf8(std::wstring const & str)
    {
        std::string result;

        utf8::utf16to8(str.begin(), str.end(), std::back_inserter(result));

        return result;
    }

    inline std::wstring to_utf16(std::string const & str)
    {
        std::wstring result;

        utf8::utf8to16(str.begin(), str.end(), std::back_inserter(result));

        return result;
    }
#endif
}
