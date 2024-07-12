#include "bitvector.hpp"
#include <cmath>
#include <bitset>
#include <algorithm>

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
  rankBlockSize(static_cast<size_t>(std::max(floor(log2(static_cast<double>(bits.size()))/2), 1.0))), //< type warnings not really important here. Expected to be below 64
  rankSuperblockSize(rankBlockSize * rankBlockSize),
  rankSuperblocks(bits.size() / rankSuperblockSize + (bits.size() % rankSuperblockSize == 0 ? 0 : 1)),
  rankBlocks(bits.size() / rankBlockSize + (bits.size() % rankBlockSize == 0 ? 0 : 1)),
  rankLookup(1 << (rankBlockSize-1)),  //< Needs one less bc otherwise we would just look at the block
  selectSBsize(std::max(log2(bits.size()) * log2(bits.size()), 1.0)),
  selectBlockSize(static_cast<size_t>(sqrt(log2(bits.size())))) {
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

    // Fill rank helper structure. -------------------------------------------------------------------------------- rank
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
    size_t blocksInSuperblock = (rankSuperblockSize/rankBlockSize);
    for (size_t i = 0; i < rankSuperblocks.size(); ++i) { // for each superblock
        rankSuperblocks[i] = superblockOnes;
        size_t blockOnes = 0;
        for (size_t j = 0; i*blocksInSuperblock+j < rankBlocks.size() && j < rankSuperblockSize/rankBlockSize; ++j) { // for each block within superblock
            rankBlocks[i*blocksInSuperblock+j] = blockOnes;
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

    // Fill select helper structures ---------------------------------------------------------------------------- select
    size_t k0 = 0; //< used later for select, counts number of zeros
    size_t k1 = 0; //< used later for select, counts number of ones
    for (auto& c : bits) {
        if (c == '1') ++k1;
        else if (c == '0') ++k0;
    }
    selectOneSBs.resize((k1 / selectSBsize) + (k1 % selectSBsize == 0 ? 0 : 1));
    selectZeroSBs.resize((k0 / selectSBsize) + (k0 % selectSBsize == 0 ? 0 : 1));

    buildSelectStructure(selectOneSBs, '1', bits, k1);
    buildSelectStructure(selectZeroSBs, '0', bits, k0);
}

/**
 * Every superblock has selectSBsize zeros/ones. However the last ones index is the last zero/one in the bitvector, so
 * the last superblocks index might not be to the selectSBsize.
 * Same pattern with blocks within the superblock.
 * This assures that a select is always within some kind of block.
 */
void Bitvector::buildSelectStructure(std::vector<SelectSB> &superblocks, char bit, std::string& bits, size_t numberOfBits) {
    // Fill Select superblocks.
    size_t index = 0;
    size_t count = 0;
    for (size_t i = 0; i < superblocks.size(); ++i) {
        while(count < (i+1)*selectSBsize && count < numberOfBits) {
            if (bits[index] == bit) ++count;
            ++index;
        }
        superblocks[i].index = index-1;
    }

    // Fill structure below select superblocks
    size_t start = 0;
    for (size_t i = 0; i < superblocks.size(); ++i) {
        if (superblocks[i].index - start >= static_cast<size_t>(pow(log2(bits.size()), 4))) {
            // Store answer naively with list
            superblocks[i].sbSelect.resize(selectSBsize);
            for (size_t k, n = 0; n < selectSBsize && start+k < superblocks[i].index; ++k) {
                if (bits[start + k] == bit) {
                    superblocks[i].sbSelect[n] = k;
                    ++n;
                }
            }
        } else {
            // Divide into blocks
            size_t bindex = 0;
            superblocks[i].sbSelect.resize((selectSBsize / selectBlockSize)
                + (selectSBsize % selectBlockSize == 0 ? 0 : 1));
            count = 0;

            for (size_t k = start; k <= superblocks[i].index && count < selectSBsize; ++k) {
                // Track ones/zeros
                if (bits[k] == bit) {
                    ++count;
                }
                // If count is number of ones/zeros in a block assign or it is the last block
                if (count == (bindex+1)*static_cast<size_t>(sqrt(log2(bits.size()))) ||
                    k == superblocks[i].index) {
                    // Assign index to block
                    superblocks[i].sbSelect[bindex] = k;

                    if (k - start >= static_cast<size_t>(log2(bits.size()))) {
                        // Block needs a list
                    } else {
                        // Block uses lookup
                    }
                    ++bindex;
                }
            }
        }
        // Set start for next block
        start = superblocks[i].index + 1;
    }
}

size_t Bitvector::getSize() const {
    return size;
}

bool Bitvector::access(size_t i) {
    uint64_t chunk = bitvector[i/64];
    bool bit = (chunk >> (i%64)) & static_cast<uint64_t>(1);
    return bit;
}

std::string toBinaryString(uint64_t value) {
    return std::bitset<64>(value).to_string();
}

size_t Bitvector::blockLookupOnes(size_t i) {
    --i; // Assume i != 0
    size_t blockStart = ((i / rankBlockSize) * rankBlockSize);
    size_t a = blockStart / 64;
    size_t b = i / 64;
    u_int64_t pattern;

    /**
     * Remember for all following operations in if:
     * The uint64_t blocks are in reversed order,
     * so when I say cut the left side we actually
     * cut the right side!
     */
    if (a == b) {
        pattern = bitvector[a];
        // Cut the left side
        pattern <<= (64 - 1 - (i%64));
        // Cut the right side
        pattern >>= (blockStart % 64 + (64 - 1 - (i%64)));
    } else {
        pattern = bitvector[a];
        uint64_t tmp = bitvector[b];

        // Cut the right side
        pattern >>= blockStart % 64;
        // Cut the left side
        tmp <<= (64 - 1 -(i % 64));
        // Move so tmp overlaps correctly with pattern
        tmp >>= (64 - 1 -(i % 64) - (64 - blockStart % 64));
        // Add the two patterns together
        pattern = pattern | tmp;
    }
    return rankLookup[pattern];
}

size_t Bitvector::rankOnes(size_t i) {
    auto superblock = i / rankSuperblockSize;
    auto block = i / rankBlockSize;
    auto res = rankSuperblocks[superblock] + rankBlocks[block];
    if (i%rankBlockSize != 0) {
        res += blockLookupOnes(i);
    }
    return res;
}

size_t Bitvector::rank(bool bit, size_t i) {
    if (i==0) return 0;
    if(bit) {
       return rankOnes(i);
    } else {
       return i - rankOnes(i);
    }
}

size_t Bitvector::selectBits(size_t i, std::vector<SelectSB> &superblocks) {
    size_t index = 0;
    if (i/selectSBsize != 0) {
        index = superblocks[i/selectSBsize - 1].index;
    }

    size_t superBlockStart = index + 1;
    size_t superblockLen = superblocks[i/selectSBsize].index - superBlockStart;

    if (superblockLen >= static_cast<size_t>(pow(log2(getSize()), 4))) {
        // List
    } else {
        // Superblock is divided into blocks
        size_t bitsLeft = i - index;
        if (bitsLeft / selectBlockSize > 0) {
            index += superblocks[i/selectSBsize].sbSelect[bitsLeft / selectBlockSize - 1];
        }
    }
    return index;
}

size_t Bitvector::select(bool bit, size_t i) {
    if (bit) {
        return selectBits(i, selectOneSBs);
    } else {
        return selectBits(i, selectZeroSBs);
    }
}

size_t Bitvector::getSpace() {
    return sizeof(*this) * 8;
}