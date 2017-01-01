#pragma once

#ifdef ARCHIVEPP_USE_WSTRING
#   include <utf8.h>
#endif

#include <iterator>
#include <string>

namespace archivepp
{
#ifdef ARCHIVEPP_USE_WSTRING
    typedef std::wstring string;
#else
    typedef std::string string;
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
