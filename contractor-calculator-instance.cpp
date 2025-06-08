#include "calls.h"
#include "file_controller.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>

double cost_per_gallon = 3.131;

int main() {
    std::vector<std::map<std::string, double>> totals;

    for (const bus_metrics& vect : bus_data()) {
        std::map<std::string, double> data;

        std::cout << "\nName: " << vect.name
                  << "\nDistance: " << vect.distance
                  << "\nTime: " << vect.time_driven
                  << "\n--------------";

        std::vector<std::string> info_row;
        for (const std::vector<std::string>& line : buses_and_drivers()) {
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

            const double mpg = std::stod(info_row[6]);
            if (mpg == 0.0) {
                std::cout << "Skipping: MPG is zero for " << vect.name << "\n";
                continue;
            }

            data["bus"] = std::stod(info_row[1]);
            data["fuel"] = vect.distance * cost_per_gallon / mpg;
            data["pva"]  = pva(info_row[2], std::stoi(info_row[4]));  // reference issue here
            data["management"] = management(info_row[2]) * vect.distance;
            data["maintenance"] = maintenance(std::stoi(info_row[2]));
            data["driver"] = driver_payment(info_row[3]);

            totals.push_back(data);
        } catch (const std::exception& e) {

            std::cout << "Error processing " << vect.name << ": " << e.what() << "\n";
        }
    }


    for (const std::map<std::string, double>& data2 : totals) {
        try{
        std::string line =
            std::to_string(data2.at("bus")) + "," +
            std::to_string(data2.at("fuel")) + "," +
            std::to_string(data2.at("driver")) + "," +
            std::to_string(data2.at("maintenance")) + "," +
            std::to_string(data2.at("management")) + "," +
            std::to_string(data2.at("pva"));

        std::cout << line << std::endl;

        append_line_to_file("../output.csv", line);
        } catch (const std::exception& e) {
            std::cout << "Error processing " << e.what();
        }
    }

    return 0;
}
