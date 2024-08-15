#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "myFunctions.h"
#include "mapping.h"

int main(void) {
    // Initialise struct Truck and Map
    struct Truck trucks[3];
    int numTrucks = 3;
    struct Map baseMap = populateMap();

    // Print header for Project 
    printf("================= \n");
    printf("Seneca Polytechnic Deliveries\n");
    printf("================= \n");

    // Create new trucks for each route
    struct Route routes[3] = { getBlueRoute(), getYellowRoute(), getGreenRoute() };
    char ids[3] = { 'B', 'Y', 'G' };

    // Update Truck struct
    for (int i = 0; i < numTrucks; i++) {
        trucks[i].route = routes[i];
        trucks[i].truckID = ids[i];
        trucks[i].availableWeight = trucks[i].availableVolume = 0;
    }

    // Add routes for each route
    struct Map routeMap = addRoute(&baseMap, &trucks[0].route);
    routeMap = addRoute(&routeMap, &trucks[1].route);
    routeMap = addRoute(&routeMap, &trucks[2].route);

    // Receive Customer information
    getCustomerInfo(trucks, numTrucks, &baseMap);

    return 0;
}