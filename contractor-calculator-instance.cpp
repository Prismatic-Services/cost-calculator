// contractor-calculator-instance.cpp : Defines the entry point for the application.
//

#include "file_controller.h"
#include "utils.h"
#include "calculations.h"

#include <iostream>
#include <vector>
#include <string>


int main()
{
    const std::vector<std::string> data = readfilein("../Data/Driver Payment Scale.csv");
    const std::vector<std::string> csv_as_vector = split_on_delimiter(data, ',');

    const std::map<std::string, double> driver_payment_map = csv_to_map(csv_as_vector);
    const double val = driver_payment_calculations("2022-07-21", driver_payment_map);

    std::cout << val << std::endl;

    return 0;
}
