#include "compression_filesystem.hpp"

#include "duckdb/common/exception.hpp"
#include "duckdb/common/file_opener.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/common/string_util.hpp"
#include "duckdb/function/scalar/string_common.hpp"

#include <cstdlib>
#include <cstring>

namespace duckdb {

    bool CompressionFileSystem::CanHandleFile(const string &fpath) {
        return false;
    }

}
