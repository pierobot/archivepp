#include <archivepp/basic_archive_entry.hpp>

#include <cstring>
#include <system_error>

#include <zip.h>

namespace archivepp
{
    class archive_entry_zip final : public basic_archive_entry
    {
    public:
        archive_entry_zip(zip_t * handle, uint64_t index, std::error_code & ec);

        virtual ~archive_entry_zip();

        virtual uint64_t get_size() const override;

        virtual std::string get_contents() const override;
    protected:
    private:
        zip_stat_t m_zip_stat;

        inline zip_t * get_zip_handle() const;
        inline std::error_code get_last_error();
    };
}