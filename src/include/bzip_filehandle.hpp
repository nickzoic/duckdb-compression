#include "duckdb/common/file_system.hpp"
#include "duckdb/main/query_context.hpp"

#include <bzlib.h>
#include <iostream>

namespace duckdb {

    struct BzipFileHandle : public FileHandle {
        public:
            BzipFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags);
            void Close() override;
	    int64_t Read(void *buffer, idx_t nr_bytes);
            int64_t Read(QueryContext context, void *buffer, idx_t nr_bytes);
            void Read(void *buffer, idx_t nr_bytes, idx_t location);
            void Read(QueryContext context, void *buffer, idx_t nr_bytes, idx_t location);
	private:
	    FILE *filePtr;
	    BZFILE *bzFile;

    };
}
