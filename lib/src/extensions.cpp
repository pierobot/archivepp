#include <archivepp/extensions.hpp>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>

namespace archivepp
{
    static std::vector<archivepp::string> & operator+=(std::vector<archivepp::string> & left, std::vector<archivepp::string> right)
    {
        left.reserve(left.size() + right.size());

        using iterator_type = std::vector<archivepp::string>::iterator;

        std::copy(std::move_iterator<iterator_type>(right.begin()),
                  std::move_iterator<iterator_type>(right.end()),
                  std::back_inserter(left));

        return left;
    }

    static std::vector<archivepp::string> & operator-=(std::vector<archivepp::string> & left, std::vector<archivepp::string> const & right)
    {
        left.erase(std::remove_if(left.begin(), left.end(),
            [&](archivepp::string const & left_str) -> bool
        {
            return std::any_of(right.begin(), right.end(),
                [&](archivepp::string const & right_str)
            {
                return left_str == right_str;
            });

        }), left.end());

        return left;
    }

    /**
     * Used to guess the type of file whenever an extension is not found.
     * It compares the first 4 bytes of the file against known signatures for archive formats.
     */
    static archivepp::string guess_extension(archivepp::string const & path)
    {
        auto is_zip = [](uint8_t bytes[8]) -> bool
        {
            static uint8_t zip_signature[4] = { 0x50, 0x4b, 0x03, 0x04 }; // PK
            static uint8_t zip_signature_empty[4] = { 0x50, 0x4b, 0x05, 0x06 }; // PK empty archive
            static uint8_t zip_signature_spanned[4] = { 0x50, 0x4b, 0x07, 0x08 }; // PK spanned archive

            return std::memcmp(bytes, zip_signature, 4) == 0||
                   std::memcmp(bytes, zip_signature_empty, 4) == 0 ||
                   std::memcmp(bytes, zip_signature_spanned, 4) == 0;
        };

        auto is_rar = [](uint8_t bytes[8]) -> bool
        {
            static uint8_t rar_signature_1_5[7] = { 0x52, 0x61, 0x72, 0x21, 0x1a, 0x07, 0x00 }; // Rar! 1.5+
            static uint8_t rar_signature_5_0[8] = { 0x52, 0x61, 0x72, 0x21, 0x1a, 0x07, 0x01, 0x00 }; // Rar! 5.0+

            return std::memcmp(bytes, rar_signature_1_5, 7) == 0||
                   std::memcmp(bytes, rar_signature_5_0, 8) == 0;
        };

        auto is_7z = [](uint8_t bytes[8]) -> bool
        {
            static uint8_t sz_signature[6] = { 0x37, 0x7a, 0xbc, 0xaf, 0x27, 0x1c }; // 7z

            return std::memcmp(bytes, sz_signature, 8) == 0;
        };

        std::ifstream file(path, std::ios_base::binary);
        if (file)
        {
            uint8_t bytes[8] {};

            file.read(reinterpret_cast<char*>(&bytes[0]), sizeof(bytes));

            if (is_zip(bytes) == true)
                return ARCHIVEPP_STR("zip");
            else if (is_rar(bytes) == true)
                return ARCHIVEPP_STR("rar");
            else if (is_7z(bytes) == true)
                return ARCHIVEPP_STR("7z");
        }

        return archivepp::string();
    }

    archivepp::string get_extension(archivepp::string const & path)
    {
        if (path.empty() == true)
            return archivepp::string();

        // Get the filename
        size_t start_pos = path.rfind('/');
        size_t end_pos = path.length();

        if (start_pos == archivepp::string::npos)
            start_pos = path.rfind('\\');

        if (start_pos != archivepp::string::npos)
            ++start_pos;
        else
            start_pos = 0;

        archivepp::string name(path.substr(start_pos, end_pos - start_pos));
        
        // Get the extension from the name
        start_pos = name.rfind('.');
        end_pos = start_pos != archivepp::string::npos ? name.length() : archivepp::string::npos;

        // Is there an extension in the path
        if (start_pos != archivepp::string::npos &&
            end_pos != archivepp::string::npos)
        {
            // Yes, get it
            ++start_pos; // skip the .
            return name.substr(start_pos, end_pos - start_pos);
        }

        // No extension found, guess it.
        return guess_extension(path);
    }

    namespace zip
    {
        static std::vector<archivepp::string> g_extensions = {
            ARCHIVEPP_STR("zip"),
            ARCHIVEPP_STR("cbz")
        };

        void add_extensions(std::vector<archivepp::string> extensions)
        {
            g_extensions += extensions;
        }

        void remove_extensions(std::vector<archivepp::string> const & extensions)
        {
            g_extensions -= extensions;
        }

        std::vector<archivepp::string> const & get_extensions()
        {
            return g_extensions;
        }

        bool is_whitelisted(archivepp::string const & extension)
        {
            return std::find(g_extensions.cbegin(), g_extensions.cend(), extension) != g_extensions.cend();
        }
    }

    namespace rar
    {
        static std::vector<archivepp::string> g_extensions = {
            ARCHIVEPP_STR("rar"),
            ARCHIVEPP_STR("cbr")
        };

        void add_extensions(std::vector<archivepp::string> extensions)
        {
            g_extensions += extensions;
        }

        void remove_extensions(std::vector<archivepp::string> const & extensions)
        {
            g_extensions -= extensions;
        }

        std::vector<archivepp::string> const & get_extensions()
        {
            return g_extensions;
        }

        bool is_whitelisted(archivepp::string const & extension)
        {
            return std::find(g_extensions.cbegin(), g_extensions.cend(), extension) != g_extensions.cend();
        }
    }
}