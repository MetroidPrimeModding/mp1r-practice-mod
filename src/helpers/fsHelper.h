#pragma once

#include "nn/fs.h"
#include "nn/result.h"

namespace FsHelper {

  struct LoadData {
    const char *path;
    void *buffer;
    long bufSize;
  };

  nn::Result createDirectory(const char *path);
  nn::Result writeFileToPath(const void *buf, size_t size, const char *path);

  void loadFileFromPath(LoadData &loadData);
  bool tryLoadFileFromPath(LoadData &loadData);

  long getFileSize(const char *path);

  bool isFileExist(const char *path);
  bool isDirectoryExist(const char *path);
}
