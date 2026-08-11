#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>

using namespace std;

// Maximum parking slots available in the lot
const int MAX_SLOTS = 20;

// ANSI Color Codes for Console
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN = "\033[36m";

// Structure to store vehicle details
struct Vehicle {
    int slotNo;
    string vehicleNo;
    string ownerName;
    int type;          // 1: Bike, 2: Car, 3: Bus/Truck
    time_t entryTime;
};

// Global list to store currently parked vehicles
vector<Vehicle> parkedList;
int totalVehiclesServiced = 0;
double totalRevenue = 0.0;

// Function to convert vehicle type number to readable name
string getVehicleTypeName(int type) {
    if (type == 1) return "Bike (2-Wheeler)";
    if (type == 2) return "Car (4-Wheeler)";
    if (type == 3) return "Bus/Truck (Heavy)";
    return "Other";
}

// Function to get rate per hour based on vehicle category
double getRatePerHour(int type) {
    if (type == 1) return 20.0;  // Rs 20/hr for Bike
    if (type == 2) return 50.0;  // Rs 50/hr for Car
    if (type == 3) return 100.0; // Rs 100/hr for Heavy Vehicle
    return 30.0;
}

// Check if a vehicle with the given number is already parked
bool isAlreadyParked(string vNo) {
    int total = (int)parkedList.size();
    for (int i = 0; i < total; i++) {
        if (parkedList[i].vehicleNo == vNo) {
            return true;
        }
    }
    return false;
}

// Find the first empty slot number (from 1 to MAX_SLOTS)
int findFreeSlot() {
    int total = (int)parkedList.size();
    for (int slot = 1; slot <= MAX_SLOTS; slot++) {
        bool occupied = false;
        for (int i = 0; i < total; i++) {
            if (parkedList[i].slotNo == slot) {
                occupied = true;
                break;
            }
        }
        if (!occupied) {
            return slot; // found empty slot
        }
    }
    return -1; // parking is full
}

// Save records to a text file
void saveData() {
    ofstream outFile("parking_data.txt");
    if (!outFile) {
        return;
    }
    outFile << totalVehiclesServiced << " " << totalRevenue << endl;
    outFile << parkedList.size() << endl;
    for (int i = 0; i < (int)parkedList.size(); i++) {
        outFile << parkedList[i].slotNo << endl;
        outFile << parkedList[i].vehicleNo << endl;
        outFile << parkedList[i].ownerName << endl;
        outFile << parkedList[i].type << endl;
        outFile << parkedList[i].entryTime << endl;
    }
    outFile.close();
}

// Load saved records from file
void loadData() {
    ifstream inFile("parking_data.txt");
    if (!inFile) {
        return; // No existing file found
    }
    int count = 0;
    if (inFile >> totalVehiclesServiced >> totalRevenue) {
        if (inFile >> count) {
            parkedList.clear();
            for (int i = 0; i < count; i++) {
                Vehicle v;
                inFile >> v.slotNo;
                inFile.ignore();
                getline(inFile, v.vehicleNo);
                getline(inFile, v.ownerName);
                inFile >> v.type;
                inFile >> v.entryTime;
                parkedList.push_back(v);
            }
        }
    }
    inFile.close();
}

