#define _CRT_SECURE_NO_WARNINGS
#include "myFunctions.h"
#include "myHeader.h"
#include "mapping.h"
#include <math.h>
#include <float.h> // For DBL_MAX
#include "stdio.h"
#include <string.h>

//****// FUNCTIONS IMPLEMENTATION //****//

/**
 * Assigns a shipment to the best available truck, considering load, size, weight, and route.
 */
int assignShipmentToTruck(const struct Map* deliveryMap, struct Truck* fleet, int totalTrucks, struct PackageInfo* shipmentDetails) {
    int bestTruckIndex = -1;
    double shortestDistance = DBL_MAX;
    double dist;

    // Iterate through all trucks to find the best one
    for (int i = 0; i < totalTrucks; i++) {
        if (isTruckCapacitySufficient(&fleet[i], shipmentDetails)) {
            // Calculate the distance from the truck's last delivery point to the shipment's destination
            if (fleet[i].packageCount > 0) {
                dist = distance(&fleet[i].packages[fleet[i].packageCount - 1]->dest, &shipmentDetails->dest);
            }

            // Find the truck with the shortest distance to the shipment's destination
            if (dist < shortestDistance) {
                shortestDistance = dist;
                bestTruckIndex = i;
            }
        }

        if ((shipmentDetails[i].weight < fleet[i].availableWeight) && (shipmentDetails[i].size <= fleet[i].availableVolume)) {
            return 1;
        }

    }

    // If a suitable truck is found, update its capacity and add the shipment
    if (bestTruckIndex != -1) {
        updateTruckCapacity(&fleet[bestTruckIndex], shipmentDetails);
        fleet[bestTruckIndex].packages[fleet[bestTruckIndex].packageCount++] = shipmentDetails;
    }

    return bestTruckIndex;
}

/**
 * Checks if the truck has sufficient capacity to hold the shipment.
 */
int isTruckCapacitySufficient(const struct Truck* truck, const struct PackageInfo* shipment) {
    // Check if the truck has enough available weight and volume
    return (truck->availableWeight >= shipment->weight) && (truck->availableVolume >= shipment->size);
}

/**
 * Displays the route that the truck used to deliver the package to its destination.
 */
int displayRouteToDestination(const struct Route* route, const struct Point destination) {
    if (route == NULL) {
        return 0; // Failure
    }

    printf("Route to destination (%d, %d):\n", destination.row, destination.col);
    for (int i = 0; i < route->numPoints; i++) {
        printf("(%d, %d) -> ", route->points[i].row, route->points[i].col);
    }
    printf("End\n");

    return 1; // Success
}

/**
 * Validates the shipment details entered by the user.
 */
int validateShipment(const struct PackageInfo* shipment) {
    // Check if the weight is more than 0 and size is 1, 3, or 5
    if (shipment->weight <= 0) {
        printf("Weight must be greater than 0.\n");
        return 0;
    }
    if (shipment->size != 1 && shipment->size != 3 && shipment->size != 5) {
        printf("Size must be 1, 3, or 5.\n");
        return 0;
    }
    // Check if the destination row is within the valid range
    if (shipment->dest.row < 1 || shipment->dest.row > 25) {
        printf("Row must be between 1 and 25.\n");
        return 0;
    }
    // Check if the destination column is within the valid range (0-25)
    if (shipment->dest.col < 0 || shipment->dest.col > 25) {
        printf("Column must be between 0 and 25.\n");
        return 0;
    }
    return 1;
}

/**
 * Updates the truck's available weight and volume after adding the shipment.
 */
