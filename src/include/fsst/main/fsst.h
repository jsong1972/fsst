#ifndef FSST_H
#define FSST_H


#include <cstdint>
#include <utility>
#include <array>
#include <string>


namespace fsst {


class SymbolTable {
private:
    size_t nSymbols_;
    std::array<std::string, 512> symbols_;
    int16_t sIndex[256];
    uint64_t *binSymbols_;
    uint8_t *lens_;

public:
    SymbolTable() : nSymbols_{0},
                    symbols_{},
                    binSymbols_{nullptr},
                    lens_{nullptr} {
        for (uint16_t code = 0; code <= 255; code++)
            symbols_[code] = std::string(1, (uint8_t)code);
        ::memset(sIndex, 0, sizeof(sIndex));
    }

    ~SymbolTable() {
        delete [] binSymbols_;
        delete [] lens_;
    }

public:
    void CompressCount(uint16_t count1[512],
                       uint16_t count2[][512],
                       const uint8_t* in,
                       size_t len);
    void AdjustTable(uint16_t count1[512], uint16_t count2[][512]);
    void Seal();

public:
    uint16_t FindLongestSymbol(const uint8_t* in, size_t len);
    uint8_t GetSymbolLen(uint16_t pos);
    uint64_t *Get64Symbols() {
        return binSymbols_;
    }
    uint8_t *GetLens() {
        return lens_;
    }

public:
    void DumpSymbols();

private:
    bool StartsWith_(const uint8_t *in, size_t len, uint16_t subindex);
};


size_t Decode(uint8_t* in,
              size_t len,
              uint8_t* out,
              uint64_t syms[255],
              uint8_t lens[255]);


size_t Encode(uint8_t* in,
              size_t len,
              uint8_t* out,
              SymbolTable& st);


void BuildSymbolTable(SymbolTable& st, const uint8_t* in, size_t len);


} // namespace fsst


#endif
