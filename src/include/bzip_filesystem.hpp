#pragma once

#include "duckdb/common/file_system.hpp"
#include "duckdb/common/http_util.hpp"
#include "duckdb/common/multi_file/multi_file_list.hpp"

#include <bzlib.h>
#include <iostream>

namespace duckdb {

    struct BzipFileHandle : public FileHandle {
        public:
            BzipFileHandle(FileSystem &fs, const string &path, FileOpenFlags flags);
	    void Close() override;

	    FILE *filePtr;
	    BZFILE *bzFile;
	    bool finished = false;
	    bool writing = false;
    };

    class BzipFileSystem : public FileSystem {
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
        int64_t Read(FileHandle &handle, void *buffer, int64_t nr_bytes) override;
        int64_t Write(FileHandle &handle, void *buffer, int64_t nr_bytes) override;

    };
}
