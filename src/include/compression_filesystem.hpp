#pragma once

#include "duckdb/common/file_system.hpp"
#include "duckdb/common/http_util.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"

namespace duckdb {
    class CompressionFileSystem : public FileSystem {
        bool CanHandleFile(const string &fpath) override;
	vector<OpenFileInfo> Glob(const string &path, FileOpener *opener) override;
	bool FileExists(const string &filename, optional_ptr<FileOpener> opener = nullptr) override;
	bool ListFiles(const string &directory, const std::function<void(const string &, bool)> &callback, FileOpener *opener) override;
	string GetName() const override {
	    return "CompressionFileSystem";
	}
	bool CanSeek() override {
	    return false;
	}
	bool SupportsGlobExtended() const override {
                return false;
        }
	unique_ptr<FileHandle> OpenFile(const string &path, FileOpenFlags flags,
                                        optional_ptr<FileOpener> opener = nullptr) override;
	void Read(FileHandle &handle, void *buffer, int64_t nr_bytes, idx_t location) override;
        int64_t Read(FileHandle &handle, void *buffer, int64_t nr_bytes) override;

    };
}
