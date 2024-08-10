/*
*           SFT PROJECT - MILESTONE 5
*           SECTION: ZBB
*           GROUP: D
*           TEAM MEMBERS:
*           ARSHNOOR KAUR, GURMEHAK KAUR UPPAL, AJAYPARTAP SINGH MAAN, MANAS GANDOTRA
*/
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
}