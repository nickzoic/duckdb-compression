#pragma once

#include "duckdb/common/file_system.hpp"
#include "duckdb/common/http_util.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/main/query_context.hpp"

#include <lz4frame.h>
#include <iostream>

namespace duckdb {

    struct Lz4FileHandle : public FileHandle {
        public:
            Lz4FileHandle(FileSystem &fs, const string &path, FileOpenFlags flags);
	    void Close() override;

	    FILE *filePtr;
	    LZ4F_cctx *lz4f_cctx;
	    bool finished = false;
	    bool writing = false;
    };

    class Lz4FileSystem : public FileSystem {
        bool CanHandleFile(const string &fpath) override;
	vector<OpenFileInfo> Glob(const string &path, FileOpener *opener) override;
	bool FileExists(const string &filename, optional_ptr<FileOpener> opener = nullptr) override;
	bool ListFiles(const string &directory, const std::function<void(const string &, bool)> &callback, FileOpener *opener) override;
	void MoveFile(const string &source, const string &target, optional_ptr<FileOpener> opener = nullptr) override;
	int64_t GetFileSize(FileHandle &handle) override;
	bool OnDiskFile(FileHandle &handle) override {
                return false;
        }
	string GetName() const override {
	    return "Lz4FileSystem";
	}
	bool CanSeek() override {
	    return false;
	}
	bool SupportsGlobExtended() const override {
                return false;
        }
	unique_ptr<FileHandle> OpenFile(const string &path, FileOpenFlags flags,
                                        optional_ptr<FileOpener> opener = nullptr) override;
        int64_t Read(FileHandle &handle, void *buffer, int64_t nr_bytes) override;
        int64_t Write(FileHandle &handle, void *buffer, int64_t nr_bytes) override;

    };
}
