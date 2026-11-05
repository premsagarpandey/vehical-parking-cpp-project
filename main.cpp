#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <limits>
#include <cctype>

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

// Helper: Trim leading and trailing whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Helper: Convert string to uppercase
string toUpper(string str) {
    for (char &c : str) {
        c = (char)toupper((unsigned char)c);
    }
    return str;
}

// Helper: Convert string to lowercase
string toLower(string str) {
    for (char &c : str) {
        c = (char)tolower((unsigned char)c);
    }
    return str;
}

// Helper: Clear input stream buffer safely
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Helper: Read an integer within range safely (prevents infinite loops)
int readInt(const string& prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            clearInputBuffer();
            if (val >= minVal && val <= maxVal) {
                return val;
            }
            cout << RED << "Please enter a number between " << minVal << " and " << maxVal << "." << RESET << "\n";
        } else {
            cout << RED << "Invalid input! Please enter a valid number." << RESET << "\n";
            clearInputBuffer();
        }
    }
}

// Helper: Read a non-empty string
string readLine(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (!input.empty()) {
            return input;
        }
        cout << RED << "Input cannot be empty. Please try again." << RESET << "\n";
    }
}

// Cross-platform screen pause
void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

// Cross-platform screen clear
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

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

// Check if a vehicle with the given number is already parked (case-insensitive)
bool isAlreadyParked(const string& vNo) {
    string normVNo = toUpper(trim(vNo));
    for (size_t i = 0; i < parkedList.size(); i++) {
        if (toUpper(parkedList[i].vehicleNo) == normVNo) {
            return true;
        }
    }
    return false;
}

