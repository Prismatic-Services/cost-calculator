#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <map>
#include <chrono>


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

inline std::vector<std::string> split_string_on_delimiter(const std::string& str, const char delimiter) {
	std::vector<std::string> tokens;

	std::stringstream ss(str);
	std::string token;

	while (getline(ss, token, delimiter)) {
		tokens.push_back(token);
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

inline int time_difference_minutes(const std::string& time1, const std::string& time2) {
	std::tm tm1 = {}, tm2 = {};

	std::istringstream ss1(time1);
	ss1 >> std::get_time(&tm1, "%I:%M %p");

	std::istringstream ss2(time2);
	ss2 >> std::get_time(&tm2, "%I:%M %p");

	// Ensure both time structures have the same date for comparison
	tm1.tm_mday = tm2.tm_mday = 1;
	tm1.tm_mon = tm2.tm_mon = 0;
	tm1.tm_year = tm2.tm_year = 100; // Year 2000

	time_t t1 = mktime(&tm1);
	time_t t2 = mktime(&tm2);

	return static_cast<int>(difftime(t2, t1) / 60);
}

inline std::chrono::sys_days parse_ddMMyyyy(const std::string& date_str) {
	std::istringstream ss(date_str);
	int d, m, y;
	ss >> std::setw(2) >> d >> std::setw(2) >> m >> std::setw(4) >> y;
	return std::chrono::sys_days{
		std::chrono::year{y} /
		std::chrono::month{static_cast<unsigned>(m)} /
		std::chrono::day{static_cast<unsigned>(d)}
	};
}


inline std::chrono::sys_days parse_yyyy_mm_dd(const std::string& date_str) {
	std::istringstream ss(date_str);
	int y, m, d;
	char sep1, sep2;
	ss >> y >> sep1 >> m >> sep2 >> d;
	return std::chrono::sys_days{
		std::chrono::year{y} /
		std::chrono::month{static_cast<unsigned>(m)} /
		std::chrono::day{static_cast<unsigned>(d)}
	};
}
#endif