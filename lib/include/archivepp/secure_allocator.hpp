#pragma once

#include <cstddef>
#include <limits>
#include <memory>

namespace archivepp
{
    void secure_scrub(volatile void * ptr, size_t size);

    template<class Type>
    struct secure_allocator
    {
    public:
        typedef Type value_type;
        typedef Type * pointer;
        typedef Type const * const_pointer;
        typedef Type & reference;
        typedef Type const & const_reference;
        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        template<class OtherType>
        struct rebind
        {
            typedef secure_allocator<OtherType> other;
        };

        secure_allocator() noexcept {}
        secure_allocator(secure_allocator const & other) noexcept {}

        template<class OtherType>
        secure_allocator(secure_allocator<OtherType> const & other) noexcept {}

        ~secure_allocator() {}

        pointer address(reference x) const noexcept
        {
            return std::addressof(x);
        }

        const_pointer address(const_reference x) const noexcept
        {
            return std::addressof(x);
        }

        pointer allocate(size_type n)
        {
            if (n > (std::numeric_limits<size_type>::max() / sizeof(value_type)))
                throw std::bad_alloc();
            
            return static_cast<pointer>(::operator new(n * sizeof(value_type)));
        }

        void deallocate(pointer ptr, size_type n)
        {
            secure_scrub(ptr, n);

            ::operator delete(ptr);
        }

        template<class OtherType, class... Args>
        void construct(Type * ptr, Args &&... args)
        {
            ::new (static_cast<void*>(ptr)) OtherType(std::forward<Args>(args)...);
        }

        void destroy(pointer ptr)
        {
            ((value_type *)ptr)->~value_type();
        }

        template<class OtherType>
        void destroy(OtherType * ptr)
        {
            ptr->~OtherType();
        }
    protected:
    private:
    };

    template<class T>
    bool operator==(secure_allocator<T> const &, secure_allocator<T> const &)
    {
        return true;
    }

    template<class T>
    bool operator!=(secure_allocator<T> const &, secure_allocator<T> const &)
    {
        return false;
    }
}
