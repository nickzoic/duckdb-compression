#include "compression_filesystem.hpp"

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
	std::cout << "CompressionFileSystem::CanHandleFile" << fpath << "\n";
	return StringUtil::EndsWith(fpath, ".bz2");
    }
    bool CompressionFileSystem::FileExists(const string &filename, optional_ptr<FileOpener> opener) {
	std::cout << "CompressionFileSystem::FileExists" << filename << "\n";
        if (!CanHandleFile(filename)) {
                return false;
        }
	struct stat buffer;
	return stat(filename.c_str(), &buffer) == 0;
    }
    vector<OpenFileInfo> CompressionFileSystem::Glob(const string &path, FileOpener *opener) {
	std::cout << "CompressionFileSystem::Glob" << path << "\n";
	if (FileExists(path)) return {OpenFileInfo(path)};
	return {};
    }
    bool CompressionFileSystem::ListFiles(const string &directory, const std::function<void(const string &, bool)> &callback, FileOpener *opener) {
	std::cout << "CompressionFileSystem::ListFiles" << directory << "\n";
	return false;
    }

    unique_ptr<FileHandle> CompressionFileSystem::OpenFile(const string &path, FileOpenFlags flags, optional_ptr<FileOpener> opener) {
	std::cout << "CompressionFileSystem::OpenFile" << path << "\n";
        if (flags.OpenForWriting()) {
                throw IOException("CompressionFileSystem is read-only: cannot open '%s' for writing", path);
        }
	throw IOException("CompressionFileSystem doesn't actually do anything yet");

        //auto handle = make_uniq<GithubFileHandle>(*this, path, flags);
    }

    void CompressionFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes, idx_t location) {

    }

    int64_t CompressionFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes) {
	    return 0;
    }


}
