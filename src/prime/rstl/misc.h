#pragma once

// not fully implemented forward decls/unverified structs from prime decomp
namespace rstl {
    template <typename T, typename F, typename M>
    class basic_string;
    template <typename T>
    class char_traits;

    class rmemory_allocator;

    template <typename T, class Allocator>
    class vector {
        Allocator allocator;
        int count;
        int capacity;
        T* items;
    };

    template <typename T, int size, class Allocator>
    class growable_reserved_vector;

    template <typename T>
    class shared_owned_memory_allocator;

    class owned_memory_arena_allocator;
}