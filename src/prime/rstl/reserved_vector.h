#ifndef _RSTL_RESERVED_VECTOR
#define _RSTL_RESERVED_VECTOR

#include "types.h"
#include <Stream/CInputStream.h>


namespace rstl {
    template < typename T, int N >
    class reserved_vector {
        int x0_count;
        uchar x4_data[N * sizeof(T)];
    public:
        reserved_vector() : x0_count(0) {}
        explicit reserved_vector(const T& value) : x0_count(N) { uninitialized_fill_n(data(), N, value); }
        explicit reserved_vector(int count, const T& value);
        reserved_vector(CInputStream& in);

        inline T* data() { return reinterpret_cast< T* >(x4_data); }
        inline const T* data() const { return reinterpret_cast< const T* >(x4_data); }
        inline bool empty() const { return size() == 0; }
        inline int size() const { return x0_count; }
        inline int capacity() const { return N; }
        inline T& front() { return data()[0]; }
        inline const T& front() const { return data()[0]; }
        inline T& back() { return data()[x0_count - 1]; }
        inline const T& back() const { return data()[x0_count - 1]; }
        inline T& operator[](int idx) { return data()[idx]; }
        inline const T& operator[](int idx) const { return data()[idx]; }
    };

} // namespace rstl

#endif // _RSTL_RESERVED_VECTOR