#include "calls.h"
#include "file_controller.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <chrono>

double cost_per_gallon = 3.131;
auto start = std::chrono::steady_clock::now();

int main(const int argc, char* argv[]) {
    std::cout << "Executing" << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_subdir>" << std::endl;
        return 1;
    }

    std::vector<std::map<std::string, double>> totals;

    const std::string base_path = "./storage/" + std::string(argv[1]) + "/";
    const std::string headers = "Bus Number,Fuel Cost,Driver Payment,Maintenance,Management,PVA";
    const std::string output_path = "./calculated/" + std::string(argv[1]) + "output.csv";

    append_line_to_file(output_path, headers);

    for (const bus_metrics& vect : bus_data(base_path)) {
        std::vector<std::string> info_row;
        for (const std::vector<std::string>& line : buses_and_drivers(base_path)) {
            if (line.size() > 1 && line[1] == vect.name) {
                info_row = line;
                break;
            }
        }

        if (info_row.size() < 7) {
            std::cout << "Skipping: Missing or invalid data for " << vect.name << "\n";
            continue;
        }

        try {
            std::map<std::string, double> data;

            const double mpg = std::stod(info_row[6]);
            if (mpg == 0.0) {
                std::cout << "Skipping: MPG is zero for " << vect.name << "\n";
                continue;
            }

            data["bus"] = std::stod(info_row[1]);
            data["fuel"] = vect.distance * cost_per_gallon / mpg;
            data["pva"] = pva(base_path, split_string_on_delimiter(info_row[2], '-')[0], std::stoi(info_row[4]));
            data["management"] = management(base_path, info_row[2]);
            data["maintenance"] = maintenance(base_path, std::stoi(info_row[4])) * vect.distance;
            data["driver"] = driver_payment(base_path, info_row[3]);

            totals.push_back(data);
        } catch (const std::exception& e) {
            std::cout << "Error processing " << vect.name << ": " << e.what() << "\n";
        }
    }

    for (const std::map<std::string, double>& data2 : totals) {
        try {
            std::string line =
                std::to_string(static_cast<int>(data2.at("bus"))) + "," +
                std::to_string(truncateToTwoDecimalPlaces(data2.at("fuel"))) + "," +
                std::to_string(truncateToTwoDecimalPlaces(data2.at("driver"))) + "," +
                std::to_string(truncateToTwoDecimalPlaces(data2.at("maintenance"))) + "," +
                std::to_string(truncateToTwoDecimalPlaces(data2.at("management"))) + "," +
                std::to_string(truncateToTwoDecimalPlaces(data2.at("pva")));

            append_line_to_file(output_path, line);
        } catch (const std::exception& e) {
            std::cout << "Error processing " << e.what() << "\n";
        }
    }

    const auto diff = std::chrono::steady_clock::now() - start;
    std::cout << "Time for execution: " << std::chrono::duration<double, std::milli>(diff).count() << " ms" << std::endl;
    return 0;
}
