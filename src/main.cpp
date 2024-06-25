#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char* argv[]) {
    // Check for valid input
    if ( argc < 3) {
        printf("Fatal: Not enough arguments!");
        return 1;
    }

    std::string inputFile = argv[1];
    std::ifstream input(inputFile);

    // Check if the file was opened successfully
    if (!input.is_open()) {
        std::cerr << "Failed to open the file: " << inputFile << std::endl;
        return 1; // Exit with an error code
    }

    // Buffer
    std::string line;

    // Read the file line by line
    while (std::getline(input, line)) {
        // Output the line to the console
        std::cout << line << std::endl;
    }
}

