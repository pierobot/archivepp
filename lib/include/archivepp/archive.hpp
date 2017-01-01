#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include <archivepp/archive_entry.hpp>
#include <archivepp/string.hpp>

namespace archivepp
{
    class archive
    {
    public:
        typedef std::unique_ptr<archive_entry> entry_pointer;
        typedef std::function<bool(entry_pointer const & entry_ptr)> filter_function;

        virtual ~archive() {}

        virtual archivepp::string const & get_path() const = 0;
        
        virtual int64_t get_number_of_entries() const = 0;

        virtual std::vector<entry_pointer> get_entries(filter_function filter_fn) const = 0;
    protected:
    private:
    };
}