int updateTruckCapacity(struct Truck* truck, struct PackageInfo* shipment) {
    if (truck == NULL || shipment == NULL) {
        return 0; // Failure due to null pointers
    }

    // Highlighted Change: Ensure there's space for more packages
    if (truck->packageCount >= truck->allocatedShipments) {
        return 0; // Failure due to no space in the packages array
    }

    // Update truck's available weight and volume
    truck->availableWeight -= shipment->weight;
    truck->availableVolume -= shipment->size;

    // Check if the package exceeds available weight or volume
    if (shipment->weight > truck->availableWeight || shipment->size > truck->availableVolume) {
        return 0; // Failure: Package exceeds available capacity
    }

    // Highlighted Change: Add the package to the truck's list
    truck->packages[truck->packageCount++] = shipment;

    return 1; // Success
}

/**
 * Finds the nearest available truck to the shipment's pickup location.
 */

int findNearestTruck(const struct Map* map, struct Truck* fleet, int totalTrucks, const struct Point pickupLocation) {
    // Check for null pointers and valid total trucks count
    if (fleet == NULL || totalTrucks <= 0) {
        return -1; // No trucks available
    }

    int nearestTruckIndex = -1;
    double shortestDistance = DBL_MAX;
    double dist;

    // Iterate through all trucks to find the nearest one to the pickup location
    for (int i = 0; i < totalTrucks; i++) {
        // Check if the truck has at least one delivery point
        if (fleet[i].packageCount > 0 && fleet[i].packages != NULL) {
            // Calculate the distance from the truck's last delivery point to the pickup location
            dist = distance(&fleet[i].packages[fleet[i].packageCount - 1]->dest, &pickupLocation);

            // Find the truck with the shortest distance to the pickup location
            if (dist < shortestDistance) {
                shortestDistance = dist;
                nearestTruckIndex = i;
            }
        }
    }
    return nearestTruckIndex;
}

/**
 * Initializes a Truck structure with the provided truck ID and route.
 * Sets the available weight to 2500 kilograms and volume to 100 cubic meters.
 * Initializes other fields such as allocated shipments and package count.
 * Starts with no packages.
 */
int initializeTruck(struct Truck* truck, int truckID, char route) {
    if (truck == NULL || (route != 'B' && route != 'Y' && route != 'G')) {
        return -1; // Invalid arguments
    }

    truck->truckID = truckID;
    truck->availableWeight = 2500.0;
    truck->availableVolume = 100.0;
    truck->allocatedShipments = 0;
    truck->packageCount = 0;

    // Initialize package pointers to NULL
    for (int i = 0; i < truck->packageCount; i++) {
        truck->packages[i] = NULL;
    }

    return 0; // Success
}

/**
 * Initializes a PackageInfo structure by prompting the user to enter package details.
 * Continues to prompt until valid data is provided, as determined by the validateShipment function.
 */
int initializePackage(struct PackageInfo* package, float weight, float size, int destCol, int destRow)
{
    int validInput = 0;

    while (!validInput) {
        printf("Enter package weight: ");
        package->weight = weight;

        package->size = size;

        package->dest.row = destRow;


        package->dest.col = destCol;

        if (validateShipment(package)) {
            return 1;
        }
        else {
            printf("Invalid input. Please try again.\n");
            return 0;
        }
    }
    return 1;
}

//****// NEW FUNCTIONS IMPLEMENTATION //****//


/**
 * Finds the index of the point in the route that is closest to the destination.
 */
int shortestRouteToDestination(const struct Map* map, const struct Route* route, const struct Point dest) {
    int flag = MAX_ROUTE;
    int index = 0;
    struct Route result = { {{0,0}}, 0, DIVERSION };

    // Iterate through each point in the route
    for (int i = 0; i < route->numPoints; i++) {
        result = shortestPath(map, route->points[i], dest);

        // Check if the current route has fewer points and is valid
        if (flag >= result.numPoints && result.numPoints != 0) {
            flag = result.numPoints;
            index = i;
        }
    }

    return index;
}

/**
 * Validates the customer information and updates the current client structure.
 */
