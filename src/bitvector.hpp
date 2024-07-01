#ifndef BITVECTOR_BITVECTOR_HPP
#define BITVECTOR_BITVECTOR_HPP

#include <cstddef>
#include <cstdint>
#include <vector>
#include <string>

class Bitvector {

public:
    explicit Bitvector(std::string bits);

    /**
     * Access the bit a specific index
     * @param i The index to access
     * @return The bit at index as bool
     */
    bool access(size_t i);

    /**
     * Get the index with n zeros or ones before
     * @param bit What bit to track
     * @param n Amount of bits before position
     * @return The index where n bits are before
     */
    size_t select(bool bit, size_t n);

    /**
     * Get the number of bits bit before index i
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
    std::vector<uint64_t> bitvector;
};


#endif //BITVECTOR_BITVECTOR_HPP
