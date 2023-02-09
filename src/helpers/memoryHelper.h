#pragma once

#include "logger/Logger.hpp"


namespace MemoryHelper {
    template<class T>
    void logMemory(T *ptr);

    void logMemory(void *ptr, size_t size);

    void logBytes(void *ptr, size_t size);
}

