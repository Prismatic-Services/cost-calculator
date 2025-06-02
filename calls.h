#pragma once

#include <vector>
#include <string>
#include "utils.h"
#include "calculations.h"


#ifndef CALLS_H
#define CALLS_H

inline double driver_payment(const std::string& driver_start_date){
    const std::vector<std::string> driver_payment_scale = readfilein("../Data/Driver Payment Scale.csv");

    const std::vector<std::string> driver_payment_scale_vector = split_on_delimiter(driver_payment_scale, ',');
    const std::map<std::string, double> driver_payment_map = csv_to_map(driver_payment_scale_vector);

    return driver_payment_calculations(driver_start_date, driver_payment_map);
}

// fucked to oblivion
inline std::vector<bus_metrics> bus_data() {
    const std::vector<std::string> daily_bus_route_data = readfilein("../Data/Daily Bus Routes.csv");

    const std::vector<std::string> daily_bus_route_data_vector = split_on_delimiter(daily_bus_route_data, ',');

    const std::vector<bus_metrics> metrics = bus_metrics_combined(daily_bus_route_data_vector);

    for (const auto& m : metrics) {
        std::cout << "Vehicle Name: " << m.name << "\n";
        std::cout << "  Total Distance Driven: " << m.distance << " km\n";
        std::cout << "  Total Time Driven: " << m.time_driven << " minutes\n";
        std::cout << "---------------------\n";
    }

    return metrics;
}


inline double pva(const std::string& year, const int& row_count) {
    const std::vector<std::string> pva_table = readfilein("../Data/PVA Table.csv");

    std::vector<std::vector<std::string>> reformatted_data;

    for (const std::string& line : pva_table) {
        std::vector<std::string> temp_line;
        std::stringstream ss(line);
        std::string token;

        while (getline(ss, token, ',')) {
            temp_line.push_back(token);
        }

        reformatted_data.push_back(temp_line);
    }

    return pva_calculations(reformatted_data, year, row_count);
}

inline double maintenance(const int& row_count) {
    const std::vector<std::string> maintenance_payment_scale = readfilein("../Data/Maintenance Payment Scale.csv");

    const std::vector<std::string> maintenance_payment_scale_vector = split_on_delimiter(maintenance_payment_scale, ',');
    const std::map<std::string, double> maintenance_payment_map = csv_to_map(maintenance_payment_scale_vector);

    return maintenance_calculations(row_count, maintenance_payment_map);
}

#endif //CALLS_H
