#include "bzip_filehandle.hpp"

#include "duckdb/common/file_system.hpp"

#include <iostream>

namespace duckdb {

    BzipFileHandle::BzipFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags) : FileHandle(fs, path, flags) {
         std::cout << "BzipFileHandle::BzipFileHandle " << path << "\n";
    }

    void BzipFileHandle::Close() {
         std::cout << "BzipFileHandle::Close\n";
    }

}    
