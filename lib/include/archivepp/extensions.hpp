#pragma once

#include <archivepp/string.hpp>

#include <vector>

namespace archivepp
{
    archivepp::string get_extension(archivepp::string const & path);

    namespace zip
    {
        void add_extensions(std::vector<archivepp::string> extensions);
        void remove_extensions(std::vector<archivepp::string> const & extensions);

        std::vector<archivepp::string> const & get_extensions();

        bool is_whitelisted(archivepp::string const & extension);
    }

    namespace rar
    {
        void add_extensions(std::vector<archivepp::string> extensions);
        void remove_extensions(std::vector<archivepp::string> const & extensions);

        std::vector<archivepp::string> const & get_extensions();

        bool is_whitelisted(archivepp::string const & extension);
    }
}
