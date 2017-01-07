#pragma once

#include <archivepp/archive_entry.hpp>
#include <archivepp/except.hpp>

namespace archivepp
{
    class basic_archive_entry : public archive_entry
    {
    public:
        basic_archive_entry(archivepp::string name, uint64_t index, uint64_t uncompressed_size, uint64_t compressed_size) :
            m_name(std::move(name)),
            m_index(index),
            m_uncompressed_size(uncompressed_size),
            m_compressed_size(compressed_size)
        {
        }

        virtual ~basic_archive_entry()
        {
        }

        virtual archivepp::string const & get_name() const override final
        {
            return m_name;
        }

        virtual uint64_t get_index() const override final
        {
            return m_index;
        }

        virtual uint64_t get_uncompressed_size() const override final
        {
            return m_uncompressed_size;
        }

        virtual uint64_t get_compressed_size() const override final
        {
            return m_compressed_size;
        }
    protected:
    private:
        archivepp::string m_name;
        uint64_t m_index;
        uint64_t m_uncompressed_size;
        uint64_t m_compressed_size;
    };
}
