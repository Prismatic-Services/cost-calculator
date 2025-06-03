#pragma once

#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <ranges>
#include <unordered_map>

#include "utils.h"

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

inline double maintenance_calculations(
    const int& row_count,
    const std::map<std::string, double>& payment_scale)
{
    // Calculate the number of full years since the given date

    if (row_count < 0) {
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

                // Check if the calculated row_count fall within the specified range and return the corresponding payment rate
                if (const long upper_bound = std::strtol(end1 + 1, &end2, 10); *end2 == '\0' && row_count >= lower_bound && row_count <= upper_bound) {
                    return rate;
                }
            }

        } else if (key.find('+') != std::string::npos) {
            // Check if the key contains a single year in the correct format and is greater than or equal to the calculated years
            std::string lower_str = key.substr(0, key.find('+'));
            char* end;

            // Check if the calculated years are greater than or equal to the single year and return the corresponding payment rate
            if (const long lower_bound = std::strtol(lower_str.c_str(), &end, 10); *end == '\0' && row_count >= lower_bound) {
                return rate;
            }
        }
    }

    // If no matching key is found in the payment scale, return a default value (0.0)
    return 0.0;
}

inline double pva_calculations(const std::vector<std::vector<std::string>>& table, const std::string& year, const int& row_count) {
    int col_idx = 0;
    int row_idx = 0;

    for (col_idx=0; col_idx < table[0].size(); col_idx++) {
        if (std::to_string(row_count) == table[0][col_idx]) break;
    }

    for (row_idx=0; row_idx < table.size(); row_idx++) {
        if (table[row_idx][0] == year) break;
    }

    return std::stod(table[row_idx][col_idx]);
}


struct bus_metrics {
    std::string name;
    double distance{};
    double time_driven{};
};


// fix this half of this was lowk llama3.1 and a dream
inline std::vector<bus_metrics> bus_metrics_combined(const std::vector<std::vector<std::string>>& data) {
    std::unordered_map<std::string, bus_metrics> metrics_map;
    bus_metrics current;

    for (const std::vector<std::string>& line : data) {
        if (line[0].find("VehicleNumber") != std::string::npos) {
            current = { "Unknown", 0.0, 0.0 };
        } else {
            std::string name = line[0];
            const double time = time_difference_minutes(line[2], line[3]);
            const double dist = std::stod(line[4]);

            auto& entry = metrics_map[name];
            entry.name = name;
            entry.time_driven += time;
            entry.distance += dist;
        }
    }

    // Convert map to vector
    std::vector<bus_metrics> result;
    for (const auto &metrics: metrics_map | std::views::values) {
        result.push_back(metrics);
    }

    return result;
}


inline double management_calculations(const std::map<std::string, double>& scale, const std::string& bus_approval_date, const std::string& category = "") {
        const std::chrono::sys_days date = parse_yyyy_mm_dd(bus_approval_date);

        for (const auto& entry : scale) {
            const std::string& key = entry.first;
            const double& value = entry.second;

            if (key.find('-') != std::string::npos) {
                const std::size_t pos = key.find('-');
                std::string start_str = key.substr(0, pos);
                std::string end_str = key.substr(pos + 1);
                std::chrono::sys_days start_date = parse_ddMMyyyy(start_str);

                if (std::chrono::sys_days end_date = parse_ddMMyyyy(end_str); date >= start_date && date <= end_date) {
                    return value;
                }
            }
            else if (key.starts_with("<")) {
                if (std::chrono::sys_days cutoff_date = parse_ddMMyyyy(key.substr(1)); date < cutoff_date) {
                    return value;
                }
            }
            else if (key.starts_with(">")) {
                if (const std::chrono::sys_days cutoff_date = parse_ddMMyyyy(key.substr(1)); date > cutoff_date) {
                    return value;
                }
            }
            else if (key.starts_with(category + ">")) {
                if (const std::chrono::sys_days cutoff_date = parse_ddMMyyyy(key.substr(category.size() + 1)); date > cutoff_date) {
                    return value;
                }
            }
        }

        return -1.00; // No match
}