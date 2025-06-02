# Contractor Calculator C++ Instance

Converting the Python instance to C++ for performance and efficiency

![Diagram Image](./pris-contractor-calculator-diagram.png)

### What's been done
* \# years since calculations
* Driver payment calculations
* CSV file reading in
* PVA table calculations
* Reading in daily bus route data (formatting is **not** correct)
* Maintenance table calculations

### What's left to do?
* Fuel cost calculations
* Overall calculations
* Totaling of values
* Code cleanup
* AWS deploy?
* Dockerize instances
* Safety checks
* Data output
* Data input (outside of manual feed)


### Data Input
Entrypoint: `/Data`

| Filename                        | Data                                     |
|---------------------------------|------------------------------------------|
| `Driver Payment Scale.csv`      | Driver Hourly Pay Scale per Years Worked |
| `PVA Table.csv`                 | PVA table (row count x approval year)    |
| `Daily Bus Routes.csv`          | Daily bus routes, times, distance, and # |
| `Maintenance Payment Scale.csv` | Cost per mile x # of rows                | 

### CMake File Entrypoint
```c++
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
```

### Calculation Functions
All calculation functions can be found under `/calculations.h` and defined returning a `double` data type
```c++
// ExAMPLE
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
```