int validateCustomerInfo(struct PackageInfo* currentClient) {
    float weight, size;
    int row;
    char col;

    // Prompt the user for shipment details
    printf("Enter shipment weight, box size and destination (0 0 x to stop): ");
    if (scanf("%f %f %d%c", &weight, &size, &row, &col) != 4) {
        if (weight == 0 && size == 0) {
            return -1;  // Exit condition
        }
        printf("Invalid input\n");
        while (getchar() != '\n');  // Clear the input buffer
        return 0;
    }

    // Validate weight
    if (weight <= 0 || weight > MAX_WEIGHT) {
        printf("Invalid weight (must be 1-%d Kg.)\n", MAX_WEIGHT);
        return 0;
    }

    // Validate size
    if (size != 1 && size != 3 && size != 5) {
        printf("Invalid size\n");
        return 0;
    }

    // Validate destination
    if (row < 1 || row > 25 || col < 'A' || col > 'Y') {
        printf("Invalid destination\n");
        return 0;
    }

    // Update current client information
    currentClient->weight = weight;
    currentClient->size = size;
    currentClient->dest.row = row - 1;
    currentClient->dest.col = col - 'A';

    return 1;
}

/**
 * Prints the truck line based on the truck ID.
 */
void printTruckline(int truckID) {
    printf("Ship on %s LINE, ", truckID == 'B' ? "BLUE" : (truckID == 'Y' ? "YELLOW" : "GREEN"));
}

/**
 * Prints the diversion route from the start point to the destination.
 */
void printDiversion(struct Point* startPoint, struct Route* route, struct Point* destination) {
    // Print the start point
    printf(" %d%c,", startPoint->row + 1, startPoint->col + 'A');

    // Print each point in the route
    for (int i = 0; i < route->numPoints; i++) {
        printf(" %d%c,", route->points[i].row + 1, route->points[i].col + 'A');
    }

    // Print the destination point
    printf(" %d%c\n", destination->row + 1, destination->col + 'A');
}

/**
 * Fills the truck with the current client's package.
 */
void fillTruck(struct PackageInfo* currentClient, int truckID, struct Truck* trucks) {
    int index = truckID == 'B' ? 0 : (truckID == 'Y' ? 1 : 2);

    // Update truck's available weight and volume
    trucks[index].availableWeight += currentClient->weight;
    trucks[index].availableVolume += currentClient->size;
}

/**
 * Checks if the current client's package can fit in the truck.
 */
int canfit(struct PackageInfo* currentClient, struct Truck* truck) {
    // Check if the package can fit based on weight and volume
    return ((truck->availableVolume + currentClient->size) <= MAX_VOLUME) &&
        ((truck->availableWeight + currentClient->weight) <= MAX_WEIGHT);
}

/**
 * Calculates the remaining weight capacity of the truck.
 */
float remainingweight(struct Truck* truck) {
    return MAX_WEIGHT - truck->availableWeight;
}

/**
 * Calculates the remaining volume capacity of the truck.
 */
float remainingcapacity(struct Truck* truck) {
    return MAX_VOLUME - truck->availableVolume;
}

/**
 * Determines the limiting factor (weight or volume) for the truck's capacity.
 */
char limitingFactor(struct Truck* truck) {
    float weightPercentage = (truck->availableWeight / MAX_WEIGHT) * 100;
    float volumePercentage = (truck->availableVolume / MAX_VOLUME) * 100;
    return weightPercentage > volumePercentage ? 'W' : 'V';
}

/**
 * Compares the remaining capacities of two trucks based on their limiting factors.
 */
int compareRemaining(struct Truck* truck1, struct Truck* truck2) {
    char factor1 = limitingFactor(truck1);
    char factor2 = limitingFactor(truck2);

    // Compare based on the same limiting factor
    if (factor1 == factor2) {
        if (factor1 == 'W') {
            return remainingweight(truck1) > remainingweight(truck2) ? -1 : 1;
        }
        else {
            return remainingcapacity(truck1) > remainingcapacity(truck2) ? -1 : 1;
        }
    }
    else {
        return factor1 == 'W' ? -1 : 1;
    }
}

