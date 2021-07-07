#include <memory>
#include <cstring>
#include <queue>
#include <vector>
#include <algorithm>
#include <set>
#include "fmt/core.h"
#include "fsst/main/fsst.h"


namespace fsst {


size_t Encode(uint8_t* in,
              size_t len,
              uint8_t* out,
              SymbolTable& st) {
    auto oldOut = out;
    auto oldIn = in;
    while (in - oldIn < len) {
        uint16_t pos = st.FindLongestSymbol(in, len);
        if (pos <= 255) {
            *(out++) = 255;
            *(out++) = *(in++);
        } else {
            *(out++) = (uint8_t) pos;
            in += st.GetSymbolLen(pos);
        }
    }

    return out - oldOut;
}


size_t Decode(uint8_t* in,
              size_t len,
              uint8_t* out,
              uint64_t syms[255],
              uint8_t lens[255]) {
    auto oldOut = out;
    for (int i = 0; i < len; i++) {
        uint8_t code = *in++;
        if (code != 255) {
            *((uint64_t *) out) = syms[code];
            out += lens[code];
        } else {
            *out++ = *in++;
        }
    }

    return out - oldOut;
}



void BuildSymbolTable(SymbolTable& st, const uint8_t* in, size_t len) {
    uint16_t count1[512];
    uint16_t count2[512][512];
    for (int i = 0; i < 5; i++) {
        // fmt::print("BuildSymbolTable - iteration {}\n", i);
        ::memset(count1, 0, sizeof(uint16_t) * 512);
        ::memset(count2, 0, sizeof(uint16_t)*512*512);

        st.CompressCount(count1, count2, in, len);
        st.AdjustTable(count1, count2);
    }
    st.Seal();
}


//////////////////////////////////////////////////////////////////////////////
//
//    implementation of SymbolTable
//
/////////////////////////////////////////////////////////////////////////////


/**
 * Compress the sample (in and len) and count the frequencies
 */
void SymbolTable::CompressCount(uint16_t count1[512],
                                uint16_t count2[][512],
                                const uint8_t *in,
                                size_t len) {
    int pos = 0;
    uint16_t prev;
    auto code = FindLongestSymbol(in, len);
    while ((pos += symbols_[code].size()) < len) {
        prev = code;
        code = FindLongestSymbol(&in[pos], len-pos);
        count1[code]++; // count single symbol[code]
        count2[prev][code]++; //
        if (code >= 256) {
            // we also count frequencies for the next byte only
            auto nextByte = in[pos];
            count1[nextByte]++;
            count2[prev][nextByte]++;
        }
    }
}


struct PrioElem {
    std::string symbol;
    uint16_t gain;

    PrioElem(std::string symbol_, uint16_t gain_) : symbol{symbol_}, gain{gain_} {}
};

class Compare {
public:
    bool operator()(PrioElem p1, PrioElem p2) {
        return p1.gain >= p2.gain;
    }
};

void SymbolTable::AdjustTable(uint16_t count1[512], uint16_t count2[][512]) {
    std::set<std::string> pickedSymbols{};
    std::priority_queue<PrioElem, std::vector<PrioElem>, Compare> pq;
    for (uint16_t code1 = 0; code1 < 256+nSymbols_; code1++) {
        auto gain = symbols_[code1].size() * count1[code1];
        if (gain > 0 && pickedSymbols.find(symbols_[code1]) == pickedSymbols.end()) {
            pq.emplace(symbols_[code1], gain);
            pickedSymbols.insert(symbols_[code1]);
        }
        for (uint16_t code2 = 0; code2 < 256+nSymbols_; code2++) {
            auto s = (symbols_[code1] + symbols_[code2]).substr(0, 8);
            if (pickedSymbols.find(s) == pickedSymbols.end()) {
                gain = s.size() * count2[code1][code2];
                if (gain > 0) {
                    pq.emplace(s, gain);
                    pickedSymbols.insert(s);
                }
            }
        }
    }

    for (nSymbols_ = 0; nSymbols_ < 256 && !pq.empty(); nSymbols_++) {
        symbols_[256+nSymbols_] = pq.top().symbol;
        // fmt::print("add a symbol. idx={}, symbol={}, gain={}\n", 256+nSymbols_,
        //           pq.top().symbol, pq.top().gain);
        pq.pop();
    }

    if (nSymbols_ > 0)
        std::sort(symbols_.begin()+256, symbols_.begin()+256+nSymbols_,
                  std::greater<std::string>());

    // DumpSymbols();
}


uint16_t SymbolTable::FindLongestSymbol(const uint8_t *in, size_t len) {
    uint8_t letter = in[0];
    for (uint16_t i = 256; i < 256+nSymbols_; i++) {
        if (StartsWith_(in, len, i)) return i;
    }

    return letter;
}


uint8_t SymbolTable::GetSymbolLen(uint16_t pos) {
    return symbols_[pos].size();
}

/**
 * Checks if "in" starts with symbols_[subindex]
 */
bool SymbolTable::StartsWith_(const uint8_t *in, size_t len, uint16_t subindex) {
    size_t l = symbols_[subindex].size();
    return len >= l && (::memcmp(symbols_[subindex].data(), in, l)==0);
}


void SymbolTable::DumpSymbols() {
    for (uint16_t i = 256; i < 256+nSymbols_; i++) {
        fmt::print("{}\t\t{} \"{}\"\n", i, symbols_[i].size(), symbols_[i]);
    }
}


void SymbolTable::Seal() {
    binSymbols_ = new uint64_t[nSymbols_];
    lens_ = new uint8_t[nSymbols_];

    for (size_t i = 0; i < nSymbols_; i++) {
        binSymbols_[i] = *(uint64_t *)symbols_[256+i].data();
        lens_[i] = (uint8_t)symbols_[256+i].size();
    }
}


} // namespace fsst

