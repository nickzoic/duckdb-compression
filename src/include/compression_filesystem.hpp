#pragma once

#include "duckdb/common/file_system.hpp"
#include "duckdb/common/http_util.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"

namespace duckdb {
    class CompressionFileSystem : public FileSystem {
        bool CanHandleFile(const string &fpath) override;
	string GetName() const override {
	    return "CompressionFileSystem";
	}
	bool CanSeek() override {
	    return false;
	}
    };
}