/**
 * Checks if the destination point is adjacent to any point in the route.
 */
int checkPoint(struct Route* route, struct Point* destination) {
    // Iterate through each point in the route
    for (int i = 0; i < route->numPoints; i++) {
        if (distance(destination, &route->points[i]) == 1.0) {
            return i;
        }
    }
    return -1;
}

/**
 * Finds the best truck for the current client's shipment based on distance and capacity.
 */
int findbestTruckForShipment(struct PackageInfo* currentClient, struct Truck* trucks, int numTrucks, int* divert, struct Point* startPoint) {
    struct DistanceWithTruck distances[3] = { 0 };
    struct Point startPoints[3] = { 0 };
    struct Map map = populateMap();
    char truckID = 'X';

    // Check each truck for capacity and proximity to destination
    for (int i = 0; i < numTrucks; i++) {
        if (!canfit(currentClient, &trucks[i])) continue;

        // Check if the destination is adjacent to any point in the route
        int pointIndex = checkPoint(&trucks[i].route, &currentClient->dest);
        if (pointIndex != -1) {
            truckID = trucks[i].truckID;
            *divert = 0;
            return truckID;
        }

        // Find the shortest route to the destination
        int shortestIndex = shortestRouteToDestination(&map, &trucks[i].route, currentClient->dest);
        startPoints[i] = trucks[i].route.points[shortestIndex];
        distances[i].distance = distance(&currentClient->dest, &startPoints[i]);
        distances[i].truck = trucks[i];
    }

    // Sort trucks by shortest distance if no direct route is found
    if (truckID == 'X') {
        for (int i = 0; i < numTrucks - 1; i++) {
            for (int j = 0; j < numTrucks - i - 1; j++) {
                if (distances[j].distance > distances[j + 1].distance) {
                    struct DistanceWithTruck temp = distances[j];
                    distances[j] = distances[j + 1];
                    distances[j + 1] = temp;
                }
            }
        }

        // Choose the truck with the shortest diversion distance
        if (distances[0].distance < 1000) {
            *divert = 1;
            truckID = distances[0].truck.truckID;
            *startPoint = startPoints[truckID == 'B' ? 0 : (truckID == 'Y' ? 1 : 2)];
        }
        else {
            printf("Ships tomorrow\n");
        }
    }
    return truckID;
}

/**
 * Processes customer information and assigns the best truck for each shipment.
 */
void getCustomerInfo(struct Truck* trucks, int numTrucks, struct Map* map) {
    int flag = 0;
    int divert = 0;
    char truckID = 'X';
    struct PackageInfo currentClient = { 0 };
    struct Route newRoute = { 0 };
    struct Point startPoint = { 0 };

    // Loop until the user enters the stop condition
    do {
        // Validate customer information and update current client structure
        flag = validateCustomerInfo(&currentClient);

        if (flag == 1) {
            // Find the best truck for the current shipment
            truckID = findbestTruckForShipment(&currentClient, trucks, numTrucks, &divert, &startPoint);

            if (truckID != 'X') {
                // Print the line of the selected truck
                printTruckline(truckID);

                if (divert) {
                    printf("divert:");

                    // Calculate the diversion route
                    newRoute = shortestPath(map, startPoint, currentClient.dest);

                    if (newRoute.numPoints > 0) {
                        // Print the diversion route
                        printDiversion(&startPoint, &newRoute, &currentClient.dest);
                    }
                    else {
                        printf("Error: No valid route found.\n");
                    }
                }
                else {
                    printf("no diversion\n");
                }

                // Fill the truck with the current client's package
                fillTruck(&currentClient, truckID, trucks);
            }
        }
    } while (flag != -1);  // Continue until the user stops

    printf("Thanks for shipping with Seneca Polytechnic!\n");
}
