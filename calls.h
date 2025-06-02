#pragma once

#include <vector>
#include <string>
#include "utils.h"


#ifndef CALLS_H
#define CALLS_H

inline double driver_payment(const std::string& driver_start_date){
    const std::vector<std::string> driver_payment_scale = readfilein("../Data/Driver Payment Scale.csv");

    const std::vector<std::string> driver_payment_scale_vector = split_on_delimiter(driver_payment_scale, ',');
    const std::map<std::string, double> driver_payment_map = csv_to_map(driver_payment_scale_vector);

    return driver_payment_calculations(driver_start_date, driver_payment_map);
}

#endif //CALLS_H
