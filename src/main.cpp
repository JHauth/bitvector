#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <sstream>

#include "bitvector.hpp"

#define NAME "joshua_hauth"

int main(int argc, char* argv[]) {
    // Check for valid input
    if ( argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <inputFilename> <outputFilename>" << std::endl;
        return 1;
    }

    std::string inputFile = argv[1];
    std::string outputFile = argv[2];

    std::ifstream input(inputFile);
    std::ofstream output(outputFile);

    // Check if the file was opened successfully
    if (!input.is_open()) {
        std::cerr << "Failed to open the file: " << inputFile << std::endl;
        return 1;
    }

    if (!output.is_open()) {
        std::cerr << "Failed to open the file: " << outputFile << std::endl;
        return 1;
    }

    // Init vars
    std::string line;
    std::string cmdOut;
    std::string fileOut;
    size_t numCommands = 0;
    size_t sizeInBits = 0;
    size_t res = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<double, std::milli> timeInMS{};

    // Get number of commands
    input >> numCommands;
    input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Ignore the rest of the line

    // Init bitvector
    std::getline(input, line);
    Bitvector bitvector(line);

    // Execute commands
    while (numCommands-- > 0 && std::getline(input, line)) {
        cmdOut = "";
        fileOut = "";
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "access") {
            size_t index;
            iss >> index;

            start = std::chrono::high_resolution_clock::now();
            res = bitvector.access(index);
            end = std::chrono::high_resolution_clock::now();
            timeInMS = end - start;
        } else if (command == "rank") {
            size_t type, index;
            iss >> type >> index;

            start = std::chrono::high_resolution_clock::now();
            res = bitvector.rank(type, index);
            end = std::chrono::high_resolution_clock::now();
            timeInMS = end - start;
        } else if (command == "select") {
            size_t type, index;
            iss >> type >> index;

            start = std::chrono::high_resolution_clock::now();
            res = bitvector.select(type, index);
            end = std::chrono::high_resolution_clock::now();
            timeInMS = end - start;
        } else {
            std::cerr << "Unknown command: " << command << std::endl;
            output << "NaN" << std::endl;
            continue;
        }

        // Output result
        printf("%zu name=%s time=%f space=%zu\n", res, NAME, timeInMS.count(), sizeInBits);
        output << res << std::endl;
    }

    // Cleanup
    input.close();
    output.close();

    return 0;
}

