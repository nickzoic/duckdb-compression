#define DUCKDB_EXTENSION_MAIN

#include "compression_extension.hpp"
#include "compression_filesystem.hpp"
#include "duckdb.hpp"

namespace duckdb {

    static void LoadInternal(ExtensionLoader &loader) {
        auto &db = loader.GetDatabaseInstance();
        auto &db_fs = db.GetFileSystem();
        db_fs.RegisterSubSystem(make_uniq<CompressionFileSystem>());
    }

    void CompressionExtension::Load(ExtensionLoader &loader) {
        LoadInternal(loader);
    }

    std::string CompressionExtension::Name() {
        return "compression";
    }

    std::string CompressionExtension::Version() const {
        return "0.0.1";
    }

} // namespace duckdb

extern "C" {
    DUCKDB_CPP_EXTENSION_ENTRY(gh, loader) {
	duckdb::LoadInternal(loader);
    }
}
