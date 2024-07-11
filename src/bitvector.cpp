#include "bitvector.hpp"
#include <cmath>

uint8_t countOneBits(size_t n) {
    uint8_t count = 0;
    while (n) {
        n &= (n - 1);
        count++;
    }
    return count;
}

Bitvector::Bitvector(std::string bits)
: bitvector(bits.size() / 64 + (bits.size() % 64 == 0 ? 0 :  1)),
  size(bits.size()),
  rankBlockSize(floor(log2(bits.size())/2)),
  rankSuperblockSize(rankBlockSize * rankBlockSize),
  rankSuperblocks(bits.size() / rankSuperblockSize + (bits.size() / rankSuperblockSize == 0 ? 0 : 1)),
  rankBlocks(bits.size() / rankBlockSize + (bits.size() / rankBlockSize == 0 ? 0 : 1)),
  rankLookup(2 << rankBlockSize) {
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

    // Fill rank helper structure.
    /**
     * This could be done in the for loop before if
     * performance in constructor would be important too.
     * Chose readability over performance here.
     *
     * Each block stores the number of ones before the block, so that rank is superblock + current block
     * Therefore the first superblock and block is always zero. Deleting it would require an extra if in rank.
     * Chose to store ones and not zeros because of lookup table ability to get partial patterns.
     */
    size_t superblockOnes = 0;
    for (size_t i = 0; i < rankSuperblocks.size(); ++i) { // for each superblock
        rankSuperblocks[i] = superblockOnes;
        size_t blockOnes = 0;
        // ERROR: For each superblock do blocks. Currently superblock do all blocks...
        for (size_t j = 0; j < rankBlocks.size() && j < rankSuperblockSize/rankBlockSize; ++j) { // for each block within superblock
            rankBlocks[j] = blockOnes;
            for (size_t k = i * rankSuperblockSize + j * rankBlockSize; // for each bit
            k < i * rankSuperblockSize + (j + 1) * rankBlockSize && k < bits.size(); ++k) {
                if (bits[k] == '1') {
                    ++blockOnes;
                    ++superblockOnes;
                }
            }
        }
    }

    // Populate rank lookup table
    /**
     * This can be precomputed. On a 64-bit system only support blocks up to 64 bits length.
     * So in a block there are max 64 ones. Use uint8_t which supports up to 63 bits.
     * If there are really 63 bits we don't need to store it, because it will be stored in block
     */
    for (size_t i = 0; i < rankLookup.size(); ++i) {
        rankLookup[i] = countOneBits(i);
    }
}

bool Bitvector::access(size_t i) {
    uint64_t chunk = bitvector[i/64];
    bool bit = (chunk >> (i%64)) & static_cast<uint64_t>(1);
    return bit;
}

size_t Bitvector::blockLookupOnes(size_t i) {
    return 0;
}

size_t Bitvector::rankOnes(size_t i) {
    auto superblock = i / rankSuperblockSize;
    auto block = i / rankBlockSize;
    auto res = rankSuperblocks[superblock] + rankBlocks[block] + blockLookupOnes(i); // + lookup
    return res;
}

size_t Bitvector::rank(bool bit, size_t i) {
   if(bit) {
       return rankOnes(i);
   } else {
       return i - 1 - rankOnes(i);
   }
}

size_t Bitvector::select(bool bit, size_t i) {
    return 0;
}

size_t Bitvector::getSize() {
    return sizeof(*this) * 8;
}