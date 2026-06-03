#include "lz4_filesystem.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/file_opener.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar/string_common.hpp"

#include <cstdlib>
#include <cstring>

#include <iostream>
#include <sys/stat.h>

#include <lz4frame.h>
#include <iostream>
#include <string.h>
#include <errno.h>

namespace duckdb {

    Lz4FileHandle::Lz4FileHandle(FileSystem &fs, const string &path, FileOpenFlags flags) : FileHandle(fs, path, flags) {

	 int bzError;
         if (flags.OpenForWriting()) {
	     filePtr = fopen(path.c_str(), "wb");
	     if (!filePtr) {
	         throw IOException(strerror(errno));
	     }
	     LZ4F_createCompressionContext(&lz4f_cctx, LZ4F_VERSION);
	     writing = true;
	 } else if (flags.OpenForReading()) {
	     filePtr = fopen(path.c_str(), "rb");
	     if (!filePtr) {
	         throw IOException(strerror(errno));
	     }
	 } else {
	     // XXX should check flags more thoroughly
	     throw IOException("Unknown flags value");
	 }
    }

    void Lz4FileHandle::Close() {
	 LZ4F_freeCompressionContext(lz4f_cctx);
	 fclose(filePtr);
    }

    bool Lz4FileSystem::CanHandleFile(const string &fpath) {
	return StringUtil::EndsWith(fpath, ".lz4");
    }

    void Lz4FileSystem::MoveFile(const string &source, const string &target, optional_ptr<FileOpener> opener) {
	rename(source.c_str(), target.c_str());
    }

    bool Lz4FileSystem::FileExists(const string &filename, optional_ptr<FileOpener> opener) {
        if (!CanHandleFile(filename)) {
                return false;
        }
	struct stat buffer;
	return stat(filename.c_str(), &buffer) == 0;
    }

    vector<OpenFileInfo> Lz4FileSystem::Glob(const string &path, FileOpener *opener) {
	if (FileExists(path)) return {OpenFileInfo(path)};
	return {};
    }

    bool Lz4FileSystem::ListFiles(const string &directory, const std::function<void(const string &, bool)> &callback, FileOpener *opener) {
	return false;
    }

    int64_t Lz4FileSystem::GetFileSize(FileHandle &handle) {
	return 0;
    }

    unique_ptr<FileHandle> Lz4FileSystem::OpenFile(const string &path, FileOpenFlags flags, optional_ptr<FileOpener> opener) {
	auto handle = make_uniq<Lz4FileHandle>(*this, path, flags);
	return std::move(handle);
    }

    int64_t Lz4FileSystem::Read(FileHandle &handle, void *buffer, int64_t nr_bytes) {
	auto &fh = handle.Cast<Lz4FileHandle>();
	throw IOException("not implemented");
    }

    int64_t Lz4FileSystem::Write(FileHandle &handle, void *buffer, int64_t nr_bytes) {
	auto &fh = handle.Cast<Lz4FileHandle>();
        const LZ4F_preferences_t *prefsPtr = NULL;
	const LZ4F_compressOptions_t *cOptPtr = NULL;

	const int64_t srcSizeMax = 8192;
	std::vector<uint8_t>outBuffer(LZ4F_compressBound(srcSizeMax, prefsPtr));

	size_t n = LZ4F_compressBegin(fh.lz4f_cctx, outBuffer.data(), outBuffer.size(), prefsPtr);
        int m = fwrite(outBuffer.data(), 1, n, fh.filePtr);
	if (n != m) throw IOException("Write Failed");

        for (int64_t offset=0; offset < nr_bytes; offset += srcSizeMax) {	
	    n = LZ4F_compressUpdate(
			    fh.lz4f_cctx,
			    outBuffer.data(),
			    outBuffer.size(),
			    (char *)buffer+offset,
			    std::min(nr_bytes-offset, srcSizeMax),
			    cOptPtr
	    );
            m = fwrite(outBuffer.data(), 1, n, fh.filePtr);
	    if (n != m) throw IOException("Write Failed");
	}

	n = LZ4F_compressEnd(fh.lz4f_cctx, outBuffer.data(), outBuffer.size(), cOptPtr);
        m = fwrite(outBuffer.data(), 1, n, fh.filePtr);
	if (n != m) throw IOException("Write Failed");

	return nr_bytes;
    }
}
