#include "bzip_filesystem.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/file_opener.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar/string_common.hpp"

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <sys/stat.h>

#include <bzlib.h>
#include <iostream>

namespace duckdb {

    BzipFileHandle::BzipFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags) : FileHandle(fs, path, flags) {
         std::cout << "BzipFileHandle::BzipFileHandle " << path << "\n";

	 filePtr = fopen(path.c_str(), "rb");
	 // XXX handle errors
	 int bzError;
	 bzFile = BZ2_bzReadOpen(&bzError, filePtr, 0, 0, NULL, 0);
	 // XXX handle errors
    }

    int64_t BzipFileHandle::Read(void *buffer, idx_t nr_bytes) {
         std::cout << "BzipFileHandle::Read 1\n";
	 int bzError;
	 return BZ2_bzRead(&bzError, bzFile, buffer, nr_bytes);
	 // XXX handle errors
    }
    int64_t BzipFileHandle::Read(QueryContext context, void *buffer, idx_t nr_bytes) {
         std::cout << "BzipFileHandle::Read 2\n";
	return 0;
    }
    void BzipFileHandle::Read(void *buffer, idx_t nr_bytes, idx_t location) {
         std::cout << "BzipFileHandle::Read 3\n";
    }
    void BzipFileHandle::Read(QueryContext context, void *buffer, idx_t nr_bytes, idx_t location) {
         std::cout << "BzipFileHandle::Read 4\n";
    }

    void BzipFileHandle::Close() {
         std::cout << "BzipFileHandle::Close\n";
	 fclose(filePtr);
    }

    bool BzipFileSystem::CanHandleFile(const string &fpath) {
	return StringUtil::EndsWith(fpath, ".bz2");
    }

    bool BzipFileSystem::FileExists(const string &filename, optional_ptr<FileOpener> opener) {
        if (!CanHandleFile(filename)) {
                return false;
        }
	struct stat buffer;
	return stat(filename.c_str(), &buffer) == 0;
    }

    vector<OpenFileInfo> BzipFileSystem::Glob(const string &path, FileOpener *opener) {
	if (FileExists(path)) return {OpenFileInfo(path)};
	return {};
    }

    bool BzipFileSystem::ListFiles(const string &directory, const std::function<void(const string &, bool)> &callback, FileOpener *opener) {
	return false;
    }

    int64_t BzipFileSystem::GetFileSize(FileHandle &handle) {
	return 0;
    }

    unique_ptr<FileHandle> BzipFileSystem::OpenFile(const string &path, FileOpenFlags flags, optional_ptr<FileOpener> opener) {
        if (flags.OpenForWriting()) {
                throw IOException("BzipFileSystem is read-only: cannot open '%s' for writing", path);
        }
	auto handle = make_uniq<BzipFileHandle>(*this, path, flags);
	return std::move(handle);
    }

    void BzipFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes, idx_t location) {
	auto &fh = handle.Cast<BzipFileHandle>();
	return fh.Read(buffer, nr_bytes, location);
    }

    int64_t BzipFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes) {
	auto &fh = handle.Cast<BzipFileHandle>();
	return fh.Read(buffer, nr_bytes);
    }

}
