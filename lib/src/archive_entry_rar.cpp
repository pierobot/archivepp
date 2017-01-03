#include <archivepp/archive_entry_rar.hpp>

#include <cmath>
#include <iterator>
#include <memory>

namespace archivepp
{
    struct cb_context
    {
        std::string buffer;

        cb_context() = default;
    };

    /**
     *  Callback that the unrar library calls whenever an event occurs during extraction.
     *
     *  @param msg The event.
     *  @param context A pointer to custom data.
     *  @param data A chunk of unpacked data.
     *  @param size The size of the unpacked chunk.
     */
    static int CALLBACK rar_callback(UINT msg, LPARAM context, LPARAM data, LPARAM size)
    {
        switch (msg)
        {
        case UCM_PROCESSDATA:
        {
            // A chunk of data has been unpacked and is ready for further processing
            cb_context * context = reinterpret_cast<cb_context*>(context);
            // Ensure it's a valid pointer
            if (context == 0)
                return -1;

            char const * const unpacked_data = reinterpret_cast<char *>(data);
            // Reserve (size) + (size / 2).
            // This takes into account that there might be more than one chunk
            context->buffer.reserve(context->buffer.size() + (std::ceil(static_cast<float>(size) * 1.5f)));
            // Copy the chunk of unpacked data to our buffer
            // Use std::back_inserter to ensure that the unpacked data is always appended to our buffer
            std::copy(&unpacked_data[0], &unpacked_data[size], std::back_inserter(context->buffer));

            return 1;
        }

        case UCM_NEEDPASSWORD:
            // A password is required for further processing
            // Unimplemented for now
        default:
            return -1;
        }
    }

    archive_entry_rar::archive_entry_rar(native_handle_type handle,
                                         uint64_t index,
                                         archivepp::string name,
                                         uint64_t size,
                                         std::error_code & ec) :
        basic_archive_entry(handle, index),
        m_name(std::move(name)),
        m_size(size)
    {
    }

    archive_entry_rar::~archive_entry_rar()
    {
    }

    std::string archive_entry_rar::get_contents() const
    {
        std::unique_ptr<cb_context> context_ptr(new cb_context());
        HANDLE handle = get_handle();

        ::RARSetCallback(handle, rar_callback, reinterpret_cast<LPARAM>(context_ptr.get()));

        RARHeaderDataEx header{};
        for (uint64_t i = 0; ::RARReadHeaderEx(handle, &header) == ERAR_SUCCESS; ++i)
        {
            // Is the current index == to this entry's?
            if (i != get_index())
            {
                // No, skip this entry
#ifdef ARCHIVEPP_USE_WSTRING
                ::RARProcessFileW(handle, RAR_SKIP, nullptr, nullptr);
#else
                ::RARProcessFile(handle, RAR_SKIP, nullptr, nullptr);
#endif
                continue;
            }

#ifdef ARCHIVEPP_USE_WSTRING
            ::RARProcessFileW(handle, RAR_TEST, nullptr, nullptr);
#else
            ::RARProcessFile(handle, RAR_TEST, nullptr, nullptr);
#endif
            break;
        }

        return std::string(std::move(context_ptr->buffer));
    }

    archivepp::string archive_entry_rar::get_name() const
    {
        return m_name;
    }

    uint64_t archive_entry_rar::get_size() const
    {
        return m_size;
    }
}
