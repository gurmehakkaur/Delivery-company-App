/*
*           SFT PROJECT - MILESTONE 5
*           SECTION: ZBB
*           GROUP: D
*           TEAM MEMBERS:
*           ARSHNOOR KAUR, GURMEHAK KAUR UPPAL, AJAYPARTAP SINGH MAAN, MANAS GANDOTRA
*/

#define __DBL_MAX_ 0.0

#ifndef MYFUNCTIONS_H
#define MYFUNCTIONS_H
#include "myHeader.h"
#include "mapping.h"
#define MAX_WEIGHT 2500
#define MAX_VOLUME 50

//****// FUNCTIONS //****//

/**
 * Assigns a shipment to the best available truck, considering load, size, weight, and route.
 *
 * @param deliveryMap - the map of the delivery area with buildings on it.
 * @param fleet - an array of trucks including the route for each truck.
 * @param totalTrucks - the number of trucks in the array.
 * @param shipmentDetails - the shipment details including size and weight.
 * @returns - an integer representing the index of the truck in the array. If no truck
 *            can take the shipment, -1 is returned.
 */
int assignShipmentToTruck(const struct Map* deliveryMap, struct Truck* fleet, int totalTrucks, struct PackageInfo* shipmentDetails);

/**
 * Checks if the truck has sufficient capacity to hold the shipment.
 *
 * @param truck - the truck being checked.
 * @param shipment - the shipment details including size and weight.
 * @returns - an integer indicating if the truck can hold the shipment (1 if yes, 0 if no).
 */
int isTruckCapacitySufficient(const struct Truck* truck, const struct PackageInfo* shipmentDetails);

/**
 * Displays the route that the truck used to deliver the package to its destination.
 *
 * @param route - the route followed by the truck.
 * @param destination - the delivery destination.
 * @return 1 if the route is displayed successfully, 0 if there is an error.
 */
int displayRouteToDestination(const struct Route* route, const struct Point destination);

/**
 * Validates the shipment details entered by the user.
 *
 * @param shipment - the shipment details including size and weight.
 * @returns - an integer indicating if the shipment is valid (1 if valid, 0 if invalid).
 */
int validateShipment(const struct PackageInfo* shipment);

/**
 * Updates the truck's available weight and volume after adding the shipment.
 *
 * @param truck - the truck being updated.
 * @param shipment - the shipment details including size and weight.
 * @return 1 if the truck's capacity is updated successfully, 0 if there is an error.
 */
int updateTruckCapacity(struct Truck* truck, struct PackageInfo* shipment);

/**
 * Finds the nearest available truck to the shipment's pickup location.
 *
 * @param map - the map of the delivery area with buildings on it.
 * @param fleet - an array of trucks including the route for each truck.
 * @param totalTrucks - the number of trucks in the array.
 * @param pickupLocation - the location where the shipment needs to be picked up.
 * @returns - an integer representing the index of the nearest truck in the array. If no truck is available, -1 is returned.
 */
int findNearestTruck(const struct Map* map, struct Truck* fleet, int totalTrucks, const struct Point pickupLocation);

/**
 * Initializes a Truck with default values.
 *
 * @param truck - pointer to the Truck structure to be initialized.
 * @param truckID - The unique ID for the truck.
 * @param route - The route character for the truck.
 * @return 1 if the truck is initialized successfully, 0 if there is an error.
 */
int initializeTruck(struct Truck* truck, int truckID, char route);

/**
 * Initializes a PackageInfo structure with user input.
 *
 * @param package - pointer to the PackageInfo structure to be initialized.
 * @return 1 if the package is initialized successfully, 0 if there is an error.
 */
int initializePackage(struct PackageInfo* package, float weight, float size, int destCol, int destRow);

//****// NEW FUNCTIONS //****//

/**
 * Finds the shortest route to the destination on the map.
 * @param map - pointer to the Map structure representing the map
 * @param route - pointer to the Route structure representing the route
 * @param dest - Point structure representing the destination point
 * @returns the index of the route point that provides the shortest path to the destination
 */
int shortestRouteToDestination(const struct Map* map, const struct Route* route, const struct Point dest);
/**
 * Retrieves information about the customer and updates the truck and map data.
 * @param trucks - pointer to the array of Truck structures
 * @param numTrucks - number of trucks
 * @param map - pointer to the Map structure representing the map
 */
void getCustomerInfo(struct Truck* trucks, int numTrucks, struct Map* map);

/**
 * Finds the best truck for a given shipment based on various criteria.
 * @param currentClient - pointer to the PackageInfo structure representing the current client
 * @param trucks - pointer to the array of Truck structures
 * @param numTrucks - number of trucks
 * @param divert - pointer to an integer representing the diversion status
 * @param startPoint - pointer to the Point structure representing the starting point
 * @returns the index of the best truck for the shipment
 */
int findbestTruckForShipment(struct PackageInfo* currentClient, struct Truck* trucks, int numTrucks, int* divert, struct Point* startPoint);

/**
 * Validates the information of the current customer.
 * @param currentClient - pointer to the PackageInfo structure representing the current client
 * @returns 1 if the information is valid, 0 otherwise
 */
int validateCustomerInfo(struct PackageInfo* currentClient);

/**
 * Prints information about a specific truck line.
 * @param truckID - integer representing the ID of the truck
 */
void printTruckline(int truckID);

/**
 * Prints diversion information for a given route.
 * @param startPoint - pointer to the Point structure representing the starting point
 * @param route - pointer to the Route structure representing the route
 * @param destination - pointer to the Point structure representing the destination
 */
void printDiversion(struct Point* startPoint, struct Route* route, struct Point* destination);

/**
 * Fills a truck with the current client's package.
 * @param currentClient - pointer to the PackageInfo structure representing the current client
 * @param truckID - integer representing the ID of the truck
 * @param trucks - pointer to the array of Truck structures
 */
void fillTruck(struct PackageInfo* currentClient, int truckID, struct Truck* trucks);

/**
 * Checks if a package can fit in a specific truck.
 * @param currentClient - pointer to the PackageInfo structure representing the current client
 * @param truck - pointer to the Truck structure
 * @returns 1 if the package can fit, 0 otherwise
 */
int canfit(struct PackageInfo* currentClient, struct Truck* truck);

/**
 * Calculates the remaining weight capacity of a truck.
 * @param truck - pointer to the Truck structure
 * @returns the remaining weight capacity as a float
 */
float remainingweight(struct Truck* truck);

/**
 * Calculates the remaining volume capacity of a truck.
 * @param truck - pointer to the Truck structure
 * @returns the remaining volume capacity as a float
 */
float remainingcapacity(struct Truck* truck);

/**
 * Determines the limiting factor of a truck's capacity (weight or volume).
 * @param truck - pointer to the Truck structure
 * @returns a character indicating the limiting factor ('W' for weight, 'V' for volume)
 */
char limitingFactor(struct Truck* truck);

/**
 * Compares the remaining capacities of two trucks.
 * @param truck1 - pointer to the first Truck structure
 * @param truck2 - pointer to the second Truck structure
 * @returns a positive value if truck1 has more remaining capacity, a negative value if truck2 has more, or 0 if they are equal
 */
int compareRemaining(struct Truck* truck1, struct Truck* truck2);

/**
 * Checks if a destination point is part of a given route.
 * @param route - pointer to the Route structure representing the route
 * @param destination - pointer to the Point structure representing the destination
 * @returns 1 if the destination is part of the route, 0 otherwise
 */
int checkPoint(struct Route* route, struct Point* destination);


#endif
