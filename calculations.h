#pragma once

#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <cstdlib>  // for strtol

// Calculates number of full years since the given date (format: YYYY-MM-DD)
inline int years_since(const std::string& prior_date) {
    std::tm start_tm = {};
    std::istringstream ss(prior_date);
    ss >> std::get_time(&start_tm, "%Y-%m-%d");

    if (ss.fail()) {
        std::cerr << "Failed to parse date: " << prior_date << std::endl;
        return -1;
    }

    const std::time_t now_time = std::time(nullptr);
    const std::tm* now_tm = std::localtime(&now_time);

    int years = now_tm->tm_year + 1900 - start_tm.tm_year;

    if (now_tm->tm_mon < start_tm.tm_mon ||
        (now_tm->tm_mon == start_tm.tm_mon && now_tm->tm_mday < start_tm.tm_mday)) {
        years--;
    }

    return years - 1900;
}

// Calculates payment based on years of experience and scale
inline double driver_payment_calculations(
    const std::string& driver_start_date,
    const std::map<std::string, double>& payment_scale)
{
    const int years = years_since(driver_start_date);
    if (years < 0) {
        return 0.0; // Invalid date or parsing error
    }

    for (const auto& [key, rate] : payment_scale) {
        if (key.find('-') != std::string::npos) {
            // Use strtol for safe conversion
            const char* key_cstr = key.c_str();
            char* end1;
            const long lower = std::strtol(key_cstr, &end1, 10);

            if (*end1 == '-') {
                char* end2;
                if (const long upper = std::strtol(end1 + 1, &end2, 10); *end2 == '\0' && years >= lower && years <= upper) {
                    return rate;
                }
            }

        } else if (key.find('+') != std::string::npos) {
            std::string lower_str = key.substr(0, key.find('+'));
            char* end;
            if (const long lower = std::strtol(lower_str.c_str(), &end, 10); *end == '\0' && years >= lower) {
                return rate;
            }
        }
    }

    return 0.0; // Fallback if no match
}
