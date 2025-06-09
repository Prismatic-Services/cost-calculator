#pragma once

#include <vector>
#include <string>
#include "utils.h"
#include "calculations.h"
#include "file_controller.h"


#ifndef CALLS_H
#define CALLS_H

inline double driver_payment(const std::string& driver_start_date){
    const std::vector<std::string> driver_payment_scale = readfilein("../Data/Driver Payment Scale.csv");

    const std::vector<std::string> driver_payment_scale_vector = split_on_delimiter(driver_payment_scale, ',');
    const std::map<std::string, double> driver_payment_map = csv_to_map(driver_payment_scale_vector);

    return driver_payment_calculations(driver_start_date, driver_payment_map);
}


inline std::vector<bus_metrics> bus_data() {
    const std::vector<std::string> daily_bus_route_data = readfilein("../Data/Daily Bus Routes.csv");

    std::vector<std::vector<std::string>> reformatted_data;

    for (const std::string& line : daily_bus_route_data) {
        std::vector<std::string> temp_line;
        std::stringstream ss(line);
        std::string token;

        while (getline(ss, token, ',')) {
            temp_line.push_back(token);
        }

        reformatted_data.push_back(temp_line);
    }

    return bus_metrics_combined(reformatted_data);
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

inline double management(const std::string& bus_approval_date, const std::string& category = "") {
    const std::vector<std::string> management_payment_scale = readfilein("../Data/Management Payment Scale.csv");

    const std::vector<std::string> management_payment_scale_vector = split_on_delimiter(management_payment_scale, ',');
    const std::map<std::string, double> management_payment_map = csv_to_map(management_payment_scale_vector);

    return management_calculations(management_payment_map, bus_approval_date, category);
}

inline std::vector<std::vector<std::string>> buses_and_drivers() {
    const std::vector<std::string> file = readfilein("../Data/Buses and Drivers.csv");

    std::vector<std::vector<std::string>> reformatted_data;

    for (const std::string& line : file) {
        std::vector<std::string> temp_line;
        std::stringstream ss(line);
        std::string token;

        while (getline(ss, token, ',')) {
            temp_line.push_back(token);
        }

        reformatted_data.push_back(temp_line);
    }

    return reformatted_data;
}




#endif //CALLS_H
