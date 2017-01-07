#include <archivepp/archive_entry_rar.hpp>

#include <cmath>
#include <cstring>
#include <iterator>
#include <memory>

namespace archivepp
{
//     static inline std::error_code make_ec(int e)
//     {
//         return std::error_code(e, std::system_category());
//     }

//     struct cb_context
//     {
//         std::string buffer;
//         archivepp::string password;

//         cb_context() = default;
//     };

//     /**
//      *  Callback that the unrar library calls whenever an event occurs during extraction.
//      *
//      *  @param msg The event.
//      *  @param context A pointer to custom data.
//      *  @param data A chunk of unpacked data.
//      *  @param size The size of the unpacked chunk.
//      */
//     static int CALLBACK rar_callback(UINT msg, LPARAM context, LPARAM data, LPARAM size)
//     {
//         cb_context * context_ptr = reinterpret_cast<cb_context*>(context);
//         // Ensure it's a valid pointer
//         if (context_ptr == nullptr)
//             return -1;

//         switch (msg)
//         {
//         case UCM_PROCESSDATA:
//         {
//             // A chunk of data has been unpacked and is ready for further processing
//             char const * const unpacked_data = reinterpret_cast<char *>(data);
//             // Reserve (size) + (size / 2).
//             // This takes into account that there might be more than one chunk
//             context_ptr->buffer.reserve(context_ptr->buffer.size() + (std::ceil(static_cast<float>(size) * 1.5f)));
//             // Copy the chunk of unpacked data to our buffer
//             // Use std::back_inserter to ensure that the unpacked data is always appended to our buffer
//             std::copy(&unpacked_data[0], &unpacked_data[size], std::back_inserter(context_ptr->buffer));

//             return 1;
//         }

// #ifdef ARCHIVEPP_USE_WSTRING
//         case UCM_NEEDPASSWORDW:
//         {
//             // Ensure there's a password for the context
//             if (context_ptr->password.empty() != true)
//             {
//                 // Copy the password to data and assign the size
//                 // No need to check if data and size are valid pointers
//                 // They are allocated on the stack in libunrar's function CmdExtract::ExtrDllGetPassword()
//                 auto length = context_ptr->password.length() > 128 ? 128 : context_ptr->password.length();
//                 std::wcsncpy(reinterpret_cast<wchar_t*>(data), context_ptr->password.c_str(), length);
//                 *reinterpret_cast<size_t*>(size) = length;

//                 return 1;
//             }
//         }
// #else
//         case UCM_NEEDPASSWORD:
//         {
//             // Ensure there's a password for the context
//             if (context_ptr->password.empty() != true)
//             {
//                 // Copy the password to data and assign the size
//                 // No need to check if data and size are valid pointers
//                 // They are allocated on the stack in libunrar's function CmdExtract::ExtrDllGetPassword()
//                 auto length = context_ptr->password.length() > 128 ? 128 : context_ptr->password.length();
//                 std::strncpy(reinterpret_cast<char*>(data), context_ptr->password.c_str(), length);
//                 *reinterpret_cast<size_t*>(size) = length;

//                 return 1;
//             }
//         }
// #endif

//         default:
//             return -1;
//         }
//     }

    archive_entry_rar::archive_entry_rar(archivepp::string name,
                                         uint64_t index,
                                         uint64_t uncompressed_size,
                                         uint64_t compressed_size,
                                         std::error_code & ec) :
        basic_archive_entry(std::move(name), index, uncompressed_size, compressed_size)
    {
    }

    archive_entry_rar::~archive_entry_rar()
    {
    }

//     std::string archive_entry_rar::get_contents(std::error_code & ec) const
//     {
//         std::unique_ptr<cb_context> context_ptr(new cb_context());
//         context_ptr->password = "testerino";
//         HANDLE handle = get_handle();

//         ::RARSetCallback(handle, rar_callback, reinterpret_cast<LPARAM>(context_ptr.get()));

//         RARHeaderDataEx header{};
//         uint64_t i = 0;
//         ec = make_ec(::RARReadHeaderEx(handle, &header));
//         while (!ec)
//         {
//             // Is the current index == to this entry's?
//             if (i != get_index())
//             {
//                 // No, skip this entry
// #ifdef ARCHIVEPP_USE_WSTRING
//                 ec = make_ec(::RARProcessFileW(handle, RAR_SKIP, nullptr, nullptr));
// #else
//                 ec = make_ec(::RARProcessFile(handle, RAR_SKIP, nullptr, nullptr));
// #endif
//                 if (ec)
//                     break;

//                 ++i;
//                 ec = make_ec(::RARReadHeaderEx(handle, &header));
//                 continue;
//             }

// #ifdef ARCHIVEPP_USE_WSTRING
//             ec = make_ec(::RARProcessFileW(handle, RAR_TEST, nullptr, nullptr));
// #else
//             ec = make_ec(::RARProcessFile(handle, RAR_TEST, nullptr, nullptr));
// #endif
//             break;
//         }

//         return std::string(std::move(context_ptr->buffer));
//     }
}
