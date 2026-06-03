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
#include <string.h>
#include <errno.h>

namespace duckdb {

    void throw_bzip_errors(int bzError) {
	// XXX should probably use BZ2_bzerror
	// check out state of v1.0.8 
	switch (bzError) {
	    case BZ_SEQUENCE_ERROR:
		throw IOException("Bzip2 Sequence Error");
	    case BZ_PARAM_ERROR:
		throw IOException("Bzip2 Parameter Error");
	    case BZ_MEM_ERROR:
		throw IOException("Bzip2 Memory Error");
	    case BZ_DATA_ERROR:
		throw IOException("Bzip2 Data Error");
	    case BZ_DATA_ERROR_MAGIC:
		throw IOException("Bzip2 Data (Magic) Error");
	    case BZ_IO_ERROR:
		throw IOException("Bzip2 IO Error");
	    case BZ_UNEXPECTED_EOF:
		throw IOException("Bzip2 Unexpected EOF");
	    case BZ_OUTBUFF_FULL:
		throw IOException("Bzip2 Output Buffer Error");
	    case BZ_CONFIG_ERROR:
		throw IOException("Bzip2 Config Error");
	};
    }

    BzipFileHandle::BzipFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags) : FileHandle(fs, path, flags) {

	 int bzError;
         if (flags.OpenForWriting()) {
	     filePtr = fopen(path.c_str(), "wb");
	     if (!filePtr) {
	         throw IOException(strerror(errno));
	     }
	     bzFile = BZ2_bzWriteOpen(&bzError, filePtr, 4, 0, 30);
	     writing = true;
	 } else if (flags.OpenForReading()) {
	     filePtr = fopen(path.c_str(), "rb");
	     if (!filePtr) {
	         throw IOException(strerror(errno));
	     }
	     bzFile = BZ2_bzReadOpen(&bzError, filePtr, 0, 0, NULL, 0);
	 } else {
	     // XXX should check flags more thoroughly
	     throw IOException("Unknown flags value");
	 }
	 throw_bzip_errors(bzError);
    }

    void BzipFileHandle::Close() {
	 int bzError;
	 if (writing) {
             BZ2_bzWriteClose(&bzError, bzFile, 0, NULL, NULL);
	 } else {
	     BZ2_bzReadClose(&bzError, bzFile);
	 }
	 throw_bzip_errors(bzError);
	 fclose(filePtr);
    }

    bool BzipFileSystem::CanHandleFile(const string &fpath) {
	return StringUtil::EndsWith(fpath, ".bz2");
    }

    void BzipFileSystem::MoveFile(const string &source, const string &target, optional_ptr<FileOpener> opener) {
	rename(source.c_str(), target.c_str());
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
	auto handle = make_uniq<BzipFileHandle>(*this, path, flags);
	return std::move(handle);
    }

    int64_t BzipFileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes) {
	auto &fh = handle.Cast<BzipFileHandle>();
	if (fh.finished) return 0;

	int bzError;
	int64_t r = BZ2_bzRead(&bzError, fh.bzFile, buffer, nr_bytes);
	if (bzError == BZ_STREAM_END) {
	    fh.finished = true;
	} else if (bzError != BZ_OK) {
            int bzError2;		
	    BZ2_bzReadClose(&bzError2, fh.bzFile);
	    throw_bzip_errors(bzError);
	}
	return r;
    }

    int64_t BzipFileSystem::Write(FileHandle &handle, void *buffer, int64_t nr_bytes) {
	auto &fh = handle.Cast<BzipFileHandle>();
	int bzError, bzError2;
	BZ2_bzWrite(&bzError, fh.bzFile, buffer, nr_bytes);
	if (bzError != BZ_OK) {
	    BZ2_bzWriteClose(&bzError2, fh.bzFile, 1, NULL, NULL);
	    throw_bzip_errors(bzError);
	}
	return nr_bytes;
    }
}
