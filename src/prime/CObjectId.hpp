#pragma once

namespace rstl {
  template<typename T, typename F, typename M>
  class basic_string;

  template<typename T>
  class char_traits;

  class rmemory_allocator;
}

class CInputStream;

class COutputStream;

class CGuid {
public:
  union {
    char data[64];
    struct {
      uint64_t low;
      uint64_t high;
    };
  };

};

class CObjectId : public CGuid {
  CObjectId(CInputStream &);
  void Invalid(void);
  void FromString(rstl::basic_string<char, rstl::char_traits<char>, rstl::rmemory_allocator> const &);
  void PutTo(COutputStream &);
  void GetPutToSize(void);
  void operator<(CObjectId const &);
public:

};