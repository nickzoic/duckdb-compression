#include "lzma_filesystem.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/file_opener.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar/string_common.hpp"

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <sys/stat.h>

#include <lzma.h>

namespace duckdb {

LzmaFileHandle::LzmaFileHandle(FileSystem &fs, const string &path,
                               FileOpenFlags flags)
    : FileHandle(fs, path, flags) {
  std::cout << "LzmaFileHandle::LzmaFileHandle " << path << "\n";
  throw IOException("LzmaFileSystem not implemented yet");
}

int64_t LzmaFileHandle::Read(void *buffer, idx_t nr_bytes) {
  std::cout << "LzmaFileHandle::Read 1\n";
  throw IOException("LzmaFileSystem not implemented yet");
}
int64_t LzmaFileHandle::Read(QueryContext context, void *buffer,
                             idx_t nr_bytes) {
  std::cout << "LzmaFileHandle::Read 2\n";
  throw IOException("LzmaFileSystem not implemented yet");
}
void LzmaFileHandle::Read(void *buffer, idx_t nr_bytes, idx_t location) {
  std::cout << "LzmaFileHandle::Read 3\n";
  throw IOException("LzmaFileSystem not implemented yet");
}
void LzmaFileHandle::Read(QueryContext context, void *buffer, idx_t nr_bytes,
                          idx_t location) {
  std::cout << "LzmaFileHandle::Read 4\n";
  throw IOException("LzmaFileSystem not implemented yet");
}

void LzmaFileHandle::Close() {
  std::cout << "LzmaFileHandle::Close\n";
  throw IOException("LzmaFileSystem not implemented yet");
}

bool LzmaFileSystem::CanHandleFile(const string &fpath) {
  return StringUtil::EndsWith(fpath, ".lzma") or
         StringUtil::EndsWith(fpath, ".xz");
}

bool LzmaFileSystem::FileExists(const string &filename,
                                optional_ptr<FileOpener> opener) {
  if (!CanHandleFile(filename)) {
    return false;
  }
  struct stat buffer;
  return stat(filename.c_str(), &buffer) == 0;
}

vector<OpenFileInfo> LzmaFileSystem::Glob(const string &path,
                                          FileOpener *opener) {
  if (FileExists(path))
    return {OpenFileInfo(path)};
  return {};
}

bool LzmaFileSystem::ListFiles(
    const string &directory,
    const std::function<void(const string &, bool)> &callback,
    FileOpener *opener) {
  return false;
}

int64_t LzmaFileSystem::GetFileSize(FileHandle &handle) { return 0; }

unique_ptr<FileHandle>
LzmaFileSystem::OpenFile(const string &path, FileOpenFlags flags,
                         optional_ptr<FileOpener> opener) {
  if (flags.OpenForWriting()) {
    throw IOException(
        "LzmaFileSystem is read-only: cannot open '%s' for writing", path);
  }
  auto handle = make_uniq<LzmaFileHandle>(*this, path, flags);
  return std::move(handle);
}

void LzmaFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes,
                          idx_t location) {
  std::cout << "LzmaFileSystem::Read 1\n";
  auto &fh = handle.Cast<LzmaFileHandle>();
  return fh.Read(buffer, nr_bytes, location);
}

int64_t LzmaFileSystem::Read(FileHandle &handle, void *buffer,
                             int64_t nr_bytes) {
  std::cout << "LzmaFileSystem::Read 2\n";
  auto &fh = handle.Cast<LzmaFileHandle>();
  return fh.Read(buffer, nr_bytes);
}

} // namespace duckdb
