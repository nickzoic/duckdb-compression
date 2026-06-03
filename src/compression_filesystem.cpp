#include "compression_filesystem.hpp"
#include "bzip_filehandle.hpp"
#include "lzma_filehandle.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/file_opener.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar/string_common.hpp"

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <sys/stat.h>

namespace duckdb {
    bool CompressionFileSystem::CanHandleFile(const string &fpath) {
	return StringUtil::EndsWith(fpath, ".bz2") or
		StringUtil::EndsWith(fpath, ".xz") or
		StringUtil::EndsWith(fpath, ".lzma");
    }
    bool CompressionFileSystem::FileExists(const string &filename, optional_ptr<FileOpener> opener) {
        if (!CanHandleFile(filename)) {
                return false;
        }
	struct stat buffer;
	return stat(filename.c_str(), &buffer) == 0;
    }
    vector<OpenFileInfo> CompressionFileSystem::Glob(const string &path, FileOpener *opener) {
	if (FileExists(path)) return {OpenFileInfo(path)};
	return {};
    }
    bool CompressionFileSystem::ListFiles(const string &directory, const std::function<void(const string &, bool)> &callback, FileOpener *opener) {
	return false;
    }
    int64_t CompressionFileSystem::GetFileSize(FileHandle &handle) {
	return 0;
    }

    unique_ptr<FileHandle> CompressionFileSystem::OpenFile(const string &path, FileOpenFlags flags, optional_ptr<FileOpener> opener) {
        if (flags.OpenForWriting()) {
                throw IOException("CompressionFileSystem is read-only: cannot open '%s' for writing", path);
        }
	unique_ptr<FileHandle> handle;
	if (StringUtil::EndsWith(path, ".bz2")) {
	    handle = make_uniq<BzipFileHandle>(*this, path, flags);
	} else if (StringUtil::EndsWith(path, ".xz") or StringUtil::EndsWith(path, ".lzma")) {
	    handle = make_uniq<LzmaFileHandle>(*this, path, flags);
	} else {
	    throw IOException("CompressionFileSystem: Unknown File Type");
	}
	return std::move(handle);
    }

    void CompressionFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes, idx_t location) {
	return handle.Read(buffer, nr_bytes, location);
    }

    int64_t CompressionFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes) {
	return handle.Read(buffer, nr_bytes);
    }

}