// Find the first empty slot number (from 1 to MAX_SLOTS)
int findFreeSlot() {
    for (int slot = 1; slot <= MAX_SLOTS; slot++) {
        bool occupied = false;
        for (size_t i = 0; i < parkedList.size(); i++) {
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

// Format timestamp into readable date and time
string formatTime(time_t t) {
    char buffer[64];
    struct tm* tm_info = localtime(&t);
    if (!tm_info) return "N/A";
    strftime(buffer, sizeof(buffer), "%d-%b-%Y %I:%M %p", tm_info);
    return string(buffer);
}

// Save records to a text file
void saveData() {
    ofstream outFile("parking_data.txt");
    if (!outFile) {
        return;
    }
    outFile << totalVehiclesServiced << " " << fixed << setprecision(2) << totalRevenue << "\n";
    outFile << parkedList.size() << "\n";
    for (size_t i = 0; i < parkedList.size(); i++) {
        outFile << parkedList[i].slotNo << "\n";
        outFile << parkedList[i].vehicleNo << "\n";
        outFile << parkedList[i].ownerName << "\n";
        outFile << parkedList[i].type << "\n";
        outFile << parkedList[i].entryTime << "\n";
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
                if (!(inFile >> v.slotNo)) break;
                inFile.ignore(numeric_limits<streamsize>::max(), '\n');
                if (!getline(inFile, v.vehicleNo)) break;
                if (!getline(inFile, v.ownerName)) break;
                if (!(inFile >> v.type)) break;
                if (!(inFile >> v.entryTime)) break;
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

    string vNo = toUpper(readLine("\nEnter Vehicle Registration Number (e.g. DL 01 AB 1234): "));
    if (isAlreadyParked(vNo)) {
        cout << RED << "Error: Vehicle with number \"" << vNo << "\" is already parked!" << RESET << "\n";
        return;
    }
    v.vehicleNo = vNo;

    v.ownerName = readLine("Enter Owner Name: ");

    cout << "\nSelect Vehicle Type:\n";
    cout << "1. Bike / Scooter (Rs 20/hr)\n";
    cout << "2. Car / SUV (Rs 50/hr)\n";
    cout << "3. Bus / Truck (Rs 100/hr)\n";
    v.type = readInt("Enter choice (1-3): ", 1, 3);

    v.entryTime = time(nullptr); // current timestamp

    parkedList.push_back(v);
    saveData();

    cout << "\n----------------------------------------\n";
    cout << "        PARKING ENTRY SLIP              \n";
    cout << "----------------------------------------\n";
    cout << "Slot Number  : " << v.slotNo << "\n";
    cout << "Vehicle No   : " << v.vehicleNo << "\n";
    cout << "Owner Name   : " << v.ownerName << "\n";
    cout << "Vehicle Type : " << getVehicleTypeName(v.type) << "\n";
    cout << "Rate / Hour  : Rs " << fixed << setprecision(2) << getRatePerHour(v.type) << "\n";
    cout << "Entry Time   : " << formatTime(v.entryTime) << "\n";
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
    int ch = readInt("Enter choice (1-2): ", 1, 2);

    int index = -1;

    if (ch == 1) {
        int sNo = readInt("Enter Slot Number (1-" + to_string(MAX_SLOTS) + "): ", 1, MAX_SLOTS);
        for (size_t i = 0; i < parkedList.size(); i++) {
            if (parkedList[i].slotNo == sNo) {
                index = (int)i;
                break;
            }
        }
    } else {
        string vNo = toUpper(readLine("Enter Vehicle Number: "));
        for (size_t i = 0; i < parkedList.size(); i++) {
            if (toUpper(parkedList[i].vehicleNo) == vNo) {
                index = (int)i;
                break;
            }
        }
    }

    if (index == -1) {
        cout << RED << "\nVehicle not found in the parking lot!" << RESET << "\n";
        return;
    }

    Vehicle v = parkedList[index];
    time_t exitTime = time(nullptr);
    double seconds = difftime(exitTime, v.entryTime);
    if (seconds < 0) seconds = 0;

    int minutes = (int)(seconds / 60);
    int hours = (int)ceil(seconds / 3600.0);
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
    cout << "Slot Number  : " << v.slotNo << "\n";
    cout << "Vehicle No   : " << v.vehicleNo << "\n";
    cout << "Owner Name   : " << v.ownerName << "\n";
    cout << "Vehicle Type : " << getVehicleTypeName(v.type) << "\n";
    cout << "Entry Time   : " << formatTime(v.entryTime) << "\n";
    cout << "Exit Time    : " << formatTime(exitTime) << "\n";
    cout << "Time Parked  : " << minutes << " mins (" << hours << (hours == 1 ? " hr" : " hrs") << " billed)\n";
    cout << "Rate Applied : Rs " << fixed << setprecision(2) << rate << " per hour\n";
    cout << "----------------------------------------\n";
    cout << "TOTAL AMOUNT : Rs " << fixed << setprecision(2) << totalBill << "\n";
    cout << "Payment      : PAID (Cash / UPI)\n";
    cout << "========================================\n";
    cout << GREEN << "Vehicle unparked successfully! Slot #" << v.slotNo << " is now free." << RESET << "\n";
}

// Option 3: Display all parked vehicles sorted by Slot Number
void showParkedVehicles() {
    if (parkedList.empty()) {
        cout << YELLOW << "\nNo vehicles currently parked in the lot." << RESET << "\n";
        return;
    }

    // Sort temporary copy by slot number
    vector<Vehicle> sortedList = parkedList;
    sort(sortedList.begin(), sortedList.end(), [](const Vehicle& a, const Vehicle& b) {
        return a.slotNo < b.slotNo;
    });

    cout << "\n--- CURRENTLY PARKED VEHICLES (" << sortedList.size() << "/" << MAX_SLOTS << ") ---\n";
    cout << left << setw(8) << "Slot" 
         << setw(18) << "Vehicle No" 
         << setw(20) << "Owner Name" 
         << setw(22) << "Type" 
         << "Entry Time\n";
    cout << "----------------------------------------------------------------------------------------\n";
    for (size_t i = 0; i < sortedList.size(); i++) {
        cout << left << setw(8) << sortedList[i].slotNo
             << setw(18) << sortedList[i].vehicleNo
             << setw(20) << sortedList[i].ownerName
             << setw(22) << getVehicleTypeName(sortedList[i].type)
             << formatTime(sortedList[i].entryTime) << "\n";
    }
    cout << "----------------------------------------------------------------------------------------\n";
}

// Option 4: Search for a vehicle (case-insensitive)
void searchVehicle() {
    if (parkedList.empty()) {
        cout << RED << "\nParking lot is empty!" << RESET << "\n";
        return;
    }

    string searchKey = toLower(readLine("\nEnter Vehicle Number or Owner Name: "));

    bool found = false;
    for (size_t i = 0; i < parkedList.size(); i++) {
        string vNoLower = toLower(parkedList[i].vehicleNo);
        string oNameLower = toLower(parkedList[i].ownerName);

        if (vNoLower.find(searchKey) != string::npos ||
            oNameLower.find(searchKey) != string::npos) {
            
            if (!found) {
                cout << GREEN << "\nMatching Vehicle(s) Found:" << RESET << "\n";
                cout << "----------------------------------------\n";
            }
            found = true;

            cout << "Slot Number  : " << parkedList[i].slotNo << "\n";
            cout << "Vehicle No   : " << parkedList[i].vehicleNo << "\n";
            cout << "Owner Name   : " << parkedList[i].ownerName << "\n";
            cout << "Vehicle Type : " << getVehicleTypeName(parkedList[i].type) << "\n";
            cout << "Entry Time   : " << formatTime(parkedList[i].entryTime) << "\n";
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
    for (size_t i = 0; i < parkedList.size(); i++) {
        if (parkedList[i].type == 1) bikeCount++;
        else if (parkedList[i].type == 2) carCount++;
        else if (parkedList[i].type == 3) heavyCount++;
    }

    int occupied = (int)parkedList.size();
    int freeSlots = MAX_SLOTS - occupied;

    cout << "\n========================================\n";
    cout << "        PARKING SYSTEM REPORT           \n";
    cout << "========================================\n";
    cout << "Total Slots Capacity : " << MAX_SLOTS << "\n";
    cout << "Currently Occupied   : " << occupied << "\n";
    cout << "Currently Free       : " << freeSlots << "\n";
    cout << "----------------------------------------\n";
    cout << "Active Bikes         : " << bikeCount << "\n";
    cout << "Active Cars          : " << carCount << "\n";
    cout << "Active Heavy Vehicles: " << heavyCount << "\n";
    cout << "----------------------------------------\n";
    cout << "Total Serviced       : " << totalVehiclesServiced << " vehicle(s)\n";
    cout << "Total Revenue        : Rs " << fixed << setprecision(2) << totalRevenue << "\n";
    cout << "========================================\n";
}

// Option 7: Clear all parking records
void clearAllRecords() {
    cout << RED << "\nWARNING: Are you sure you want to clear ALL records?" << RESET << "\n";
    cout << "This will delete all currently parked vehicles, total revenue, and service count.\n";
    string confirm = toUpper(readLine("Type 'YES' to confirm: "));
    if (confirm == "YES" || confirm == "Y") {
        parkedList.clear();
        totalVehiclesServiced = 0;
        totalRevenue = 0.0;
        saveData();
        cout << GREEN << "\nAll records have been cleared successfully! Fresh start initialized." << RESET << "\n";
    } else {
        cout << YELLOW << "\nOperation cancelled. Records are safe." << RESET << "\n";
    }
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
        cout << "7. Clear All Records\n";
        cout << "-------------------------------------\n";
        choice = readInt("Enter your choice (1-7): ", 1, 7);

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
            case 7:
                clearAllRecords();
                break;
            default:
                cout << RED << "\nInvalid choice! Please enter 1 to 7." << RESET << "\n";
                break;
        }

        if (choice != 6) {
            pauseScreen();
            clearScreen();
        }
    }

    saveData();
    return 0;
}
