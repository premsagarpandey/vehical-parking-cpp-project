#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Vehicle {
public:
    int vehicleNumber;
    string ownerName;
    string vehicleType;

    Vehicle(int number, string owner, string type) {
        vehicleNumber = number;
        ownerName = owner;
        vehicleType = type;
    }
};

class ParkingSystem {
private:
    vector<Vehicle> vehicles;
    int totalSlots;

public:
    ParkingSystem(int slots) {
        totalSlots = slots;
    }

    // Vehicle park karna
    void parkVehicle() {
        if (vehicles.size() >= totalSlots) {
            cout << "\nParking Full! No slot available.\n";
            return;
        }

        int number;
        string owner, type;

        cout << "\nEnter Vehicle Number: ";
        cin >> number;

        cout << "Enter Owner Name: ";
        cin >> owner;

        cout << "Enter Vehicle Type (Car/Bike): ";
        cin >> type;

        vehicles.push_back(Vehicle(number, owner, type));

        cout << "\nVehicle parked successfully!\n";
        cout << "Slot Number: " << vehicles.size() << endl;
    }

    // Parked vehicles display karna
    void displayVehicles() {
        if (vehicles.empty()) {
            cout << "\nNo vehicles are parked.\n";
            return;
        }

        cout << "\n========== PARKED VEHICLES ==========\n";

        for (int i = 0; i < vehicles.size(); i++) {
            cout << "\nSlot Number : " << i + 1;
            cout << "\nVehicle No  : " << vehicles[i].vehicleNumber;
            cout << "\nOwner Name  : " << vehicles[i].ownerName;
            cout << "\nVehicle Type: " << vehicles[i].vehicleType;
            cout << "\n--------------------------------------\n";
        }
    }

    // Vehicle search karna
    void searchVehicle() {
        int number;
        bool found = false;

        cout << "\nEnter Vehicle Number to search: ";
        cin >> number;

        for (int i = 0; i < vehicles.size(); i++) {
            if (vehicles[i].vehicleNumber == number) {
                cout << "\nVehicle Found!\n";
                cout << "Slot Number : " << i + 1 << endl;
                cout << "Vehicle No  : " << vehicles[i].vehicleNumber << endl;
                cout << "Owner Name  : " << vehicles[i].ownerName << endl;
                cout << "Vehicle Type: " << vehicles[i].vehicleType << endl;

                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\nVehicle not found.\n";
        }
    }

    // Vehicle remove karna
    void removeVehicle() {
        int number;
        bool found = false;

        cout << "\nEnter Vehicle Number to remove: ";
        cin >> number;

        for (int i = 0; i < vehicles.size(); i++) {
            if (vehicles[i].vehicleNumber == number) {

                vehicles.erase(vehicles.begin() + i);

                cout << "\nVehicle removed successfully!\n";

                found = true;
                break;
            }
        }

        if (!found) {
            cout << "\nVehicle not found.\n";
        }
    }

    // Available slots
    void showAvailableSlots() {
        int occupied = vehicles.size();
        int available = totalSlots - occupied;

        cout << "\n========== PARKING STATUS ==========\n";
        cout << "Total Slots     : " << totalSlots << endl;
        cout << "Occupied Slots  : " << occupied << endl;
        cout << "Available Slots : " << available << endl;
    }
};

int main() {

    ParkingSystem parking(10);

    int choice;

    do {
        cout << "\n\n====================================";
        cout << "\n       VEHICLE PARKING SYSTEM";
        cout << "\n====================================";
        cout << "\n1. Park Vehicle";
        cout << "\n2. Display Parked Vehicles";
        cout << "\n3. Search Vehicle";
        cout << "\n4. Remove Vehicle";
        cout << "\n5. Show Available Slots";
        cout << "\n6. Exit";
        cout << "\n====================================";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            parking.parkVehicle();
            break;

        case 2:
            parking.displayVehicles();
            break;

        case 3:
            parking.searchVehicle();
            break;

        case 4:
            parking.removeVehicle();
            break;

        case 5:
            parking.showAvailableSlots();
            break;

        case 6:
            cout << "\nThank you for using Parking System!\n";
            break;

        default:
            cout << "\nInvalid choice! Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}