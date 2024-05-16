#pragma once

#include <prime/rstl/string.h>
class CInputStream;

class COutputStream;

class CGuid {
public:

  CGuid() { low = 0; high = 0; }

  CGuid(CInputStream &);
  CGuid(char const(&)[37]);

  void PutTo(COutputStream &) const;
  int GetPutToSize(void) const; // return 0x10;
  void FromString(rstl::string const&);
  void SetFromString(rstl::string const&);
  void IsGuidString(rstl::string const&);
  rstl::string AsString(void) const;
  int GetHashValue(void) const;
  void HexDWord(char const*);

  union {
    char data[64];
    struct {
      uint64_t low;
      uint64_t high;
    };
  };

};

class CObjectId : public CGuid {
  public:
  CObjectId() : CGuid() {}

  CObjectId(CInputStream &);

  explicit inline CObjectId(char const(&str)[37]) : CGuid(str) {}

  static CObjectId* Invalid(void);
  static CObjectId* FromString(rstl::string const&);

  void PutTo(COutputStream &);
  void GetPutToSize(void);
  void operator<(CObjectId const&);
};