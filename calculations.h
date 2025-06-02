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
/**
 * Calculates driver payment based on their years of experience.
 *
 * @param driver_start_date The date when the driver started working (format: YYYY-MM-DD).
 * @param payment_scale A map containing the payment scales for different years of experience.
 *                       Each key in the map should be a string representing the year range
 *                       or a single year, and each value is the corresponding payment rate.
 *
 * @return The calculated driver payment based on their years of experience and the provided payment scale.
 */
inline double driver_payment_calculations(
    const std::string& driver_start_date,
    const std::map<std::string, double>& payment_scale)
{
    // Calculate the number of full years since the given date
    const int years = years_since(driver_start_date);

    if (years < 0) {
        return 0.0; // Invalid date or parsing error
    }

    for (const auto& [key, rate] : payment_scale) {
        // Check if the key contains a range (e.g., "5-10") and is in the correct format
        if (key.find('-') != std::string::npos) {
            // Use strtol to safely convert the lower bound from string to long int
            const char* key_cstr = key.c_str();
            char* end1;
            const long lower_bound = std::strtol(key_cstr, &end1, 10);

            if (*end1 == '-') {
                char* end2;
                // Use strtol to safely convert the upper bound from string to long int
                const long upper_bound = std::strtol(end1 + 1, &end2, 10);

                // Check if the calculated years fall within the specified range and return the corresponding payment rate
                if (*end2 == '\0' && years >= lower_bound && years <= upper_bound) {
                    return rate;
                }
            }

        } else if (key.find('+') != std::string::npos) {
            // Check if the key contains a single year in the correct format and is greater than or equal to the calculated years
            std::string lower_str = key.substr(0, key.find('+'));
            char* end;

            // Check if the calculated years are greater than or equal to the single year and return the corresponding payment rate
            if (const long lower_bound = std::strtol(lower_str.c_str(), &end, 10); *end == '\0' && years >= lower_bound) {
                return rate;
            }
        }
    }

    // If no matching key is found in the payment scale, return a default value (0.0)
    return 0.0;
}

inline double pva_calculations(const std::string& year, const int& row_count) {
    int col_idx;



    return 1.00;
}


struct bus_metrics {
    std::string name;
    double distance;
    double time_driven;
};

//inline std::vector<bus_metrics> bus_metrics_combined() {

//}