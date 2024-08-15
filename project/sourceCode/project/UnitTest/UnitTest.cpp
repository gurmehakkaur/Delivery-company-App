#include "pch.h"
#include "UnitTest.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(IntegrationTests)
	{
	public:
		TEST_METHOD(T1)
		{
			// Test data for T1 (Valid shipment and sufficient truck capacity)
			Truck truck = { 1, 1000.0, 10.0, 0, 0, {'B', {}}, nullptr };
			PackageInfo shipment = { 800.0, 5.0, {10, 10} };

			// Validate shipment
			int shipmentValidationResult = validateShipment(&shipment);
			Assert::AreEqual(1, shipmentValidationResult, L"Shipment validation failed");

			// Check truck capacity
			if (shipmentValidationResult == 1) {
				int capacityCheckResult = isTruckCapacitySufficient(&truck, &shipment);
				Assert::AreEqual(1, capacityCheckResult, L"Truck capacity insufficient");
			}
		}

		TEST_METHOD(T2)
		{
			// Test data for T2 (Valid shipment but insufficient truck capacity)
			Truck truck = { 2, 500.0, 5.0, 0, 0, {'Y', {}}, nullptr };
			PackageInfo shipment = { 600.0, 3.0, {1, 1} };

			// Validate shipment
			int shipmentValidationResult = validateShipment(&shipment);
			Assert::AreEqual(1, shipmentValidationResult, L"Shipment validation failed");

			// Check truck capacity
			if (shipmentValidationResult == 1) {
				int capacityCheckResult = isTruckCapacitySufficient(&truck, &shipment);
				Assert::AreEqual(0, capacityCheckResult, L"Truck capacity should be insufficient");
			}
		}

		TEST_METHOD(T3)
		{
			// Test data for T3 (Invalid shipment: zero weight)
			Truck truck = { 3, 2500.0, 100.0, 0, 0, {'G', {}}, nullptr };
			PackageInfo shipment = { 0.0, 3.0, {15, 15} };

			// Validate shipment
			int shipmentValidationResult = validateShipment(&shipment);
			Assert::AreEqual(0, shipmentValidationResult, L"Shipment validation should fail");

			// Check truck capacity only if shipment is valid
			if (shipmentValidationResult == 1) {
				int capacityCheckResult = isTruckCapacitySufficient(&truck, &shipment);
				Assert::AreEqual(1, capacityCheckResult, L"Truck capacity check should be irrelevant");
			}
		}

		TEST_METHOD(T4)
		{
			// Test data for T4 (Invalid shipment: invalid destination row)
			Truck truck = { 4, 1500.0, 20.0, 0, 0, {'B', {}}, nullptr };
			PackageInfo shipment = { 1200.0, 5.0, {30, 10} };

			// Validate shipment
			int shipmentValidationResult = validateShipment(&shipment);
			Assert::AreEqual(0, shipmentValidationResult, L"Shipment validation should fail");

			// Check truck capacity only if shipment is valid
			if (shipmentValidationResult == 1) {
				int capacityCheckResult = isTruckCapacitySufficient(&truck, &shipment);
				Assert::AreEqual(1, capacityCheckResult, L"Truck capacity check should be irrelevant");
			}
		}
		TEST_METHOD(T5)
		{
			// Truck with more remaining weight capacity than volume
			Truck truck1 = { 1, 1500.0, 25.0, 0, 0, {'B', {}}, nullptr }; // 1500 kg weight, 25 m³ volume
			Truck truck2 = { 2, 1200.0, 30.0, 0, 0, {'Y', {}}, nullptr }; // 1200 kg weight, 30 m³ volume

			// Determine limiting factors
			char factor1 = limitingFactor(&truck1);
			char factor2 = limitingFactor(&truck2);

			// Check limiting factor for truck1
			Assert::AreEqual('W', factor1, L"Truck1 limiting factor should be weight");

			// Check limiting factor for truck2
			Assert::AreEqual('V', factor2, L"Truck2 limiting factor should be volume");

			// Compare remaining capacities
			int comparisonResult = compareRemaining(&truck1, &truck2);
			Assert::AreEqual(-1, comparisonResult, L"No comparison");
		}

		TEST_METHOD(T6)
		{
			//Truck with more remaining volume capacity than weight
			Truck truck1 = { 3, 2000.0, 20.0, 0, 0, {'G', {}}, nullptr }; // 2000 kg weight, 20 m³ volume
			Truck truck2 = { 4, 1500.0, 25.0, 0, 0, {'B', {}}, nullptr }; // 1500 kg weight, 25 m³ volume

			// Determine limiting factors
			char factor1 = limitingFactor(&truck1);
			char factor2 = limitingFactor(&truck2);

			// Check limiting factor for truck1
			Assert::AreEqual('W', factor1, L"Truck1 limiting factor should be weight");

			// Check limiting factor for truck2
			Assert::AreEqual('W', factor2, L"Truck2 limiting factor should be weight");

			// Compare remaining capacities
			int comparisonResult = compareRemaining(&truck1, &truck2);
			Assert::AreEqual(1, comparisonResult, L"Truck2 should have more remaining volume capacity than Truck1");
		}

		TEST_METHOD(T7)
		{
			// Both trucks with equal remaining capacities
			Truck truck1 = { 5, 1000.0, 30.0, 0, 0, {'Y', {}}, nullptr }; // 1000 kg weight, 30 m³ volume
			Truck truck2 = { 6, 1000.0, 30.0, 0, 0, {'G', {}}, nullptr }; // 1000 kg weight, 30 m³ volume

			// Determine limiting factors
			char factor1 = limitingFactor(&truck1);
			char factor2 = limitingFactor(&truck2);

			// Check limiting factor for both trucks
			Assert::AreEqual('V', factor1, L"Truck1 limiting factor should be volume");
			Assert::AreEqual('V', factor2, L"Truck2 limiting factor should be volume");

			// Compare remaining capacities
			int comparisonResult = compareRemaining(&truck1, &truck2);
			Assert::AreEqual(1, comparisonResult, L"Same volume");
		}

		TEST_METHOD(T8)
		{
			//One truck with remaining weight capacity being the limiting factor
			Truck truck1 = { 7, 2000.0, 10.0, 0, 0, {'B', {}}, nullptr }; // 2000 kg weight, 10 m³ volume
			Truck truck2 = { 8, 1500.0, 30.0, 0, 0, {'G', {}}, nullptr }; // 1500 kg weight, 30 m³ volume

			// Determine limiting factors
			char factor1 = limitingFactor(&truck1);
			char factor2 = limitingFactor(&truck2);

			// Check limiting factor for truck1
			Assert::AreEqual('W', factor1, L"Truck1 limiting factor should be weight");

			// Check limiting factor for truck2
			Assert::AreEqual('V', factor2, L"Truck2 limiting factor should be volume");

			// Compare remaining capacities
			int comparisonResult = compareRemaining(&truck1, &truck2);
			Assert::AreEqual(-1, comparisonResult, L"No comparison");
		}
		// TEST CASE-3 : INTEGRATING validateShipment() AND assignShipmentToTruck()
		TEST_METHOD(T9)
		{
			// Initialize the map
			Map map = populateMap();

			// Create a valid shipment
			PackageInfo shipment = { 600.0, 20.0, {3, 3} }; // Weight and size within truck capacity

			// Initialize trucks with routes
			Route route1 = { {{1, 1}, {2, 2}, {3, 3}}, 3, 'B' };
			Truck truck1 = { 1, 2500.0f, 100.0f, 0, 0, route1, nullptr };

			Route route2 = { {{4, 4}, {5, 5}}, 2, 'G' };
			Truck truck2 = { 2, 1500.0f, 50.0f, 0, 0, route2, nullptr };

			Truck fleet[] = { truck1, truck2 };
			int totalTrucks = 2;

			// Validate the shipment
			int valid = validateShipment(&shipment);

			// Assign the shipment to a truck if valid
			int truckIndex = -1;
			if (valid) {
				truckIndex = assignShipmentToTruck(&map, fleet, totalTrucks, &shipment);
			}

			// Check the assignment result
			Assert::AreNotEqual(0, truckIndex, L"Truck should have been assigned to the shipment.");

		}

		TEST_METHOD(T10)
		{
			// Initialize the map
			Map map = populateMap();

			// Create a shipment that exceeds weight capacity
			PackageInfo shipment = { 3000.0, 20.0, {6, 6} }; // Weight exceeds truck capacity

			// Initialize trucks with routes
			Route route1 = { {{1, 1}, {2, 2}}, 2, 'B' };
			Truck truck1 = { 1, 2500.0f, 100.0f, 0, 0, route1, nullptr };

			Route route2 = { {{3, 3}, {4, 4}}, 2, 'G' };
			Truck truck2 = { 2, 2000.0f, 80.0f, 0, 0, route2, nullptr };

			Truck fleet[] = { truck1, truck2 };
			int totalTrucks = 2;

			// Validate the shipment
			int valid = validateShipment(&shipment);

			// Attempt to assign the shipment to a truck if valid
			int truckIndex = -1;
			if (valid) {
				truckIndex = assignShipmentToTruck(&map, fleet, totalTrucks, &shipment);
			}

			// Check the assignment result, as weight exceeds the capacity, ans -1
			Assert::AreEqual(-1, truckIndex);
		}

		TEST_METHOD(T11)
		{
			// Initialize the map
			Map map = populateMap();

			// Create a valid shipment
			PackageInfo shipment = { 500.0, 10.0, {2, 2} }; // Within truck capacity

			// Initialize trucks with routes
			Route route1 = { {{1, 1}, {2, 2}}, 2, 'B' };
			Truck truck1 = { 1, 1000.0f, 40.0f, 0, 0, route1, nullptr };

			Route route2 = { {{3, 3}, {4, 4}}, 2, 'G' };
			Truck truck2 = { 2, 2000.0f, 60.0f, 0, 0, route2, nullptr };

			Truck fleet[] = { truck1, truck2 };
			int totalTrucks = 2;

			// Validate the shipment
			int valid = validateShipment(&shipment);

			// Assign the shipment to a truck if valid
			int truckIndex = -1;
			if (valid == 1) {
				truckIndex = assignShipmentToTruck(&map, fleet, totalTrucks, &shipment);
			}

			// Check the assignment result
			Assert::AreNotEqual(0, truckIndex);

		}

		TEST_METHOD(T12)
		{
			// Initialize the map
			Map map = populateMap();

			// Create a shipment that is valid but no truck has enough remaining capacity
			PackageInfo shipment = { 500.0, 20.0, {5, 5} }; // Valid shipment

			// Initialize trucks with routes and already filled to capacity
			Route route1 = { {{1, 1}, {2, 2}}, 2, 'B' };
			Truck truck1 = { 1, 500.0f, 10.0f, 2, 0, route1, nullptr };
			PackageInfo package1 = { 300.0, 10.0, {1, 1} };
			PackageInfo package2 = { 200.0, 20.0, {2, 2} };
			PackageInfo* packageArray1[10] = { &package1, &package2 };
			truck1.packages = packageArray1;

			Route route2 = { {{3, 3}, {4, 4}}, 2, 'G' };
			Truck truck2 = { 2, 1000.0f, 50.0f, 0, 0, route2, nullptr };

			Truck fleet[] = { truck1, truck2 };
			int totalTrucks = 2;

			// Validate the shipment
			int valid = validateShipment(&shipment);

			// Attempt to assign the shipment to a truck if valid
			int truckIndex = -1;
			if (valid) {
				truckIndex = assignShipmentToTruck(&map, fleet, totalTrucks, &shipment);
			}

			// Check the assignment result
			Assert::AreEqual(-1, truckIndex, L"Truck should not be assigned to the shipment due to full capacity.");
		}

		// TEST CASE-4 : INTEGRATING isTruckCapacitySufficient() AND initializeTruck()
		TEST_METHOD(T13)
		{
			// Test Case 1: Truck has just enough capacity for the package

			// Initialize Route for the truck
			Route route1 = { {{1, 1}, {1, 2}, {1, 3}}, 3, 'B' };

			PackageInfo package1 = { 600.0, 3.0, {1, 1} };
			PackageInfo* packageArray[1] = { &package1 };

			// Initialize Truck with the route and package pointer
			Truck truck1 = { 1, 2500.0f, 100.0f, 0, 1, route1, packageArray };

			int res1 = isTruckCapacitySufficient(&truck1, packageArray[0]);
			int res2 = 0;

			if (res1 == 1) {
				// Simulate adding the package to the truck
				truck1.allocatedShipments++;
				truck1.packageCount++;
				truck1.availableWeight -= package1.weight;
				truck1.availableVolume -= package1.size;
				res2 = initializeTruck(&truck1, truck1.truckID, 'B');
			}

			Assert::AreEqual(1, res1);  // Truck should have enough capacity
			Assert::AreEqual(0, res2);  // Truck initialization should succeed
		}

		TEST_METHOD(T14)
		{
			// Test Case 2: Truck does not have enough capacity for the second package

			// Initialize Route for the truck
			Route route2 = { {{2, 1}, {2, 2}, {2, 3}}, 3, 'G' };

			Truck truck2 = { 2, 1000.0f, 50.0f, 1, 0, route2, nullptr };
			PackageInfo package2a = { 600.0f, 30.0f };
			PackageInfo package2b = { 500.0f, 25.0f };

			// Simulate adding the first package
			truck2.allocatedShipments++;
			truck2.packageCount++;
			truck2.availableWeight -= package2a.weight;
			truck2.availableVolume -= package2a.size;

			int res1 = isTruckCapacitySufficient(&truck2, &package2b);
			int res2 = 0;

			if (res1 == 1) {
				// Truck has enough capacity for the second package
				res2 = initializeTruck(&truck2, truck2.truckID, 'G');
			}

			Assert::AreEqual(0, res1);  // Truck should NOT have enough capacity for the second package
		}

		TEST_METHOD(T15)
		{
			// Test Case 3: Truck just barely cannot handle the weight of a new package

			// Initialize Route for the truck
			Route route3 = { {{3, 1}, {3, 2}, {3, 3}}, 3, 'Y' };

			Truck truck3 = { 3, 600.0f, 50.0f, 1, 0, route3, nullptr };
			PackageInfo package3a = { 500.0f, 25.0f };
			PackageInfo package3b = { 200.0f, 20.0f };

			// Simulate adding the first package
			truck3.allocatedShipments++;
			truck3.packageCount++;
			truck3.availableWeight -= package3a.weight;
			truck3.availableVolume -= package3a.size;

			int res1 = isTruckCapacitySufficient(&truck3, &package3b);
			int res2 = 0;

			if (res1 == 1) {
				// Truck has enough capacity for the second package
				res2 = initializeTruck(&truck3, truck3.truckID, 'Y');
			}

			Assert::AreEqual(0, res2);  // Truck should NOT have enough capacity for the second package
		}

		TEST_METHOD(T16)
		{
			// Test Case 4: Truck cannot handle volume but can handle weight

			// Initialize Route for the truck
			Route route4 = { {{4, 1}, {4, 2}, {4, 3}}, 3, 'B' };

			Truck truck4 = { 4, 2000.0f, 30.0f, 1, 0, route4, nullptr };
			PackageInfo package4a = { 800.0f, 20.0f };
			PackageInfo package4b = { 500.0f, 15.0f };

			// Simulate adding the first package
			truck4.allocatedShipments++;
			truck4.packageCount++;
			truck4.availableWeight -= package4a.weight;
			truck4.availableVolume -= package4a.size;

			int res1 = isTruckCapacitySufficient(&truck4, &package4b);
			int res2 = 0;

			if (res1 == 1) {
				// Truck has enough capacity for the second package
				res2 = initializeTruck(&truck4, truck4.truckID, 'B');
			}

			Assert::AreEqual(0, res2);  // Truck should NOT have enough capacity for the second package due to volume
		}

		// TEST CASE-5 : INTEGRATING initializeTruck() AND updateTruckCapacity()

		TEST_METHOD(T17)
		{
			// Test Case: Initialize truck and then update its capacity

			// Initialize the map
			Map map = populateMap();

			// Create packages
			PackageInfo package1 = { 600.0, 10.0, {1, 1} };
			PackageInfo package2 = { 500.0, 15.0, {2, 2} };
			PackageInfo* packageArray[10] = { nullptr };

			// Initialize a truck
			Truck truck1 = { 1, 2500.0f, 100.0f, 0, 0, { { {1, 1}, {2, 2} }, 2, 'B' }, packageArray };

			// Initialize the truck (setting up its initial state)
			int initResult = initializeTruck(&truck1, truck1.truckID, 'B');

			// Verify initialization result
			Assert::AreEqual(0, initResult, L"Truck initialization failed");

			// Assign a package to the truck
			truck1.packageCount++;
			truck1.availableWeight -= package1.weight;
			truck1.availableVolume -= package1.size;

			// Update the truck's capacity
			int res = updateTruckCapacity(&truck1, &package1);

			// Verify truck capacity has been updated correctly
			Assert::AreEqual(1, res);
		}

		TEST_METHOD(T18)
		{
			// Test Case: Truck initialization with no packages and update capacity with no packages

			// Initialize the map
			Map map = populateMap();

			// Create a truck with no initial packages
			PackageInfo* emptyPackages[10] = { nullptr };
			Truck truck2 = { 2, 2500.0f, 100.0f, 0, 0, { { {1, 1}, {3, 3} }, 2, 'G' }, emptyPackages };

			// Initialize the truck
			int initResult = initializeTruck(&truck2, truck2.truckID, 'G');

			// Verify initialization result
			Assert::AreEqual(0, initResult, L"Truck initialization failed");

			// Create a package and assign it
			PackageInfo package2 = { 300.0, 5.0, {4, 4} };
			truck2.packageCount++;
			truck2.availableWeight -= package2.weight;
			truck2.availableVolume -= package2.size;

			// Update the truck's capacity
			int res = updateTruckCapacity(&truck2, &package2);
			Assert::AreEqual(1, res);
		}

		TEST_METHOD(T19)
		{
			// Test Case: Initialize truck with packages and update capacity

			// Initialize the map
			Map map = populateMap();

			// Create packages
			PackageInfo package1 = { 600.0, 10.0, {1, 1} };
			PackageInfo package2 = { 500.0, 20.0, {2, 2} };
			PackageInfo* packageArray[10] = { &package1, &package2 };

			// Initialize a truck with a route
			Truck truck1 = { 3, 2500.0f, 100.0f, 0, 0, { { {1, 1}, {2, 2}, {3, 3} }, 3, 'B' }, packageArray };

			// Initialize the truck
			int initResult = initializeTruck(&truck1, truck1.truckID, 'B');

			// Verify initialization result
			Assert::AreEqual(0, initResult, L"Truck initialization failed");

			// Update the truck's capacity after adding packages
			int resp1 = updateTruckCapacity(&truck1, &package1);
			int resp2 = updateTruckCapacity(&truck1, &package2);

			// Verify that updating capacity after adding packages fails as expected
			Assert::AreEqual(1, resp1, L"Update should succeed for package1");
			Assert::AreEqual(1, resp2, L"Update should succeed for package2");
		}

		TEST_METHOD(T20)
		{
			// Test Case: Initialize truck and try updating its capacity with packages that exceed its available capacity

			// Initialize the map
			Map map = populateMap();

			// Create packages that exceed the truck's capacity
			PackageInfo largePackage1 = { 3000.0, 50.0, {1, 1} }; // Exceeds weight capacity
			PackageInfo largePackage2 = { 150.0, 70.0, {2, 2} };  // Exceeds volume capacity
			PackageInfo* packageArray[10] = { nullptr };

			// Initialize a truck with a route
			Truck truck2 = { 4, 1000.0f, 30.0f, 0, 0, { { {1, 1}, {2, 2}, {3, 3} }, 3, 'G' }, packageArray };

			// Initialize the truck
			int initResult = initializeTruck(&truck2, truck2.truckID, 'G');

			// Verify initialization result
			Assert::AreEqual(0, initResult, L"Truck initialization failed");

			// Attempt to update the truck's capacity with oversized packages
			int resp1 = updateTruckCapacity(&truck2, &largePackage1);
			int resp2 = updateTruckCapacity(&truck2, &largePackage2);

			// Assert that both updates should fail
			Assert::AreEqual(0, resp1, L"Update should fail for package1");
			Assert::AreEqual(0, resp2, L"Update should fail for package2");

		}

	};

	TEST_CLASS(Whitebox)
	{
	public:
		//assignshipmentToTruck
		TEST_METHOD(T1)
		{
			struct Truck truck = { 1, 3000, 120, 0, 0, {0}, nullptr };
			struct PackageInfo package = { 10, 5, {10, 10} };

			int result = isTruckCapacitySufficient(&truck, &package);

			Assert::AreEqual(1, result, L"Expected truck to have sufficient capacity for the package");
		}
		TEST_METHOD(T2)
		{
			struct Truck truck = { 1, 100, 120, 0, 0, {0}, nullptr };
			struct PackageInfo package = { 150, 5, {10, 10} };

			int result = isTruckCapacitySufficient(&truck, &package);

			Assert::AreEqual(0, result, L"Expected truck to lack sufficient weight capacity for the package");
		}

		TEST_METHOD(T3)
		{
			struct Truck truck = { 1, 3000, 20, 0, 0, {0}, nullptr };
			struct PackageInfo package = { 10, 25, {10, 10} };

			int result = isTruckCapacitySufficient(&truck, &package);

			Assert::AreEqual(0, result, L"Expected truck to lack sufficient volume capacity for the package");
		}

		TEST_METHOD(T4)
		{
			struct Truck truck = { 1, 100, 50, 0, 0, {0}, nullptr };
			struct PackageInfo package = { 100, 50, {10, 10} };

			int result = isTruckCapacitySufficient(&truck, &package);

			Assert::AreEqual(1, result, L"Expected truck to have exactly matching capacity for the package");
		}

		//TEST FUNCTION VALIDATESHIPMENT
		TEST_METHOD(T5)
		{
			struct PackageInfo shipment = { 10, 1, {10, 10} };

			int result = validateShipment(&shipment);

			Assert::AreEqual(1, result, L"Expected shipment to be valid");
		}

		TEST_METHOD(T6)
		{
			struct PackageInfo shipment = { 10, 2, {10, 10} };

			int result = validateShipment(&shipment);

			Assert::AreEqual(0, result, L"Expected shipment validation to fail due to invalid size");
		}

		TEST_METHOD(T7)
		{
			struct PackageInfo shipment = { -5, 1, {10, 10} };

			int result = validateShipment(&shipment);

			Assert::AreEqual(0, result, L"Expected shipment validation to fail due to invalid weight");
		}

		TEST_METHOD(T8)
		{
			struct PackageInfo shipment = { 10, 1, {26, 10} };

			int result = validateShipment(&shipment);

			Assert::AreEqual(0, result, L"Expected shipment validation to fail due to invalid destination row");
		}


		//Test function updateTruckCapacity
		 TEST_METHOD(T9_ValidShipment)
 {
     // Existing Truck structure initialization
     struct Truck truck = {
         1,                 // truckID
         1000,              // availableWeight
         500,               // availableVolume
         10,                // allocatedShipments (assuming 10 as an example)
         0,                 // packageCount
         {'B'},             // route, assuming 'B' for Blue
         nullptr            // packages pointer will be initialized below
     };

     // Allocate memory for packages
     truck.packages = (struct PackageInfo**)malloc(truck.allocatedShipments * sizeof(struct PackageInfo*));

     if (truck.packages == nullptr) {
         // Handle allocation failure if necessary
         return; // Exit the test case to prevent further issues
     }

     // Your existing test case code continues here...
     struct PackageInfo shipment = { 100, 50, {10, 10} };
     int result = updateTruckCapacity(&truck, &shipment);

     Assert::AreEqual(1, result, L"Expected successful update");

     // Free the allocated memory after the test
     free(truck.packages);
 }

		TEST_METHOD(T10)
		{
			struct PackageInfo shipment = { 100, 50, {10, 10} };

			int result = updateTruckCapacity(NULL, &shipment);

			Assert::AreEqual(0, result, L"Expected failure due to null truck pointer");
		}

		TEST_METHOD(T11)
		{
			struct Truck truck = { 1, 1000, 500, 0, 0, {0}, nullptr };

			int result = updateTruckCapacity(&truck, NULL);

			Assert::AreEqual(0, result, L"Expected failure due to null shipment pointer");
		}

		 TEST_METHOD(T12)
 {
     struct Truck truck = { 1, 100, 50, 0, 0, {0}, nullptr };
     struct PackageInfo shipment = { 200, 100, {10, 10} };

     int result = updateTruckCapacity(&truck, &shipment);

     Assert::AreEqual(0, result, L"Expected failure as shipment exceeds available capacity");
 }

		// Test function checkPoint
		TEST_METHOD(T13)
		{
			// Arrange
			struct Point points[2] = { {1, 1}, {2, 1} }; // Adjusted to have two points
			struct Route route = { {points[0], points[1]}, 2, BLUE }; // Initializing with two points
			struct Point destination = { 2, 2 };

			// Act
			int result = checkPoint(&route, &destination);

			// Assert
			Assert::AreEqual(1, result, L"Expected point at index 1 to be adjacent."); // Adjusted expected result
		}
		TEST_METHOD(T14)
		{
			// Arrange
			struct Point points[2] = { {1, 1}, {2, 2} };
			struct Route route = { {points[0], points[1]}, 2, BLUE };
			struct Point destination = { 4, 4 };

			// Act
			int result = checkPoint(&route, &destination);

			// Assert
			Assert::AreEqual(-1, result, L"No adjacent point should be found.");
		}
		TEST_METHOD(T15)
		{
			// Arrange
			struct Point points[1] = { {1, 1} };
			struct Route route = { {points[0]}, 1, BLUE };
			struct Point destination = { 1, 2 };

			// Act
			int result = checkPoint(&route, &destination);

			// Assert
			Assert::AreEqual(0, result, L"Expected point at index 0 to be adjacent.");
		}
		TEST_METHOD(T16)
		{
			// Arrange
			struct Point points[MAX_ROUTE];  // Declare an array of Points
			struct Route route = { {0}, 0, BLUE };  // Initialize with zero points
			struct Point destination = { 1, 1 };

			// Act
			int result = checkPoint(&route, &destination);

			// Assert
			Assert::AreEqual(-1, result, L"With no points in the route, no adjacent point should be found.");
		}


		//Test function initialise package
		TEST_METHOD(T17)
		{
			// Test ID: T17
			PackageInfo package;
			int result = initializePackage(&package, 500.0f, 3.0f, 10, 5);
			Assert::AreEqual(1, result);
		}

		TEST_METHOD(T18)
		{
			// Test ID: T18
			PackageInfo package;
			int result = initializePackage(&package, -100.0f, 3.0f, 10, 5);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(T19)
		{
			// Test ID: T19
			PackageInfo package;
			int result = initializePackage(&package, 500.0f, 3.0f, 26, 5);
			Assert::AreEqual(0, result);
		}

		TEST_METHOD(T20)
		{
			// Test ID: T20
			PackageInfo package;
			int result = initializePackage(&package, 500.0f, 3.0f, 25, 25);
			Assert::AreEqual(1, result);
		}

		//Test fucntion canFit
		TEST_METHOD(T21)
		{
			struct PackageInfo package = { 100.0, 10.0, {1, 1} };
			struct Truck truck = { 1, 2400.0, 40.0, 0, 0, {'B'}, NULL };

			int result = canfit(&package, &truck);

			Assert::AreEqual(1, result, L"Expected package to fit within available weight and volume.");
		}

		TEST_METHOD(T22) {
			struct PackageInfo package = { 200.0, 10.0, {1, 1} };
			struct Truck truck = { 1, 2350.0, 40.0, 0, 0, {'B'}, NULL };

			int result = canfit(&package, &truck);

			Assert::AreEqual(0, result, L"Expected package to exceed available weight.");
		}
		// Test case: Package exceeds available volume
		TEST_METHOD(T23) {
			struct PackageInfo package = { 100.0, 20.0, {1, 1} };
			struct Truck truck = { 1, 2400.0, 35.0, 0, 0, {'B'}, NULL };

			int result = canfit(&package, &truck);

			Assert::AreEqual(0, result, L"Expected package to exceed available volume.");
		}

		// Test case: Package exceeds both available weight and volume
		TEST_METHOD(T24) {
			struct PackageInfo package = { 200.0, 15.0, {1, 1} };
			struct Truck truck = { 1, 2350.0, 40.0, 0, 0, {'B'}, NULL };

			int result = canfit(&package, &truck);

			Assert::AreEqual(0, result, L"Expected package to exceed both available weight and volume.");
		}


		//Test function compareRemaining
		TEST_METHOD(T25)
		{
			struct Truck truck1 = { 500, 500, 0, 0, NULL }; // Remaining weight: 2000
			struct Truck truck2 = { 500, 500, 0, 0, NULL }; // Remaining weight: 2000

			Assert::AreEqual(1, compareRemaining(&truck1, &truck2), L"Trucks with equal remaining weight should be considered equal, hence Truck 1 is ranked higher.");
		}
		TEST_METHOD(T26)
		{
			struct Truck truck1 = { 1000, 1500, 0, 0, NULL }; // Remaining weight: 1000
			struct Truck truck2 = { 1000, 2000, 0, 0, NULL }; // Remaining weight: 1500

			Assert::AreEqual(-1, compareRemaining(&truck1, &truck2), L"Truck 1 with more remaining weight capacity should be ranked higher.");
		}
		TEST_METHOD(T27)
		{
			struct Truck truck1 = { 1000, 2000, 0, 0, NULL }; // Remaining weight: 1500
			struct Truck truck2 = { 1000, 1500, 0, 0, NULL }; // Remaining weight: 1000

			Assert::AreEqual(1, compareRemaining(&truck1, &truck2), L"Truck 2 with more remaining weight capacity should be ranked higher.");
		}
		TEST_METHOD(T28)
		{
			struct Truck truck1 = { 1500, 50, 0, 0, NULL }; // Remaining volume: 50
			struct Truck truck2 = { 1500, 75, 0, 0, NULL }; // Remaining volume: 25

			Assert::AreEqual(-1, compareRemaining(&truck1, &truck2), L"Truck 1 with more remaining volume capacity should be ranked higher when volume is the limiting factor.");
		}



		// Tests for limitingFactor function
		 TEST_METHOD(T29_WeightIsLimitingFactor)
 {
     // Setup: Truck with weight limiting factor
     struct Truck truck = {
         1,               // truckID (example value)
           2000,    // availableWeight 
             30,     // availableVolume 
             0,       // allocatedShipments
             0,       // packageCount
             {'B'},   // route
             nullptr  // packages (correctly initialized as nullptr)
         };



         // Action: Determine the limiting factor
         char result = limitingFactor(&truck);

         // Assertion: Expect 'W' since weight is the limiting factor
         Assert::AreEqual('W', result, L"Expected weight to be the limiting factor");
     }
 TEST_METHOD(T30_VolumeIsLimitingFactor)
 {
     // Setup: Truck with weight limiting factor
     struct Truck truck = {
         1,               // truckID (example value)
           2000,    // availableWeight 
             100,     // availableVolume 
             0,       // allocatedShipments
             0,       // packageCount
             {'B'},   // route
             nullptr  // packages (correctly initialized as nullptr)
     };
     char result = limitingFactor(&truck);

     Assert::AreEqual('V', result, L"Expected volume to be the limiting factor");
 }
 TEST_METHOD(T31_)
 {
     // Setup: Truck with weight limiting factor
     struct Truck truck = {
         1,               // truckID (example value)
           1250,    // availableWeight 
             25,     // availableVolume 
             0,       // allocatedShipments
             0,       // packageCount
             {'B'},   // route
             nullptr  // packages (correctly initialized as nullptr)
     };
     char result = limitingFactor(&truck);

     Assert::AreEqual('V', result, L"Expected volume to be the limiting factor when percentages are equal");
 }

 TEST_METHOD(T32)
 {
     // Setup: Truck with weight limiting factor
     struct Truck truck = {
         1,               // truckID (example value)
           2500,    // availableWeight 
             55,     // availableVolume 
             0,       // allocatedShipments
             0,       // packageCount
             {'B'},   // route
             nullptr  // packages (correctly initialized as nullptr)
     };
     char result = limitingFactor(&truck);

     Assert::AreEqual('V', result, L"Expected volume to be the limiting factor when percentages are equal");
 }

	};
	//TEST_CLASS(Blackbox)
	//{
	//public:
	//	TEST_METHOD(T1)
	//	{
	//		Map deliveryMap = { {{0}}, MAP_ROWS, MAP_COLS };

	//		Truck* fleet = new Truck[3];

	//		fleet[0].truckID = 1;
	//		fleet[0].availableWeight = 1000.0;
	//		fleet[0].availableVolume = 10.0;
	//		fleet[0].allocatedShipments = 0;
	//		fleet[0].packageCount = 0;
	//		fleet[0].route = 'B';
	//		fleet[0].packages = new PackageInfo * [100]; // Initialize the packages array

	//		fleet[1].truckID = 2;
	//		fleet[1].availableWeight = 500.0;
	//		fleet[1].availableVolume = 10.0;
	//		fleet[1].allocatedShipments = 0;
	//		fleet[1].packageCount = 0;
	//		fleet[1].route = 'Y';
	//		fleet[1].packages = new PackageInfo * [100]; // Initialize the packages array

	//		PackageInfo shipmentDetails;
	//		shipmentDetails.weight = 1000.0;
	//		shipmentDetails.size = 12.0;
	//		shipmentDetails.dest = { 10, 10 };

	//		int totalTrucks = 2;

	//		int result = assignShipmentToTruck(&deliveryMap, fleet, totalTrucks, &shipmentDetails);

	//		Assert::AreEqual(1, fleet->truckID);

	//		// Clean up
	//		for (int i = 0; i < fleet[0].packageCount; ++i) {
	//			free(fleet[0].packages[i]);
	//		}
	//		free(fleet[0].packages);
	//	}

	//	TEST_METHOD(T2)
	//	{

	//		Map deliveryMap = { {{0}}, MAP_ROWS, MAP_COLS };

	//		Truck* fleet = new Truck[3];

	//		fleet[0].truckID = 1;
	//		fleet[0].availableWeight = 500.0;
	//		fleet[0].availableVolume = 5.0;
	//		fleet[0].allocatedShipments = 0;
	//		fleet[0].packageCount = 0;
	//		fleet[0].route = 'B';
	//		fleet[0].packages = NULL;
	//		fleet[1].truckID = 2;
	//		fleet[1].availableWeight = 600.0;
	//		fleet[1].availableVolume = 6.0;
	//		fleet[1].allocatedShipments = 0;
	//		fleet[1].packageCount = 0;
	//		fleet[1].route = 'Y';
	//		fleet[1].packages = NULL;
	//		fleet[2].truckID = 3;
	//		fleet[2].availableWeight = 700.0;
	//		fleet[2].availableVolume = 7.0;
	//		fleet[2].allocatedShipments = 0;
	//		fleet[2].packageCount = 0;
	//		fleet[2].route = 'G';
	//		fleet[2].packages = NULL;

	//		PackageInfo shipmentDetails;
	//		shipmentDetails.weight = 800.0;
	//		shipmentDetails.size = 8.0;
	//		shipmentDetails.dest = { 10, 10 };

	//		int totalTrucks = 3;

	//		int result = assignShipmentToTruck(&deliveryMap, fleet, totalTrucks, &shipmentDetails);

	//		Assert::AreEqual(-1, result);
	//		// Clean up
	//		for (int i = 0; i < fleet[0].packageCount; ++i) {
	//			free(fleet[0].packages[i]);
	//		}
	//		free(fleet[0].packages);
	//	}

	//	TEST_METHOD(T3)
	//	{
	//		Map deliveryMap = { {{0}}, MAP_ROWS, MAP_COLS };

	//		Truck* fleet = new Truck[3];

	//		fleet[0].truckID = 1;
	//		fleet[0].availableWeight = 1000.0;
	//		fleet[0].availableVolume = 10.0;
	//		fleet[0].allocatedShipments = 0;
	//		fleet[0].packageCount = 0;
	//		fleet[0].route = 'B';
	//		fleet[0].packages = new PackageInfo * [100]; // Initialize the packages array

	//		fleet[1].truckID = 2;
	//		fleet[1].availableWeight = 1000.0;
	//		fleet[1].availableVolume = 10.0;
	//		fleet[1].allocatedShipments = 0;
	//		fleet[1].packageCount = 0;
	//		fleet[1].route = 'Y';
	//		fleet[1].packages = new PackageInfo * [100]; // Initialize the packages array

	//		fleet[2].truckID = 3;
	//		fleet[2].availableWeight = 1000.0;
	//		fleet[2].availableVolume = 10.0;
	//		fleet[2].allocatedShipments = 0;
	//		fleet[2].packageCount = 0;
	//		fleet[2].route = 'Y';
	//		fleet[2].packages = new PackageInfo * [100]; // Initialize the packages array

	//		PackageInfo shipmentDetails;
	//		shipmentDetails.weight = 1000.0;
	//		shipmentDetails.size = 12.0;
	//		shipmentDetails.dest = { 10, 10 };

	//		int totalTrucks = 3;

	//		int result = assignShipmentToTruck(&deliveryMap, fleet, totalTrucks, &shipmentDetails);

	//		// Multiple trucks are available, the system will select the 1st available truck. 
	//		Assert::AreEqual(1, fleet->truckID);

	//		// Clean up
	//		for (int i = 0; i < fleet[0].packageCount; ++i) {
	//			free(fleet[0].packages[i]);
	//		}
	//		free(fleet[0].packages);
	//	}
	//	TEST_METHOD(T4)
	//	{

	//		Map deliveryMap = { {{0}}, MAP_ROWS, MAP_COLS };

	//		Truck* fleet = new Truck[3];

	//		fleet[0].truckID = 1;
	//		fleet[0].availableWeight = 500.0;
	//		fleet[0].availableVolume = 5.0;
	//		fleet[0].allocatedShipments = 0;
	//		fleet[0].packageCount = 0;
	//		fleet[0].route = 'B';
	//		fleet[0].packages = NULL;
	//		fleet[1].truckID = 2;
	//		fleet[1].availableWeight = 600.0;
	//		fleet[1].availableVolume = 6.0;
	//		fleet[1].allocatedShipments = 0;
	//		fleet[1].packageCount = 0;
	//		fleet[1].route = 'Y';
	//		fleet[1].packages = NULL;
	//		fleet[2].truckID = 3;
	//		fleet[2].availableWeight = 700.0;
	//		fleet[2].availableVolume = 7.0;
	//		fleet[2].allocatedShipments = 0;
	//		fleet[2].packageCount = 0;
	//		fleet[2].route = 'G';
	//		fleet[2].packages = NULL;

	//		PackageInfo shipmentDetails;
	//		shipmentDetails.weight = 800.0;
	//		shipmentDetails.size = 8.0;
	//		shipmentDetails.dest = { 10, 10 };

	//		int totalTrucks = 3;

	//		int result = assignShipmentToTruck(&deliveryMap, fleet, totalTrucks, &shipmentDetails);

	//		Assert::AreEqual(-1, result);
	//		// Clean up
	//		for (int i = 0; i < fleet[0].packageCount; ++i) {
	//			free(fleet[0].packages[i]);
	//		}
	//		free(fleet[0].packages);
	//	}
	//	TEST_METHOD(T5)
	//	{
	//		PackageInfo packageDetails = { 500.0, 5.0, {10, 10} };
	//		int result = validateShipment(&packageDetails);
	//		Assert::AreEqual(1, result);
	//	}
	//	TEST_METHOD(T6)
	//	{
	//		PackageInfo packageDetails = { 1.0, 1.0, {10, 10} };
	//		int result = validateShipment(&packageDetails);
	//		Assert::AreEqual(1, result);
	//	}
	//	TEST_METHOD(T7)
	//	{
	//		PackageInfo packageDetails = { 0.0, 0.0, {10, 10} };
	//		int result = validateShipment(&packageDetails);
	//		Assert::AreEqual(0, result);
	//	}
	//	TEST_METHOD(T8)
	//	{
	//		PackageInfo packageDetails = { 500.0, 120.0, {10, 10} };
	//		int result = validateShipment(&packageDetails);
	//		Assert::AreEqual(0, result);
	//	}

	//	TEST_METHOD(T9)
	//	{
	//		struct Point points[MAX_ROUTE];
	//		points[0] = { 0, 0 };
	//		points[1] = { 0, 1 };
	//		points[2] = { 0, 2 };
	//		points[3] = { 1, 2 };
	//		points[4] = { 2, 2 };

	//		struct Route route = { *points, 5 };
	//		struct Point destination = { 2, 2 };

	//		int result = displayRouteToDestination(&route, destination);
	//		Assert::AreEqual(1, result);
	//	}

	//	TEST_METHOD(T10)
	//	{
	//		struct Point points[MAX_ROUTE];
	//		points[0] = { 0, 0 };
	//		points[1] = { 0, 1 };
	//		points[2] = { 0, 2 };
	//		points[3] = { 1, 2 };
	//		points[4] = { 2, 2 };

	//		struct Route route = { *points, 5 };
	//		struct Point destination = { 3, 3 };

	//		int result = displayRouteToDestination(&route, destination);
	//		Assert::AreEqual(1, result);
	//	}

	//	TEST_METHOD(T11)
	//	{
	//		struct Point points[MAX_ROUTE];
	//		points[0] = { 5,5 };
	//		struct Route route = { *points, 1 };
	//		struct Point destination = { 5, 5 };

	//		int result = displayRouteToDestination(&route, destination);
	//		Assert::AreEqual(1, result);
	//	}
	//	TEST_METHOD(T12)
	//	{
	//		struct Point points[MAX_ROUTE];
	//		points[0] = { 0, 0 };
	//		points[1] = { 0, 1 };
	//		points[2] = { 0, 2 };
	//		points[3] = { 1, 2 };
	//		points[4] = { 2, 2 };
	//		struct Route route = { *points, 5 };
	//		struct Point destination = { 3, 3 };

	//		int result = displayRouteToDestination(&route, destination);
	//		Assert::AreEqual(1, result);
	//	}

	//	TEST_METHOD(T13)
	//	{
	//		Truck truck = { 123, 2000.0, 80.0,10, 0, 'B',{NULL} };
	//		PackageInfo shipment = { 500.0, 10.0, {10,5} };
	//		int result = isTruckCapacitySufficient(&truck, &shipment);
	//		Assert::AreEqual(1, result);
	//	}
	//	TEST_METHOD(T14)
	//	{
	//		Truck truck = { 123, 500.0, 10.0,10, 0, 'B',{NULL} };
	//		PackageInfo shipment = { 500.0, 10.0, {10,5} };
	//		int result = isTruckCapacitySufficient(&truck, &shipment);
	//		Assert::AreEqual(1, result);
	//	}
	//	TEST_METHOD(T15)
	//	{
	//		Truck truck = { 123, 400.0, 50.0,10, 0, 'B',{NULL} };
	//		PackageInfo shipment = { 450.0, 5.0, {10,5} };
	//		int result = isTruckCapacitySufficient(&truck, &shipment);
	//		Assert::AreEqual(0, result);
	//	}
	//	TEST_METHOD(T16)
	//	{
	//		Truck truck = { 123, 1000.0, 5.0,10, 0, 'B',{NULL} };
	//		PackageInfo shipment = { 200.0, 6.0, {10,5} };
	//		int result = isTruckCapacitySufficient(&truck, &shipment);
	//		Assert::AreEqual(0, result);
	//	}
	//	TEST_METHOD(T17)
	//	{
	//		struct Truck truck = { 1, 2500.0, 100.0, 0, 0, 'A' };
	//		struct PackageInfo shipment = { 500.0, 20.0, { 5, 5 } };

	//		int result = updateTruckCapacity(&truck, &shipment);
	//		Assert::AreEqual(1, result); // Success

	//	}

	//	// Test Case 2: Shipment size and weight equal to trucks remaining size and weight
	//	TEST_METHOD(T18)
	//	{
	//		struct Truck truck = { 2, 500.0, 20.0, 0, 0, 'B' };
	//		struct PackageInfo shipment = { 500.0, 20.0, { 5, 5 } };

	//		int result = updateTruckCapacity(&truck, &shipment);
	//		Assert::AreEqual(1, result); // Success

	//	}
	//	// Test Case 3: Shipment exceeds weight capacity
	//	TEST_METHOD(T19)
	//	{
	//		struct Truck truck = { 3, 2500.0, 20.0, 0, 0, 'C' };
	//		struct PackageInfo shipment = { 3000.0, 10.0, { 5, 5 } };

	//		int result = updateTruckCapacity(&truck, &shipment);
	//		Assert::AreEqual(1, result); // Success, but truck will have negative available weight

	//	}

	//	// Test Case 4: Case in which truck is NULL
	//	TEST_METHOD(T20)
	//	{
	//		struct Truck truck = { 1000.0, 50.0 }; // Initializing truck with some values
	//		struct PackageInfo shipment = { 500.0, 20.0, { 5, 5 } };

	//		int result = updateTruckCapacity(&truck, &shipment);
	//		Assert::AreEqual(1, result); // Success
	//	}

	//	TEST_METHOD(T21)
	//	{
	//		Map map = {
	//			//0	1  2  3  4  5  6  7  8  9  0  1  2  3  4  5  6  7  8  9  0  1  2  3  4
	//			//A B  C  D  E  F  G  H  I  J  K  L  M  N  O  P  Q  R  S  T  U  V  W  X  Y
	//			{
	//			{0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//0
	//			{0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},	//1
	//			{0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0},	//2
	//			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//3
	//			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//4
	//			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//5
	//			{1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0},	//6
	//			{1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1},	//7
	//			{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1},	//8
	//			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//9
	//			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//10
	//			{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},	//11
	//			{1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},	//12
	//			{1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},	//13
	//			{1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},	//14
	//			{1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},	//15
	//			{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//16
	//			{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//17
	//			{0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},	//18
	//			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//19
	//			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},	//20
	//			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0},	//21
	//			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1},	//22
	//			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1},	//23
	//			{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}		//24
	//			},
	//			10, 10 };

	//		struct PackageInfo package1 = { 10.0, 1.0, {0, 0} };
	//		struct PackageInfo* packages1[] = { &package1 };

	//		// Correct Truck initialization
	//		Truck* fleet = new Truck[1];
	//		fleet[0] =
	//		{ 101, 2500.0, 100.0, 1, 1, 'B', packages1 };

	//		int totalTrucks = 1;  // Number of trucks in the fleet
	//		Point pickupLocation = { 0, 0 };

	//		// Call the function
	//		int result = findNearestTruck(&map, fleet, totalTrucks, pickupLocation);

	//		// Check the result
	//		Assert::AreEqual(-1, result);

	//	}

	//	TEST_METHOD(T22)
	//	{
	//		// Test data
	//		Map map = { {{'.'}}, 10, 10 };

	//		// Trucks placed at the same distance from the pickup location
	//		Truck fleet[3] = {
	//			{101, 2500.0, 100.0, 0, 0, 'B', nullptr},
	//			{102, 2500.0, 100.0, 0, 0, 'Y', nullptr},
	//			{103, 2500.0, 100.0, 0, 0, 'G', nullptr}
	//		};

	//		int totalTrucks = 3;
	//		Point pickupLocation = { 5, 5 };

	//		// Call the function
	//		int result = findNearestTruck(&map, fleet, totalTrucks, pickupLocation);

	//		// Check the result
	//		// In case of tie, ensure the function handles it correctly (e.g., selecting the truck with the lowest ID)
	//		Assert::AreEqual(101, fleet->truckID);
	//	}
	//	TEST_METHOD(T23)
	//	{
	//		// Test data
	//		// NO TRUCK
	//		struct Map map = { {{'.'}}, 25, 25 };

	//		struct Truck fleet[3] = { {101, 0.0, 0.0, 0, 0, 'B', nullptr},
	//			{102, 0.0, 0.0, 0, 0, 'Y', nullptr},
	//			{103, 0.0, 0.0, 0, 0, 'G', nullptr}
	//		};

	//		int totalTrucks = 0;
	//		struct Point pickupLocation = { 2, 2 };

	//		// Call the function
	//		int result = findNearestTruck(&map, fleet, totalTrucks, pickupLocation);

	//		// Check the result
	//		Assert::AreEqual(-1, result);
	//	}

	//	TEST_METHOD(T24)
	//	{
	//		// Test data
	//		struct Map map = { {{'.'}}, 10, 10 };

	//		// Create packages
	//		struct PackageInfo package1 = { 10.0, 1.0, {1, 1} };
	//		struct PackageInfo package2 = { 20.0, 2.0, {2, 1} };
	//		struct PackageInfo package3 = { 15.0, 1.5, {3, 1} };

	//		// Create package pointers
	//		struct PackageInfo* packages1[] = { &package1 };
	//		struct PackageInfo* packages2[] = { &package2 };
	//		struct PackageInfo* packages3[] = { &package3 };

	//		// Create trucks
	//		struct Truck fleet[3] = {
	//		{101, 2500.0, 100.0, 0, 1, 'B', packages1},
	//		{102, 2500.0, 100.0, 0, 1, 'Y', packages2},
	//		{103, 2500.0, 100.0, 0, 1, 'G', packages3}
	//		};

	//		int totalTrucks = 3;
	//		struct Point pickupLocation = { 0, 0 };

	//		// Call the function
	//		int result = findNearestTruck(&map, fleet, totalTrucks, pickupLocation);

	//		// Check the result
	//		// Truck 1 (index 0) has the nearest distance to (0,0) with the last delivery point at (1,1)
	//		Assert::AreEqual(101, fleet->truckID);
	//	}

	//	// Test Case 1: Standard case with valid truck pointer and parameters
	//	TEST_METHOD(T25)
	//	{
	//		struct Truck* truck = nullptr;
	//		int truckID = 1;
	//		char route = 'B'; // Use a valid route character

	//		int result = initializeTruck(truck, truckID, route);
	//		Assert::AreEqual(0, result); // Check if initialization was successful

	//	}


	//	// Test Case 2: Case where truck pointer is NULL
	//	TEST_METHOD(T26)
	//	{
	//		struct Truck* truck = NULL;
	//		int truckID = 1;
	//		char route = 'B';

	//		int result = initializeTruck(truck, truckID, route);
	//		Assert::AreEqual(0, result); // Failure
	//	}

	//	// Test Case 3: Case with large truck ID
	//	TEST_METHOD(T27)
	//	{
	//		struct Truck* truck = nullptr;
	//		int truckID = 2147483647; // Maximum value for an int
	//		char route = 'B';

	//		int result = initializeTruck(truck, truckID, route);
	//		Assert::AreEqual(0, result); // Success
	//	}

	//	// Test Case 4: Case with non-standard route character
	//	TEST_METHOD(T28)
	//	{
	//		struct Truck* truck = nullptr;
	//		int truckID = 5;
	//		char route = '.'; // Non-standard route character

	//		// Call the initializeTruck function with non-standard route character
	//		int result = initializeTruck(truck, truckID, route);

	//		// Check if the function returns -1 as expected
	//		Assert::AreEqual(0, result);
	//	}

	//	TEST_METHOD(T29)
	//	{
	//		struct PackageInfo package;
	//		int result = initializePackage(&package, 500.0f, 3.0f, 10, 5);
	//		Assert::AreEqual(1, result);

	//	}
	//	TEST_METHOD(T30)
	//	{
	//		struct PackageInfo package;
	//		int result = initializePackage(&package, -100.0f, 3.0f, 10, 5);
	//		Assert::AreEqual(0, result);

	//	}
	//	TEST_METHOD(T31)
	//	{
	//		struct PackageInfo package;
	//		int result = initializePackage(&package, 500.0f, 3.0f, 26, 5);
	//		Assert::AreEqual(0, result);

	//	}
	//	TEST_METHOD(T32)
	//	{
	//		struct PackageInfo package;
	//		int result = initializePackage(&package, 500.0f, 3.0f, 25, 25);
	//		Assert::AreEqual(1, result);
	//	}

	//};

}
