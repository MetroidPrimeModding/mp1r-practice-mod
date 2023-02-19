#include "fsHelper.h"
#include "logger/Logger.hpp"
#include "diag/assert.hpp"
#include "init.h"

namespace FsHelper {
  nn::Result createDirectory(const char *path) {
    nn::fs::DirectoryHandle handle;
    if (!nn::fs::OpenDirectory(&handle, path, nn::fs::OpenDirectoryMode_All)) {
      Logger::log("dir exists %s\n", path);
      nn::fs::CloseDirectory(handle);
      return 0;
    }

    if (nn::fs::CreateDirectory(path)) {
      Logger::log("Failed to Create directory %s\n", path);
      return 1;
    }

    Logger::log("Created directory %s\n", path);
    return 0;
  }

  nn::Result writeFileToPath(const void *buf, size_t size, const char *path) {
    nn::fs::FileHandle handle;

    if (!isFileExist(path)) {
//      Logger::log("Removing Previous File.\n");
//      nn::fs::DeleteFile(path); // remove previous file
      if (nn::fs::CreateFile(path, size)) {
        Logger::log("Failed to Create File %s\n", path);
        return 1;
      }
    }

    if (nn::fs::OpenFile(&handle, path, nn::fs::OpenMode_Write)) {
      Logger::log("Failed to Open File %s\n", path);
      return 1;
    }

    if (nn::fs::SetFileSize(handle, size)) {
      Logger::log("Failed to set file size %s\n", path);
      return 1;
    }

    if (nn::fs::WriteFile(handle, 0, buf, size, nn::fs::WriteOption::CreateOption(nn::fs::WriteOptionFlag_Flush))) {
      Logger::log("Failed to Write to File %s\n", path);
      return 1;
    }

    Logger::log("Successfully wrote file to: %s!\n", path);

    nn::fs::CloseFile(handle);

    return 0;
  }

  // make sure to free buffer after usage is done
  void loadFileFromPath(LoadData &loadData) {

    nn::fs::FileHandle handle;

    EXL_ASSERT(FsHelper::isFileExist(loadData.path), "Failed to Find File!\nPath: %s", loadData.path);

    R_ABORT_UNLESS(nn::fs::OpenFile(&handle, loadData.path, nn::fs::OpenMode_Read))

    long size = 0;
    nn::fs::GetFileSize(&size, handle);
    loadData.buffer = nn::init::GetAllocator()->Allocate(size);
    loadData.bufSize = size;

    EXL_ASSERT(loadData.buffer, "Failed to Allocate Buffer! File Size: %ld", size);

    R_ABORT_UNLESS(nn::fs::ReadFile(handle, 0, loadData.buffer, size))

    nn::fs::CloseFile(handle);
  }

  bool tryLoadFileFromPath(LoadData &loadData) {
    if (!FsHelper::isFileExist(loadData.path)) {
      Logger::log("File doedsn't exit %s\n", loadData.path);
      return false;
    }

    nn::fs::FileHandle handle;
    if (nn::fs::OpenFile(&handle, loadData.path, nn::fs::OpenMode_Read)) {
      Logger::log("Failed to open read %s\n", loadData.path);
      return false;
    }

    long size = 0;
    nn::fs::GetFileSize(&size, handle);
    loadData.buffer = nn::init::GetAllocator()->Allocate(size);
    loadData.bufSize = size;

    EXL_ASSERT(loadData.buffer, "Failed to Allocate Buffer! File Size: %ld", size);

    R_ABORT_UNLESS(nn::fs::ReadFile(handle, 0, loadData.buffer, size))

    nn::fs::CloseFile(handle);
    return true;
  }


  long getFileSize(const char *path) {
    nn::fs::FileHandle handle;
    long result = -1;

    nn::Result openResult = nn::fs::OpenFile(&handle, path, nn::fs::OpenMode::OpenMode_Read);

    if (openResult.isSuccess()) {
      nn::fs::GetFileSize(&result, handle);
      nn::fs::CloseFile(handle);
    }

    return result;
  }

  bool isFileExist(const char *path) {
    nn::fs::DirectoryEntryType type;
    nn::fs::GetEntryType(&type, path);

    return type == nn::fs::DirectoryEntryType_File;
  }

  bool isDirectoryExist(const char *path) {
    nn::fs::DirectoryEntryType type;
    nn::fs::GetEntryType(&type, path);

    Logger::log("TYPE %d\n", type);
    return type == nn::fs::DirectoryEntryType_Directory;
  }
}