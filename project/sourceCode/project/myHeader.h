#ifndef MYHEADER_H
#define MYHEADER_H
#include "mapping.h"
#define MAP_ROWS 25
#define MAP_COLS 25
#define MAX_ROUTE 100
#define BLUE 2
#define GREEN 4
#define YELLOW 8
#define DIVERSION 16

/**
* A PackageInfo is a detailed information of each box/package in the truck
*/
struct PackageInfo {
    // Weight of the package
    double weight;

    // Size of the package
    double size;

    // Destination point of the package
    struct Point dest;
};

/**
* A Truck contains every details regarding a Truck, its ID, weight and destination.
*/
struct Truck
{
    // A unique ID for a truck
    int truckID;

    // Available weight capacity: 2500 kgs
    float availableWeight;

    // Available volume capacity: 100 c.mtr 
    float availableVolume;

    // Number of shipments a truck can do
    int allocatedShipments;

    // Pointer which tracks the packages count for a truck
    int packageCount;

    // Struct to specify the truck route ( B for Blue, Y for Yellow, G for Green )
    struct Route route;

    // Stores each package information using dynamic address to an array of structure
    struct PackageInfo** packages;
};

/**
 * DistanceWithTruck holds the distance and the associated truck information.
 */
struct DistanceWithTruck {
    // Distance from a specific point or destination
    double distance;

    // Truck associated with this distance
    struct Truck truck;
};



#endif
