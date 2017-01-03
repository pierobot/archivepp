#include <archivepp/archive_rar.hpp>

namespace archivepp
{
    archive_rar::archive_rar(archivepp::string path, std::error_code & ec) :
        basic_archive(std::move(path)),
        m_rar(nullptr),
        m_arc_data()
    {
        m_arc_data.OpenMode = RAR_OM_EXTRACT;
#ifdef ARCHIVEPP_USE_WSTRING
        m_arc_data.ArcNameW = const_cast<char*>(&get_path()[0]);
#else
        m_arc_data.ArcName = const_cast<char*>(&get_path()[0]);
#endif
        m_rar = ::RAROpenArchiveEx(&m_arc_data);
        if (m_rar != nullptr)
        {
            ec = std::error_code();
        }
        else
        {
            ec = std::error_code(m_arc_data.OpenResult, std::system_category());
        }
    }

    archive_rar::~archive_rar()
    {
        if (m_rar != nullptr)
        {
            ::RARCloseArchive(m_rar);
        }
    }

    int64_t archive_rar::get_number_of_entries() const
    {
        if (m_rar == nullptr)
            throw std::runtime_error("Cannot access m_rar because it is null.");

        RARHeaderDataEx header {};
        int64_t count = 0;
        while (::RARReadHeaderEx(m_rar, &header) == ERAR_SUCCESS)
        {
            ::RARProcessFile(m_rar, RAR_SKIP, NULL, NULL);
            ++count;
        }

        return count;
    }

    auto archive_rar::get_entries(filter_function filter_fn) const -> std::vector<entry_pointer>
    {
        std::vector<entry_pointer> entries;

        if (m_rar == nullptr)
            throw std::runtime_error("Cannot access m_rar because it is null.");

        RARHeaderDataEx header {};
        int64_t index = 0;
        while (::RARReadHeaderEx(m_rar, &header) == ERAR_SUCCESS)
        {
            ::RARProcessFile(m_rar, RAR_SKIP, NULL, NULL);
            ++index;

            std::error_code ec;
#ifdef ARCHIVEPP_USE_WSTRING
            entry_pointer entry_ptr(new archive_entry_rar(m_rar, index, &header.FileNameW[0], header.UnpSize, ec));
#else
            entry_pointer entry_ptr(new archive_entry_rar(m_rar, index, &header.FileName[0], header.UnpSize, ec));
#endif
            if (!ec && entry_ptr != nullptr)
                entries.emplace_back(std::move(entry_ptr));
        }

        return entries;
    }
}
