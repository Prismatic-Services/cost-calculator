#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


inline std::vector<std::string> readfilein(const std::string& filename) {
	std::vector<std::string> returned;
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

inline void append_line_to_file(const std::string& filename, const std::string& line) {
	if (std::ofstream file(filename, std::ios::app); file.is_open()) {
		file << line << '\n'; // Append the line followed by a newline
		file.close(); // Close the file
	} else {
		// Optionally handle error
		throw std::runtime_error("Failed to open file: " + filename);
	}
}