#ifndef BITVECTOR_BITVECTOR_HPP
#define BITVECTOR_BITVECTOR_HPP

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

class Bitvector {
private:
    struct SelectBlock {
        size_t index;
        std::vector<size_t> blockSelect;
    };
    struct SelectSB {
        size_t index;
        std::vector<size_t> sbSelect;
    };

    /**
      * Get the number of one bits bit before index i
      * @param i The index to begin tracking
      * @return Number of bits of type bit before the index i
      */
    size_t rankOnes(size_t i);

    /**
     * Get the number of ones in a block via lookup table
     * @param i Index in bitvector
     * @return Number of ones in block at position
     */
    size_t blockLookupOnes(size_t i);

    size_t selectBits(size_t i, std::vector<SelectSB>& superblocks);

    void buildSelectStructure(std::vector<SelectSB>& superblocks, char bit, std::string& bits, size_t numberOfBits);
public:
    explicit Bitvector(std::string bits);

    /**
     * Get the size of the bitvector
     * @return Size of bitvector
     */
    size_t getSize() const;

    /**
     * Access the bit a specific index.
     * Undefined behaviour for out-of-range access
     * @param i The index to access
     * @return The bit at index as bool
     */
    bool access(size_t i);

    /**
     * Get the first position where n bits of type bit accumulated.
     * Assumes that there is actually a position. If there is no position
     * with n bits the behaviour is undefined!
     * @param bit What bit to track
     * @param n Amount of bits before position
     * @return The index where n bits are before
     */
    size_t select(bool bit, size_t n);

    /**
     * Get the number of bits bit before index i.
     * Undefined behaviour for invalid indices i!
     * @param bit What bit to track
     * @param i The index to begin tracking
     * @return Number of bits of type bit before the index i
     */
    size_t rank(bool bit, size_t i);

    /**
     * Returns the size of the class
     * @return size in bits
     */
    size_t getSize();

private:
    std::vector<uint64_t> bitvector;       //< Holds bits
    size_t size;                           //< Number of bits in bitvector
    size_t rankBlockSize;                  //< Size of one block
    std::vector<size_t> rankBlocks;        //< Block for rank
    size_t rankSuperblockSize;             //< Size of one superblock
    std::vector<size_t> rankSuperblocks;   //< Superblock for rank
    std::vector<uint8_t> rankLookup;       //< Lookup table for rank blocks
    size_t selectSBsize;                   //< How many bits of one kind are in select superblock
    std::vector<SelectSB> selectZeroSBs;     //< Select superblocks for zeros
    std::vector<SelectSB> selectOneSBs;      //< Select superblocks for ones
};


#endif //BITVECTOR_BITVECTOR_HPP
