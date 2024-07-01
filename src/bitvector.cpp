#include "bitvector.hpp"

Bitvector::Bitvector(std::string bits)
: bitvector(bits.size()) {}

bool Bitvector::access(size_t pos) {
    return 0;
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