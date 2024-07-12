#include <gtest/gtest.h>
#include <cmath>

#include "../src/bitvector.hpp"

std::string generateBitString(const std::string& pattern, size_t totalBits) {
    std::string result;
    size_t patternLength = pattern.size();

    if (patternLength == 0) {
        return result; // Return an empty string if the pattern is empty
    }

    // Append the pattern repeatedly until we reach the desired length
    while (result.size() + patternLength <= totalBits) {
        result += pattern;
    }

    // Append the remaining bits if needed
    size_t remainingBits = totalBits - result.size();
    if (remainingBits > 0) {
        result += pattern.substr(0, remainingBits);
    }

    return result;
}

TEST(GenerateBitString, BasicPattern) {
    std::string pattern = "01";
    size_t totalBits = 5;
    std::string expected = "01010";
    EXPECT_EQ(generateBitString(pattern, totalBits), expected);
}

TEST(GenerateBitString, PatternLargerThanTotalBits) {
    std::string pattern = "110";
    size_t totalBits = 2;
    std::string expected = "11";
    EXPECT_EQ(generateBitString(pattern, totalBits), expected);
}

TEST(GenerateBitString, ExactMultipleOfPattern) {
    std::string pattern = "101";
    size_t totalBits = 9;
    std::string expected = "101101101";
    EXPECT_EQ(generateBitString(pattern, totalBits), expected);
}

TEST(GenerateBitString, EmptyPattern) {
    std::string pattern = "";
    size_t totalBits = 10;
    std::string expected = "";
    EXPECT_EQ(generateBitString(pattern, totalBits), expected);
}

TEST(GenerateBitString, EmptyTotalBits) {
    std::string pattern = "111";
    size_t totalBits = 0;
    std::string expected = "";
    EXPECT_EQ(generateBitString(pattern, totalBits), expected);
}

TEST(GenerateBitString, LargePattern) {
    std::string pattern = "110011001100";
    size_t totalBits = 25;
    std::string expected = "1100110011001100110011001";
    EXPECT_EQ(generateBitString(pattern, totalBits), expected);
}

TEST(GenerateBitString, TotalBitsLessThanPatternLength) {
    std::string pattern = "110011";
    size_t totalBits = 4;
    std::string expected = "1100";
    EXPECT_EQ(generateBitString(pattern, totalBits), expected);
}

// Test the access function of the Bitvector class with basic patterns
TEST(Access, BasicAssertions) {
    // Create a Bitvector with a known pattern
    Bitvector bv("101010");

    // Check the bits at each index
    EXPECT_TRUE(bv.access(0));   // The bit at index 0 is 1
    EXPECT_FALSE(bv.access(1));  // The bit at index 1 is 0
    EXPECT_TRUE(bv.access(2));   // The bit at index 2 is 1
    EXPECT_FALSE(bv.access(3));  // The bit at index 3 is 0
    EXPECT_TRUE(bv.access(4));   // The bit at index 4 is 1
    EXPECT_FALSE(bv.access(5));  // The bit at index 5 is 0
}

// Test the access function of the Bitvector class with various patterns
TEST(Access, VariousPatterns) {
    // Create a Bitvector with a different pattern
    Bitvector bv("110011001100");

    // Check the bits at each index
    EXPECT_TRUE(bv.access(0));   // The bit at index 0 is 1
    EXPECT_TRUE(bv.access(1));   // The bit at index 1 is 1
    EXPECT_FALSE(bv.access(2));  // The bit at index 2 is 0
    EXPECT_FALSE(bv.access(3));  // The bit at index 3 is 0
    EXPECT_TRUE(bv.access(4));   // The bit at index 4 is 1
    EXPECT_TRUE(bv.access(5));   // The bit at index 5 is 1
    EXPECT_FALSE(bv.access(6));  // The bit at index 6 is 0
    EXPECT_FALSE(bv.access(7));  // The bit at index 7 is 0
    EXPECT_TRUE(bv.access(8));   // The bit at index 8 is 1
    EXPECT_TRUE(bv.access(9));   // The bit at index 9 is 1
    EXPECT_FALSE(bv.access(10)); // The bit at index 10 is 0
    EXPECT_FALSE(bv.access(11)); // The bit at index 11 is 0
}

