#include <fmt/core.h>
#include <fmt/format.h>
#include <filesystem>
#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include "fsst/main/fsst.h"


namespace fs = std::filesystem;


/**
 * Benchmark loads a file specified via the command line into memory,
 * builds the symbol table, and repeatedly encodes and decodes it.
 *
 * We focus on the time spent on encoding and decoding.
 */


static void Usage_(const char* szProgName) {
    fmt::print("Usage: {} <path-to-data-file>\n", szProgName);
}


int main(int argc, char **argv) {
    if (argc < 2) {
        Usage_(argv[0]);
        return 1;
    }

    fs::path pathDataFile{argv[1]};
    if (!fs::exists(pathDataFile) || !fs::is_regular_file(pathDataFile)) {
        fmt::print("Invalid data file {}\n", pathDataFile.generic_string());
        return 2;
    }

    // load the file into memory
    fmt::print("Loading file ...\n");
    size_t nDataFileSize = fs::file_size(pathDataFile);
    if (nDataFileSize <= 0) {
        fmt::print("I/O failure\n");
        return 3;
    }
    auto bufData = std::make_unique<uint8_t[]>(nDataFileSize);
    int fd = ::open(pathDataFile.c_str(), O_RDONLY);
    if (fd < 0) {
        fmt::print("Failed to open the file {} - {}\n", pathDataFile.generic_string(), strerror(errno));
        return 4;
    }
    if (::read(fd, bufData.get(), nDataFileSize) != nDataFileSize) {
        fmt::print("Failed to read the file - {}\n", strerror(errno));
        return 5;
    }
    ::close(fd);

    // build the symbol table
    fmt::print("Building the symbol table ...\n");
    fsst::SymbolTable st;
    fsst::BuildSymbolTable(st, bufData.get(), std::min((size_t)64*1024, nDataFileSize));

    // encode and decode
    uint8_t *bufCompress = new uint8_t[nDataFileSize*2];
    uint8_t *bufUnpack = new uint8_t[nDataFileSize*2];

    //    warm run
    fmt::print("Warming ...");
    auto nSizeAfterCompress = fsst::Encode(bufData.get(), nDataFileSize, bufCompress, st);
    auto nSizeAfterUnpack = fsst::Decode(bufCompress, nSizeAfterCompress, bufUnpack, st.Get64Symbols(), st.GetLens());
    fmt::print("originalSize={}, compressedSize={}, unpackedSize={}\n",
               nDataFileSize, nSizeAfterCompress, nSizeAfterUnpack);
    if (nDataFileSize != nSizeAfterUnpack) {
        fmt::print("Fatal error\n");
        return 6;
    }

    for (int i = 0; i < 16; i++) {
        fmt::print("Warm round {}\n", i);
        fsst::Encode(bufData.get(), nDataFileSize, bufCompress, st);
        fsst::Decode(bufCompress, nSizeAfterCompress, bufUnpack, st.Get64Symbols(), st.GetLens());
    }

    //    test
    std::chrono::duration<double> durationEncode;
    std::chrono::duration<double> durationDecode;
    for (int i = 0; i < 128; i++) {
        fmt::print("Benchmark round {}\n", i);
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        fsst::Encode(bufData.get(), nDataFileSize, bufCompress, st);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        durationEncode += std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
        fsst::Decode(bufCompress, nSizeAfterCompress, bufUnpack, st.Get64Symbols(), st.GetLens());
        std::chrono::high_resolution_clock::time_point t3 = std::chrono::high_resolution_clock::now();
        durationDecode += std::chrono::duration_cast<std::chrono::duration<double>>(t3 - t2);
    }

    size_t nTotalDataSize = nDataFileSize * 128 / 1024 / 1024;
    fmt::print("Encode throughput {}MB/s, Decode throughput {}MB/s",
               nTotalDataSize / durationEncode.count(), nTotalDataSize / durationDecode.count());
    return 0;
}


