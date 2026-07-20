#include "duckdb/common/file_system.hpp"

#include <lzma.h>
#include <iostream>

namespace duckdb {

struct LzmaFileHandle : public FileHandle {
public:
  LzmaFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags);
  void Close() override;
  int64_t Read(void *buffer, idx_t nr_bytes);
  int64_t Read(QueryContext context, void *buffer, idx_t nr_bytes);
  void Read(void *buffer, idx_t nr_bytes, idx_t location);
  void Read(QueryContext context, void *buffer, idx_t nr_bytes, idx_t location);

private:
  std::vector<uint8_t> buffer;
};

class LzmaFileSystem : public FileSystem {
  bool CanHandleFile(const string &fpath) override;
  vector<OpenFileInfo> Glob(const string &path, FileOpener *opener) override;
  bool FileExists(const string &filename,
                  optional_ptr<FileOpener> opener = nullptr) override;
  bool ListFiles(const string &directory,
                 const std::function<void(const string &, bool)> &callback,
                 FileOpener *opener) override;
  int64_t GetFileSize(FileHandle &handle) override;
  bool OnDiskFile(FileHandle &handle) override { return false; }
  string GetName() const override { return "LzmaFileSystem"; }
  bool CanSeek() override { return false; }
  bool SupportsGlobExtended() const override { return false; }
  unique_ptr<FileHandle>
  OpenFile(const string &path, FileOpenFlags flags,
           optional_ptr<FileOpener> opener = nullptr) override;
  void Read(FileHandle &handle, void *buffer, int64_t nr_bytes,
            idx_t location) override;
  int64_t Read(FileHandle &handle, void *buffer, int64_t nr_bytes) override;
};
} // namespace duckdb
