#include <archivepp/secure_allocator.hpp>

#if defined(ARCHIVEPP_HAS_RTLZEROSECUREMEMORY)
#   include <Windows.h>
#elif defined(ARCHIVEPP_HAS_MEMSET_S)
#   include <string.h>
#endif
namespace archivepp
{
    static volatile void * scrub_chunks_8(volatile void * ptr, size_t & remaining)
    {
        volatile uint8_t * chunk = reinterpret_cast<volatile uint8_t*>(ptr);

        while (remaining > 0)
        {
            if (remaining < 1)
                break;

            *chunk++ = 0;
            --remaining;
        }

        return chunk;
    }

    static volatile void * scrub_chunks_16(volatile void * ptr, size_t & remaining)
    {
        volatile uint16_t * chunk = reinterpret_cast<volatile uint16_t*>(ptr);

        while (remaining > 0)
        {
            if (remaining < 2)
                break;

            *chunk++ = 0;
            remaining -= 2;
        }

        return chunk;
    }

    static volatile void * scrub_chunks_32(volatile void * ptr, size_t & remaining)
    {
        volatile uint32_t * chunk = reinterpret_cast<volatile uint32_t*>(ptr);

        while (remaining > 0)
        {
            if (remaining < 4)
                break;

            *chunk++ = 0;
            remaining -= 4;
        }

        return chunk;
    }

#ifdef ARCHIVEPP_ARCH_64
    static volatile void * scrub_chunks_64(volatile void * ptr, size_t & remaining)
    {
        volatile uint64_t * chunk = reinterpret_cast<volatile uint64_t*>(ptr);

        while (remaining > 0)
        {
            if (remaining < 8)
                break;

            *chunk++ = 0;
            remaining -= 8;
        }

        return chunk;
    }
#endif

    void secure_scrub(volatile void * ptr, size_t size)
    {
#if defined(ARCHIVEPP_HAS_RTLZEROSECUREMEMORY)
        ::RtlZeroSecureMemory(ptr, size);
#elif defined(ARCHIVEPP_HAS_MEMSET_S)
        // http://en.cppreference.com/w/c/string/byte/memset
        // memset may be optimized away (under the as-if rules) if the object modified by this function is not accessed again for the rest of its lifetime.
        // For that reason, this function cannot be used to scrub memory (e.g. to fill an array that stored a password with zeroes).
        // This optimization is prohibited for memset_s: it is guaranteed to perform the memory write.
        ::memset_s(ptr, 0, size, size);
#else
        // Imitate what I've seen MSVC does for memset's optimizations.
        // It repeatedly fills the largest chunk it can with movsb, movsw, movsd, or movsq.
        // Maybe use XMM and YMM instructions in the future for large allocations.

        size_t remaining = size;
#   ifdef ARCHIVEPP_ARCH_64
        ptr = scrub_chunks_64(ptr, remaining);
#   endif
        ptr = scrub_chunks_32(ptr, remaining);
        ptr = scrub_chunks_16(ptr, remaining);
        scrub_chunks_8(ptr, remaining);
#endif
    }
}
