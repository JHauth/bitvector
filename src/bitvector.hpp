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
     * Access the bit a specific position
     * @param pos The position to access
     * @return The bit at pos as bool
     */
    bool access(size_t pos);

    /**
     * Get the position with i zeros or ones before
     * @param bit What bit to track
     * @param i Amount of bits before position
     * @return The position where i bits are before
     */
    size_t select(bool bit, size_t i);

    /**
     * Get the number of bits bit before position i
     * @param bit What bit to track
     * @param i The position to begin tracking
     * @return Number of bits of type bit before the position i
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
