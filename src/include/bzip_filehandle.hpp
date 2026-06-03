#include "duckdb/common/file_system.hpp"

namespace duckdb {

    struct BzipFileHandle : public FileHandle {
        public:
            BzipFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags);
            void Close() override;
            vector<char> buffer;
            bool loaded = false;
    };
}
