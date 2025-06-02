#include "calls.h"
#include <iostream>



int main()
{
    for (const bus_metrics& vect : bus_data()) {
        std::cout << "Name: " << vect.name << "\nDistance: " << vect.distance << "\nTime: " << vect.time_driven << "\n--------------";
    }
    return 0;
}
