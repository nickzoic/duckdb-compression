#include "duckdb/common/file_system.hpp"
#include "duckdb/main/query_context.hpp"

#include <lzma.h>
#include <iostream>

namespace duckdb {

    struct LzmaFileHandle : public FileHandle {
        public:
            LzmaFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags);
            void Close() override;
	    int64_t Read(void *buffer, idx_t nr_bytes);
	private:
	    std::vector<uint8_t> buffer;
    };
}
