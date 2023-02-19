#pragma once

#include <cstddef>
#include <cstdint>

template<typename Res>
inline Res *GetField(const void *thiz, size_t offset) {
  size_t ptr = ((size_t)thiz + offset);
  return (Res*)ptr;
};

inline size_t GetVtable(const void *thiz) {
  return *GetField<size_t>(thiz, 0);
}

template<typename T>
inline T VtableLookup(const void *thiz, size_t functionOffset) {
  size_t *vtable = *GetField<size_t*>(thiz, 0);
  size_t ptr = vtable[functionOffset + 2];
//  OSReport("VT %x offset %x ptr %x\n", (int)vtable, functionOffset + 2, ptr);
  return (T)ptr;
}
