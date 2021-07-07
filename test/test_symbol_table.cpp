#include "gtest/gtest.h"
#include "fsst/main/fsst.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include <filesystem>
#include <fstream>


namespace fs = std::filesystem;


TEST (SymbolTable, Build) {
    std::string data = "http://www.google.ca http://www.whatsapp.com http://www.amazon.in";

    fsst::SymbolTable st;
    fsst::BuildSymbolTable(st, (uint8_t*) data.data(), data.size());

    auto out = std::make_unique<uint8_t[]>(data.size() * 2);
    auto lenAfterEncode = fsst::Encode((uint8_t*)data.data(), data.size(), out.get(), st);
    fmt::print("lenAfterEncode={}\n", lenAfterEncode);

    auto raw = std::make_unique<uint8_t[]>(data.size() + 8);
    auto lenAfterDecode = fsst::Decode(out.get(), lenAfterEncode,
                                       raw.get(), st.Get64Symbols(), st.GetLens());
    fmt::print("lenAfterDecode={}\n", lenAfterDecode);
    auto result = std::string{(char *)raw.get(), lenAfterDecode};
    fmt::print("raw string = {}\n", result);

    ASSERT_EQ(data, result);
}


TEST(SymbolTable, Benchmark) {

}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}