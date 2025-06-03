#include "calls.h"
#include <iostream>


double cost_per_gallon = 3.131;


int main()
{
    std::map<std::string, double> totals;

    for (const bus_metrics& vect : bus_data()) {
        std::cout << "\nName: " << vect.name << "\nDistance: " << vect.distance << "\nTime: " << vect.time_driven << "\n--------------";

        std::vector<std::string> info_row;
        for (const std::vector<std::string>& line : buses_and_drivers()) {
            if (line[1] == vect.name) info_row = line;
        }

        const double fuel = vect.distance * cost_per_gallon / std::stod(info_row[6]);

    }
    return 0;
}
