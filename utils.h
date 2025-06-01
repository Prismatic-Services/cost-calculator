#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <map>


inline std::vector<std::string> split_on_delimiter(const std::vector<std::string>& data, const char delimiter) {
	std::vector<std::string> tokens;
	for (const std::string& line : data) {
		std::stringstream ss(line);
		std::string token;

		while (getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}
	}
	
	return tokens;
}


inline std::string current_date() {
	const std::time_t now = std::time(nullptr);

	// convert to local time
	const std::tm* local_time = std::localtime(&now);

	// convert to string
	std::stringstream ss;
	ss << std::put_time(local_time, "%Y-%m-%d");
	std::string time_as_string = ss.str();

	return time_as_string;
}

inline std::map<std::string, double> csv_to_map(const std::vector<std::string>& data) {
	std::map<std::string, double> return_map;

	for (size_t i = 0; i + 1 < data.size(); i += 2) {
		return_map[data[i]] = std::stod(data[i + 1]);
	}

	return return_map;
}

#endif