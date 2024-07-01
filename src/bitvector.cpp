#include "bitvector.hpp"

Bitvector::Bitvector(std::string bits)
: bitvector(bits.size() / 64 + (bits.size() % 64 == 0 ? 0 :  1)) {
    // Fill bitvector uin64 from right to left
    for(size_t i = 0; i < bits.size(); i += 64) {
        uint64_t chunk = 0;
        // Iterate over each bit in the chunk
        for (size_t j = 0; j < 64; ++j) {
            if (i + (63 - j) < bits.size() && bits[i + (63 - j)] == '1') {
                chunk |= (static_cast<uint64_t>(1) << (63 - j));
            }
        }

        bitvector[i / 64] = chunk;
    }
}

bool Bitvector::access(size_t i) {
    uint64_t chunk = bitvector[i/64];
    bool bit = (chunk >> (i%64)) & static_cast<uint64_t>(1);
    return bit;
}

size_t Bitvector::rank(bool bit, size_t i) {
    return 0;
}

size_t Bitvector::select(bool bit, size_t i) {
    return 0;
}

size_t Bitvector::getSize() {
    return sizeof(*this) * 8;
}