cmake_minimum_required(VERSION 3.5)

# Set extension name here
set(TARGET_NAME compression)

find_package(BZip2 REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(lz4 REQUIRED IMPORTED_TARGET liblz4)

set(EXTENSION_NAME ${TARGET_NAME}_extension)
set(LOADABLE_EXTENSION_NAME ${TARGET_NAME}_loadable_extension)

project(${TARGET_NAME})
include_directories(src/include ${LIBLZMA_INCLUDE_DIRS})

set(EXTENSION_SOURCES
    src/compression_extension.cpp
    src/compression_filesystem.cpp
    src/bzip_filehandle.cpp)

build_static_extension(${TARGET_NAME} ${EXTENSION_SOURCES})
build_loadable_extension(${TARGET_NAME} " " ${EXTENSION_SOURCES})

target_link_libraries(${EXTENSION_NAME} BZip2::BZip2 LibLZMA::LibLZMA PkgConfig::lz4)
target_link_libraries(${LOADABLE_EXTENSION_NAME} BZip2::BZip2 LibLZMA::LibLZMA PkgConfig::lz4)

install(
  TARGETS ${EXTENSION_NAME}
  EXPORT "${DUCKDB_EXPORT_SET}"
  LIBRARY DESTINATION "${INSTALL_LIB_DIR}"
  ARCHIVE DESTINATION "${INSTALL_LIB_DIR}")