// Test the access function with edge cases
TEST(Access, EdgeCases) {
    // Create a Bitvector with a simple pattern
    Bitvector bv("1");

    // Check the bit at index 0
    EXPECT_TRUE(bv.access(0));   // The bit at index 0 is 1

    // Create a Bitvector with another simple pattern
    Bitvector bv2("0");

    // Check the bit at index 0
    EXPECT_FALSE(bv2.access(0)); // The bit at index 0 is 0
}

// Test the access function of the Bitvector class with more than 64 bits
TEST(Access, LargePatterns) {
    // Create a Bitvector with a pattern longer than 64 bits
    Bitvector bv("11111111111111111111111111111111111111111111111111111111111111110000000000000000000000000000000000000000000000000000000000000000");

    // Check the bits at various indices
    // First 64 bits are all 1s
    for (size_t i = 0; i < 64; ++i) {
        EXPECT_TRUE(bv.access(i));
    }

    // Next 64 bits are all 0s
    for (size_t i = 64; i < 128; ++i) {
        EXPECT_FALSE(bv.access(i));
    }

    // Create another Bitvector with alternating pattern more than 64 bits
    Bitvector bv2("10101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010101010");

    // Check the bits at various indices
    for (size_t i = 0; i < 128; ++i) {
        if (i % 2 == 0) {
            EXPECT_TRUE(bv2.access(i));
        } else {
            EXPECT_FALSE(bv2.access(i));
        }
    }
}

/**
 * Tests the bitvector for sizes where the block size is 0.
 */
TEST(Rank, SmallBitvector) {
    Bitvector bv("1");
    EXPECT_EQ(bv.rank(1,0), 0);
    EXPECT_EQ(bv.rank(0,0), 0);

    Bitvector bv2("11");
    EXPECT_EQ(bv2.rank(1,0), 0);
    EXPECT_EQ(bv2.rank(0,0), 0);
    EXPECT_EQ(bv2.rank(0,1), 0);
    EXPECT_EQ(bv2.rank(1,1), 1);

    Bitvector bv3("111");
    EXPECT_EQ(bv3.rank(1,0), 0);
    EXPECT_EQ(bv3.rank(0,0), 0);
    EXPECT_EQ(bv3.rank(0,1), 0);
    EXPECT_EQ(bv3.rank(1,1), 1);
    EXPECT_EQ(bv3.rank(1,2), 2);
    EXPECT_EQ(bv3.rank(0,2), 0);
}

/**
 * Verifies the scenario where the index i is the first position of a block.
 * The rank calculation should only include the superblock and block values,
 * without needing a lookup.
 */
TEST(Rank, BlockOnly) {
    std::string bits = generateBitString("1", 32); //
    Bitvector bv(bits);

    size_t block = std::max(floor(log2(bits.size())/2), 1.0);

    EXPECT_EQ(bv.rank(1, block), block); // First block
    EXPECT_EQ(bv.rank(1, block*block), block*block);
    EXPECT_EQ(bv.rank(1, block*block+block), block*block+block);
}

/**
 * Tests the rank method when the index i is within a block,
 * and the block is inside a single uint64_t element of the bitvector.
 */
TEST(Rank, BlockWithin64) {}

/**
 * Verifies the rank method when the index i is within a block that
 * spans across two uint64_t elements of the bitvector.
 */
TEST(Rank, BlockBetween64) {}

/**
 * Ensures that the rank method handles the case where the index i is in the last,
 * partially filled block of the bitvector.
 */
TEST(Rank, EndBlock) {}