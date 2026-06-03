#include "bzip_filehandle.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/file_system.hpp"
#include "duckdb/common/virtual_file_system.hpp"
#include "duckdb/main/query_context.hpp"

#include <bzlib.h>
#include <fstream>
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

}    