// Option 1: Park a new vehicle
void parkVehicle() {
    if ((int)parkedList.size() >= MAX_SLOTS) {
        cout << RED << "\nSorry! Parking lot is completely full." << RESET << "\n";
        return;
    }

    int freeSlot = findFreeSlot();
    if (freeSlot == -1) {
        cout << RED << "\nNo free slot available!" << RESET << "\n";
        return;
    }

    Vehicle v;
    v.slotNo = freeSlot;

    cin.ignore();
    cout << "\nEnter Vehicle Registration Number (e.g. DL 01 AB 1234): ";
    getline(cin, v.vehicleNo);

    if (v.vehicleNo.empty()) {
        cout << RED << "Error: Vehicle number cannot be empty!" << RESET << "\n";
        return;
    }

    if (isAlreadyParked(v.vehicleNo)) {
        cout << RED << "Error: Vehicle with number \"" << v.vehicleNo << "\" is already parked!" << RESET << "\n";
        return;
    }

    cout << "Enter Owner Name: ";
    getline(cin, v.ownerName);

    cout << "\nSelect Vehicle Type:\n";
    cout << "1. Bike / Scooter (Rs 20/hr)\n";
    cout << "2. Car / SUV (Rs 50/hr)\n";
    cout << "3. Bus / Truck (Rs 100/hr)\n";
    cout << "Enter choice (1-3): ";
    cin >> v.type;

    if (v.type < 1 || v.type > 3) {
        v.type = 2; // default to car
    }

    v.entryTime = time(0); // current timestamp

    parkedList.push_back(v);
    saveData();

    cout << "\n----------------------------------------\n";
    cout << "        PARKING ENTRY SLIP              \n";
    cout << "----------------------------------------\n";
    cout << "Slot Number  : " << v.slotNo << endl;
    cout << "Vehicle No   : " << v.vehicleNo << endl;
    cout << "Owner Name   : " << v.ownerName << endl;
    cout << "Vehicle Type : " << getVehicleTypeName(v.type) << endl;
    cout << "Rate / Hour  : Rs " << getRatePerHour(v.type) << endl;
    cout << "Status       : " << GREEN << "Parked Successfully!" << RESET << "\n";
    cout << "----------------------------------------\n";
}

// Option 2: Unpark vehicle and generate bill
void unparkVehicle() {
    if (parkedList.empty()) {
        cout << RED << "\nParking lot is empty! No vehicles to unpark." << RESET << "\n";
        return;
    }

    cout << "\nChoose Unpark Method:\n";
    cout << "1. By Slot Number\n";
    cout << "2. By Vehicle Number\n";
    cout << "Enter choice: ";
    int ch;
    cin >> ch;

    int index = -1;
    int total = (int)parkedList.size();

    if (ch == 1) {
        int sNo;
        cout << "Enter Slot Number (1-" << MAX_SLOTS << "): ";
        cin >> sNo;
        for (int i = 0; i < total; i++) {
            if (parkedList[i].slotNo == sNo) {
                index = i;
                break;
            }
        }
    } else {
        cin.ignore();
        string vNo;
        cout << "Enter Vehicle Number: ";
        getline(cin, vNo);
        for (int i = 0; i < total; i++) {
            if (parkedList[i].vehicleNo == vNo) {
                index = i;
                break;
            }
        }
    }

    if (index == -1) {
        cout << RED << "\nVehicle not found in the parking lot!" << RESET << "\n";
        return;
    }

    Vehicle v = parkedList[index];
    time_t exitTime = time(0);
    double seconds = difftime(exitTime, v.entryTime);
    if (seconds < 0) seconds = 0;

    int minutes = (int)(seconds / 60);
    int hours = (int)ceil((double)seconds / 3600.0);
    if (hours < 1) hours = 1; // Minimum 1 hour charge

    double rate = getRatePerHour(v.type);
    double totalBill = hours * rate;

    // Update global revenue
    totalVehiclesServiced++;
    totalRevenue += totalBill;

    // Remove vehicle from parked list
    parkedList.erase(parkedList.begin() + index);
    saveData();

    cout << "\n========================================\n";
    cout << "          CHECK-OUT BILL / RECEIPT      \n";
    cout << "========================================\n";
    cout << "Slot Number  : " << v.slotNo << endl;
    cout << "Vehicle No   : " << v.vehicleNo << endl;
    cout << "Owner Name   : " << v.ownerName << endl;
    cout << "Vehicle Type : " << getVehicleTypeName(v.type) << endl;
    cout << "Time Parked  : " << minutes << " mins (" << hours << " hr(s) billed)\n";
    cout << "Rate Applied : Rs " << rate << " per hour\n";
    cout << "----------------------------------------\n";
    cout << "TOTAL AMOUNT : Rs " << totalBill << endl;
    cout << "Payment      : PAID (Cash / UPI)\n";
    cout << "========================================\n";
    cout << GREEN << "Vehicle unparked successfully! Slot #" << v.slotNo << " is now free." << RESET << "\n";
}

