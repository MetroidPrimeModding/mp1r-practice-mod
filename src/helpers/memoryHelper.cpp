#include "memoryHelper.h"

namespace MemoryHelper {
    template<class T>
    void logMemory(T *ptr) {
        int size = sizeof(T);
        auto all = (unsigned char *) ptr;

        for (int i = 0; i < size; i++) {
            Logger::log("%02X", all[i]);
        }
        Logger::log("\n");
    }

    void logMemory(void *ptr, size_t size) {
        auto all = (unsigned char *) ptr;
        for (int i = 0; i < size; i++) {
            Logger::log("%02X", all[i]);
        }
        Logger::log("\n");
    }

    void logBytes(void *ptr, size_t size) {
        auto *b = (unsigned char *) ptr;

        for (int i = size - 1; i >= 0; i--) {
            for (int j = 7; j >= 0; j--) {
                unsigned char byte = (b[i] >> j) & 1;
                Logger::log("%u", byte);
            }
        }
        Logger::log("\n");
    }
}
