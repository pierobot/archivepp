#pragma once

#include <cstdint>

#include <archivepp/string.hpp>

namespace archivepp
{
    class archive
    {
    public:
        virtual ~archive() {}

        virtual archivepp::string const & get_path() const = 0;
        
        virtual int64_t get_number_of_entries() const = 0;
    protected:
    private:
    };
}