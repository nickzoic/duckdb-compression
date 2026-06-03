#pragma once

#include "duckdb/common/file_system.hpp"
#include "duckdb/common/http_util.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"
#include "duckdb/main/query_context.hpp"

#include <bzlib.h>
#include <iostream>

namespace duckdb {

    struct BzipFileHandle : public FileHandle {
        public:
            BzipFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags);
            virtual void Close() override;
	    virtual int64_t Read(void *buffer, idx_t nr_bytes);
            virtual int64_t Read(QueryContext context, void *buffer, idx_t nr_bytes);
            virtual void Read(void *buffer, idx_t nr_bytes, idx_t location);
            virtual void Read(QueryContext context, void *buffer, idx_t nr_bytes, idx_t location);
	private:
	    FILE *filePtr;
	    BZFILE *bzFile;

    };

    class BzipFileSystem : public FileSystem {
        bool CanHandleFile(const string &fpath) override;
	vector<OpenFileInfo> Glob(const string &path, FileOpener *opener) override;
	bool FileExists(const string &filename, optional_ptr<FileOpener> opener = nullptr) override;
	bool ListFiles(const string &directory, const std::function<void(const string &, bool)> &callback, FileOpener *opener) override;
	int64_t GetFileSize(FileHandle &handle) override;
	bool OnDiskFile(FileHandle &handle) override {
                return false;
        }
	string GetName() const override {
	    return "BzipFileSystem";
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
