#pragma once

#include <archivepp/string.hpp>

#include <cstdint>
#include <system_error>

namespace archivepp
{
    class archive_entry
    {
    public:
        virtual ~archive_entry() {}

        virtual std::string get_contents(std::error_code & ec) const = 0;

        virtual uint64_t get_index() const = 0;

        virtual archivepp::string get_name() const = 0;
        
        virtual uint64_t get_size() const = 0;
    protected:
    private:
    };
}
