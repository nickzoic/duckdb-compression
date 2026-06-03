#include "lzma_filehandle.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/file_system.hpp"
#include "duckdb/common/virtual_file_system.hpp"
#include "duckdb/main/query_context.hpp"

#include <lzma.h>
#include <fstream>
#include <iostream>

namespace duckdb {

    LzmaFileHandle::LzmaFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags) : FileHandle(fs, path, flags) {
         std::cout << "LzmaFileHandle::LzmaFileHandle " << path << "\n";
	 throw IOException("LZMA not implemented yet");
    }

    int64_t LzmaFileHandle::Read(void *buffer, idx_t nr_bytes) {
         std::cout << "LzmaFileHandle::Read 1\n";
	 throw IOException("LZMA not implemented yet");
    }

    void LzmaFileHandle::Close() {
         std::cout << "LzmaFileHandle::Close\n";
	 throw IOException("LZMA not implemented yet");
    }

}    