// Option 3: Display all parked vehicles
void showParkedVehicles() {
    if (parkedList.empty()) {
        cout << YELLOW << "\nNo vehicles currently parked in the lot." << RESET << "\n";
        return;
    }

    cout << "\n--- CURRENTLY PARKED VEHICLES (" << parkedList.size() << "/" << MAX_SLOTS << ") ---\n";
    cout << "Slot\tVehicle No\t\tOwner Name\t\tType\n";
    cout << "--------------------------------------------------------------------\n";
    for (int i = 0; i < (int)parkedList.size(); i++) {
        cout << parkedList[i].slotNo << "\t"
             << parkedList[i].vehicleNo << "\t\t"
             << parkedList[i].ownerName << "\t\t"
             << getVehicleTypeName(parkedList[i].type) << "\n";
    }
    cout << "--------------------------------------------------------------------\n";
}

// Option 4: Search for a vehicle
void searchVehicle() {
    if (parkedList.empty()) {
        cout << RED << "\nParking lot is empty!" << RESET << "\n";
        return;
    }

    cin.ignore();
    string searchKey;
    cout << "\nEnter Vehicle Number or Owner Name: ";
    getline(cin, searchKey);

    bool found = false;
    for (int i = 0; i < (int)parkedList.size(); i++) {
        if (parkedList[i].vehicleNo.find(searchKey) != string::npos ||
            parkedList[i].ownerName.find(searchKey) != string::npos) {
            
            if (!found) {
                cout << GREEN << "\nMatching Vehicle(s) Found:" << RESET << "\n";
                cout << "----------------------------------------\n";
            }
            found = true;
            cout << "Slot Number  : " << parkedList[i].slotNo << endl;
            cout << "Vehicle No   : " << parkedList[i].vehicleNo << endl;
            cout << "Owner Name   : " << parkedList[i].ownerName << endl;
            cout << "Vehicle Type : " << getVehicleTypeName(parkedList[i].type) << endl;
            cout << "----------------------------------------\n";
        }
    }

    if (!found) {
        cout << RED << "\nNo vehicle found matching \"" << searchKey << "\"." << RESET << "\n";
    }
}

// Option 5: Show parking summary report
void showStats() {
    int bikeCount = 0, carCount = 0, heavyCount = 0;
    for (int i = 0; i < (int)parkedList.size(); i++) {
        if (parkedList[i].type == 1) bikeCount++;
        else if (parkedList[i].type == 2) carCount++;
        else if (parkedList[i].type == 3) heavyCount++;
    }

    int occupied = (int)parkedList.size();
    int freeSlots = MAX_SLOTS - occupied;

    cout << "\n========================================\n";
    cout << "        PARKING SYSTEM REPORT           \n";
    cout << "========================================\n";
    cout << "Total Slots Capacity : " << MAX_SLOTS << endl;
    cout << "Currently Occupied   : " << occupied << endl;
    cout << "Currently Free       : " << freeSlots << endl;
    cout << "----------------------------------------\n";
    cout << "Active Bikes         : " << bikeCount << endl;
    cout << "Active Cars          : " << carCount << endl;
    cout << "Active Heavy Vehicles: " << heavyCount << endl;
    cout << "----------------------------------------\n";
    cout << "Total Serviced       : " << totalVehiclesServiced << " vehicle(s)\n";
    cout << "Total Revenue        : Rs " << totalRevenue << endl;
    cout << "========================================\n";
}

// Main Menu
int main() {
    loadData();

    int choice = 0;
    while (choice != 6) {
        cout << CYAN << "\n=====================================\n";
        cout << "  VEHICLE PARKING MANAGEMENT SYSTEM  \n";
        cout << "=====================================\n" << RESET;
        cout << "1. Park Vehicle\n";
        cout << "2. Unpark Vehicle & Bill\n";
        cout << "3. View All Parked Vehicles\n";
        cout << "4. Search Vehicle\n";
        cout << "5. View Parking Report\n";
        cout << "6. Exit\n";
        cout << "-------------------------------------\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;

        switch (choice) {
            case 1:
                parkVehicle();
                break;
            case 2:
                unparkVehicle();
                break;
            case 3:
                showParkedVehicles();
                break;
            case 4:
                searchVehicle();
                break;
            case 5:
                showStats();
                break;
            case 6:
                cout << CYAN << "\nExiting... Thank you for using the system!" << RESET << "\n";
                break;
            default:
                cout << RED << "\nInvalid choice! Please enter 1 to 6." << RESET << "\n";
        }

        if (choice != 6) {
            cout << "\n";
            system("pause"); // user se wait karayega taaki output padh sake
            system("cls");   // screen clear kar dega
        }
    }

    saveData();
    return 0;
}