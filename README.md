# duckdb-compression

A [DuckDB](https://duckdb.org/) extension to allow `read_csv` to open
`.bz2` (bzip2) and `.xz` (LZMA) compressed files.

See [#12232](https://github.com/duckdb/duckdb/discussions/12232)

Based on [duckdb-gh](https://github.com/carlopi/duckdb-gh) and 
[cozip\_reader](https://github.com/asterisk-labs/cozip_reader),
thanks to Carlo and Cesar for putting their work out there.

## Building

```
git clone --recurse-submodules https://github.com/nickzoic/duckdb-compression.git
cd duckdb-compression
GEN=ninja make -j$(nproc)
```
