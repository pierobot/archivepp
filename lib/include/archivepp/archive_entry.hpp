#pragma once

#include <archivepp/string.hpp>

#include <cstdint>

namespace archivepp
{
    class archive_entry
    {
    public:
        virtual ~archive_entry() {}

        virtual std::string get_contents() const = 0;

        virtual uint64_t get_index() const = 0;

        virtual archivepp::string get_name() const = 0;
        
        virtual uint64_t get_size() const = 0;
    protected:
    private:
    };
}
