#include <archivepp/string.hpp>

namespace archivepp
{
    archivepp::string & normalize_path(archivepp::string & path)
    {
        for (auto & c : path)
        {
            if (c == ARCHIVEPP_STR('\\'))
                c = ARCHIVEPP_STR('/');
        }

        return path;
    }
}
