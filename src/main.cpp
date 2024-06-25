#include <iostream>
#include <fstream>
#include <string>
#include "bitvector.hpp"

int main(int argc, char* argv[]) {
    // Check for valid input
    if ( argc < 3) {
        printf("Fatal: Not enough arguments!");
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    std::ifstream input(inputFile);

    // Check if the file was opened successfully
    if (!input.is_open()) {
        std::cerr << "Failed to open the file: " << inputFile << std::endl;
        return 1; // Exit with an error code
    }

    // Init vars
    std::string line;
    size_t commands = 0;

    // Get number of commands
    std::getline(input, line);
    commands = atoi(line.data());

    // Init bitvector
    std::getline(input, line);
    Bitvector bitvector(line);

    while (std::getline(input, line) && commands > 0) {
        // Output the line to the console
        std::cout << line << std::endl;
        --commands;
    }
}

