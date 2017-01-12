#pragma once

#include <archivepp/basic_archive_entry.hpp>

#ifdef _MSC_VER
#   include <Windows.h>
#endif

#include <unrar/dll.hpp>

namespace archivepp
{
    class archive_entry_rar final : public basic_archive_entry
    {
    public:
        
        archive_entry_rar(archivepp::string name, uint64_t index, uint64_t uncompressed_size, uint64_t compressed_size, std::error_code & ec);
        
        virtual ~archive_entry_rar();
    protected:
    private:
    };
}
