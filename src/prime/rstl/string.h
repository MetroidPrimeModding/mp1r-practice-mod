#ifndef _RSTL_STRING
#define _RSTL_STRING

#include "types.h"

#include "prime/rstl/misc.h"

class CInputStream;
class COutputStream;

namespace rstl {
    template < typename _CharTp >
    struct char_traits {};

    template < typename _CharTp, typename Traits = char_traits< _CharTp >,
              typename Alloc = rmemory_allocator >
    class basic_string {
        struct COWData {
            uint x0_capacity;
            uint x4_refCount;
            _CharTp* x8_data;
        };

        const _CharTp* x0_ptr;
        COWData* x4_cow;
        uint x8_size;
        uint _pad; // Alloc?

        void internal_prepare_to_write(void);
        void internal_prepare_to_write(ulong len, bool);
        void internal_allocate(int size);

        void internal_dereference();

        static const _CharTp mNull;

    public:
        struct literal_t {};

        basic_string() : x0_ptr(&mNull), x4_cow(nullptr), x8_size(0) {}

        basic_string(literal_t, const _CharTp* data, const Alloc&);

        basic_string(const basic_string& str);

        basic_string(CInputStream& in, const Alloc&);

        basic_string(const _CharTp* data, ulong size, const Alloc&);

        basic_string(_CharTp data, const Alloc&);

        basic_string(ulong size, _CharTp data, const Alloc&);

        ~basic_string();

        size_t size() const;
        void reserve(int len);

        void assign(const basic_string&);
        basic_string& operator=(const basic_string& other);
        basic_string operator+(const _CharTp*);
        void append(const basic_string& other);
        void append(int, _CharTp);
        void append(const _CharTp*, int);

        int _eq_helper(const basic_string& other) const;
        bool operator==(const basic_string& other) const;
        bool operator<(const basic_string& other) const;

        const _CharTp* data() const { return x0_ptr; }
        void PutTo(COutputStream& out) const;
        const _CharTp at(int idx) const { return data()[idx]; }
    };

    template < typename _CharTp, typename Traits, typename Alloc >
    bool basic_string< _CharTp, Traits, Alloc >::operator==(const basic_string& other) const {
        return _eq_helper(other) == 0;
    }

    template < typename _CharTp, typename Traits, typename Alloc >
    bool basic_string< _CharTp, Traits, Alloc >::operator<(const basic_string& other) const {
        return _eq_helper(other) < 0;
    }

    // template <>
    // const char basic_string<char>::mNull = 0;
    // template <>
    // const wchar_t basic_string<wchar_t>::mNull = 0;

    typedef basic_string< wchar_t > wstring;
    typedef basic_string< char > string;

    wstring wstring_l(const wchar_t* data);

    string string_l(const char* data);

    string operator+(const string& a, const string& b);

    static inline string operator+(const string& a, char c) {
        string result(a);
        result.append(1, c);
        return result;
    }
} // namespace rstl

#endif // _RSTL_STRING