#pragma once

#include <archivepp/archive_entry.hpp>
#include <archivepp/except.hpp>

namespace archivepp
{
    class basic_archive_entry : public archive_entry
    {
    public:
        typedef void * native_handle_type;

        basic_archive_entry(native_handle_type handle, uint64_t index) :
            m_handle(handle),
            m_index(index)
        {
        }

        virtual ~basic_archive_entry()
        {
        }

        virtual uint64_t get_index() const
        {
            return m_index;
        }

        native_handle_type get_handle() const
        {
            return m_handle;
        }
    protected:
    private:
        native_handle_type m_handle;
        uint64_t m_index;
    };
}
