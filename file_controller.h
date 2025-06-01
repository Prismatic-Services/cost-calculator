#include <fstream>
#include <iostream>
#include <string>
#include <vector>

inline std::vector<std::string> readfilein(const std::string& filename) {
	std::vector<std::string> returned;
	std::cout << "Hello World" << std::endl;
	std::ifstream inputfile(filename);  // RAII: opens file in constructor

	if (!inputfile) {
		std::cout << "Error opening file: " << filename << std::endl;
		return returned;
	}

	std::string line;
	while (std::getline(inputfile, line)) {
		returned.push_back(std::move(line));  // optional: avoids copy
	}

	return returned;
}