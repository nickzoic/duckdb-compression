# duckdb-compression

A [DuckDB](https://duckdb.org/) extension to allow duckdb to open
`.bz2` (bzip2) files, and other compressed formats TBD.

See [#12232](https://github.com/duckdb/duckdb/discussions/12232)

Based on [duckdb-gh](https://github.com/carlopi/duckdb-gh) and 
[cozip\_reader](https://github.com/asterisk-labs/cozip_reader),
thanks to Carlo and Cesar for putting their work out there.

## Features

Currently implemented:

| Format | Extension(s) | Reading | Writing | 
|---|---|---|---|
| BZ2 | .bz2 | Y | Y |
| LZMA | .lzma, .xz | N | N |
| LZ4 | .lz4 | N | Y |
| ZSTD | .zst | N | N |

Required library packages (Ubuntu):

* libbz2-dev
* liblzma-dev
* liblz4-dev
* libzstd-dev

## Building

```
git clone --recurse-submodules https://github.com/nickzoic/duckdb-compression.git
cd duckdb-compression
GEN=ninja make -j$(nproc)
```

## Author

[Nick Moore](https://nick.zoic.org/) <nick@zoic.org> 

License: MIT
