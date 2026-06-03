# duckdb-compression

A [DuckDB](https://duckdb.org/) extension to allow duckdb to open
`.bz2` (bzip2) files, and other compressed formats TBD.

With this extension installed, `read_csv` and `read_json` and
`COPY TO ...` etc. will be able to read and write these formats.

See [#12232](https://github.com/duckdb/duckdb/discussions/12232)

Based on [duckdb-gh](https://github.com/carlopi/duckdb-gh) and 
[cozip\_reader](https://github.com/asterisk-labs/cozip_reader),
thanks to Carlo and Cesar for putting their work out there.

## Features

Currently implemented:

| Format | Extension(s) | Reading | Writing | 
|---|---|---|---|
| BZ2 | .bz2 | &check; | &check; |
| LZMA | .lzma, .xz | - | - |
| LZ4 | .lz4 | - | &check; |
| ZSTD | .zst | - | - |

Required library packages (Ubuntu):

* libbz2-dev
* liblzma-dev
* liblz4-dev
* libzstd-dev

## Building

```
sudo apt install libbz2-dev liblzma-dev liblz4-dev libzstd-dev
git clone --recurse-submodules https://github.com/nickzoic/duckdb-compression.git
cd duckdb-compression
GEN=ninja make -j$(nproc)
```

## TODO

At the moment this is more like a proof of concept than "production ready"
so feel free to join in the effort.  For example:

* More compression formats.
* Tests, error handling.
* I only implement `int64_t Read(FileHandle &, void*, int64_t)` not any
  of the other `Read` methods, this is probably bad.
* There's a bunch of other methods I haven't looked at, including `Reset`,
  `OnDiskFile`, etc.
* Reading individual files out of ZIP archives, eg: `/path/zipfile.zip/filename.csv`
* It should be possible to use multiple extensions together to load eg:
  `https://example.com/whatever.csv.bz2` but this would require a different
  approach to opening the files which I don't understand at this time ...
  `RegisterSubSystem`?
  * Is it possible to read something like `zipfile.zip/filename.csv.bz2`, 
  putting a .bz2 inside a .zip is silly but people do do it.
  * Or for triple points, `https:/example.com/path/zipfile.zip/filename.csv.bz2`.

## Author

[Nick Moore](https://nick.zoic.org/) <nick@zoic.org> 

License: MIT
