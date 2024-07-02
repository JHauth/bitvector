#include <gtest/gtest.h>

#include "../src/bitvector.hpp"

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