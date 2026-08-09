#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cmath>
#include <limits>
#include <algorithm>

using namespace std;

// ============================================================================
// CONSTANTS & ENUMS
// ============================================================================
const int DEFAULT_TOTAL_SLOTS = 20;
const string DATA_FILE = "parking_records.txt";
const string HISTORY_FILE = "parking_history.txt";
const string STATS_FILE = "parking_stats.txt";

enum class VehicleType {
    TWO_WHEELER = 1,
    FOUR_WHEELER = 2,
    HEAVY_VEHICLE = 3,
    UNKNOWN = 0
};

// ============================================================================
// HELPER UTILITY FUNCTIONS
// ============================================================================
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

string toUpperCase(string str) {
    transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return toupper(c); });
    return str;
}

string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::TWO_WHEELER: return "2-Wheeler (Bike/Scooter)";
        case VehicleType::FOUR_WHEELER: return "4-Wheeler (Car/SUV)";
        case VehicleType::HEAVY_VEHICLE: return "Heavy Vehicle (Bus/Truck)";
        default: return "Unknown";
    }
}

string vehicleTypeToShortString(VehicleType type) {
    switch (type) {
        case VehicleType::TWO_WHEELER: return "Bike";
        case VehicleType::FOUR_WHEELER: return "Car";
        case VehicleType::HEAVY_VEHICLE: return "Heavy";
        default: return "N/A";
    }
}

VehicleType intToVehicleType(int choice) {
    switch (choice) {
        case 1: return VehicleType::TWO_WHEELER;
        case 2: return VehicleType::FOUR_WHEELER;
        case 3: return VehicleType::HEAVY_VEHICLE;
        default: return VehicleType::UNKNOWN;
    }
}

double getHourlyRate(VehicleType type) {
    switch (type) {
        case VehicleType::TWO_WHEELER: return 20.0;   // ₹20 per hour
        case VehicleType::FOUR_WHEELER: return 50.0;  // ₹50 per hour
        case VehicleType::HEAVY_VEHICLE: return 100.0;// ₹100 per hour
        default: return 30.0;
    }
}

string formatTime(time_t rawTime) {
    if (rawTime == 0) return "N/A";
    struct tm* timeInfo = localtime(&rawTime);
    char buffer[64];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeInfo);
    return string(buffer);
}

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getValidInt(int minVal, int maxVal, const string& prompt = "Enter your choice: ") {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            if (val >= minVal && val <= maxVal) {
                clearInputBuffer();
                return val;
            } else {
                cout << ">> Error: Please enter a number between " << minVal << " and " << maxVal << ".\n";
            }
        } else {
            cout << ">> Error: Invalid input! Please enter a valid number.\n";
            clearInputBuffer();
        }
    }
}

string getValidString(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        input = trim(input);
        if (!input.empty()) {
            return input;
        }
        cout << ">> Error: Input cannot be empty! Please try again.\n";
    }
}

void pauseConsole() {
    cout << "\nPress Enter to continue...";
    cin.get();
}

// ============================================================================
// VEHICLE CLASS
// ============================================================================
class Vehicle {
private:
    string regNumber;
    string ownerName;
    string contactNumber;
    VehicleType type;
    time_t entryTime;

public:
    Vehicle() : regNumber(""), ownerName(""), contactNumber(""), type(VehicleType::UNKNOWN), entryTime(0) {}

    Vehicle(string reg, string owner, string contact, VehicleType vType, time_t timeStamp = 0)
        : regNumber(toUpperCase(trim(reg))),
          ownerName(trim(owner)),
          contactNumber(trim(contact)),
          type(vType),
          entryTime(timeStamp == 0 ? time(nullptr) : timeStamp) {}

    // Getters
    string getRegNumber() const { return regNumber; }
    string getOwnerName() const { return ownerName; }
    string getContactNumber() const { return contactNumber; }
    VehicleType getType() const { return type; }
    time_t getEntryTime() const { return entryTime; }

    // Serialization
    string serialize() const {
        stringstream ss;
        ss << regNumber << "|" << ownerName << "|" << contactNumber << "|"
           << static_cast<int>(type) << "|" << entryTime;
        return ss.str();
    }

    static Vehicle deserialize(const string& data) {
        stringstream ss(data);
        string reg, owner, contact, typeStr, timeStr;

        if (getline(ss, reg, '|') &&
            getline(ss, owner, '|') &&
            getline(ss, contact, '|') &&
            getline(ss, typeStr, '|') &&
            getline(ss, timeStr)) {
            
            VehicleType vType = intToVehicleType(stoi(typeStr));
            time_t t = static_cast<time_t>(stoll(timeStr));
            return Vehicle(reg, owner, contact, vType, t);
        }
        return Vehicle();
    }
};

// ============================================================================
// PARKING SPOT CLASS
// ============================================================================
class ParkingSpot {
private:
    int slotNumber;
    bool occupied;
    Vehicle vehicle;

public:
    ParkingSpot(int number = 0) : slotNumber(number), occupied(false), vehicle() {}

    int getSlotNumber() const { return slotNumber; }
    bool isOccupied() const { return occupied; }
    Vehicle getVehicle() const { return vehicle; }

    void park(const Vehicle& v) {
        vehicle = v;
        occupied = true;
    }

    void vacate() {
        occupied = false;
        vehicle = Vehicle();
    }
};

// ============================================================================
// PARKING LOT MANAGEMENT SYSTEM CLASS
// ============================================================================
class ParkingSystem {
private:
    int totalSlots;
    vector<ParkingSpot> spots;
    double totalRevenueCollected;
    int totalVehiclesServiced;

    void saveToFile() {
        ofstream outFile(DATA_FILE);
        if (outFile.is_open()) {
            for (const auto& spot : spots) {
                if (spot.isOccupied()) {
                    outFile << spot.getSlotNumber() << "|" << spot.getVehicle().serialize() << "\n";
                }
            }
            outFile.close();
        }

        ofstream statFile(STATS_FILE);
        if (statFile.is_open()) {
            statFile << totalRevenueCollected << "\n" << totalVehiclesServiced << "\n";
            statFile.close();
        }
    }

    void loadFromFile() {
        // Load active vehicles
        ifstream inFile(DATA_FILE);
        if (inFile.is_open()) {
            string line;
            while (getline(inFile, line)) {
                if (line.empty()) continue;
                size_t firstDelim = line.find('|');
                if (firstDelim != string::npos) {
                    int slotNum = stoi(line.substr(0, firstDelim));
                    string vehicleData = line.substr(firstDelim + 1);
                    if (slotNum >= 1 && slotNum <= totalSlots) {
                        Vehicle v = Vehicle::deserialize(vehicleData);
                        if (!v.getRegNumber().empty()) {
                            spots[slotNum - 1].park(v);
                        }
                    }
                }
            }
            inFile.close();
        }

        // Load stats
        ifstream statFile(STATS_FILE);
        if (statFile.is_open()) {
            statFile >> totalRevenueCollected >> totalVehiclesServiced;
            statFile.close();
        }
    }

    void logHistory(const Vehicle& v, int slotNo, time_t exitTime, double durationHours, double billAmount) {
        ofstream histFile(HISTORY_FILE, ios::app);
        if (histFile.is_open()) {
            histFile << "--------------------------------------------------------\n";
            histFile << "RECEIPT NO: RCP-" << time(nullptr) << "-" << slotNo << "\n";
            histFile << "Vehicle Number : " << v.getRegNumber() << "\n";
            histFile << "Owner Name     : " << v.getOwnerName() << " (" << v.getContactNumber() << ")\n";
            histFile << "Vehicle Type   : " << vehicleTypeToString(v.getType()) << "\n";
            histFile << "Slot Number    : " << slotNo << "\n";
            histFile << "Entry Time     : " << formatTime(v.getEntryTime()) << "\n";
            histFile << "Exit Time      : " << formatTime(exitTime) << "\n";
            histFile << "Duration       : " << fixed << setprecision(2) << durationHours << " Hours\n";
            histFile << "Amount Paid    : Rs. " << fixed << setprecision(2) << billAmount << "\n";
            histFile.close();
        }
    }

public:
    ParkingSystem(int slots = DEFAULT_TOTAL_SLOTS)
        : totalSlots(slots), totalRevenueCollected(0.0), totalVehiclesServiced(0) {
        spots.reserve(totalSlots);
        for (int i = 1; i <= totalSlots; i++) {
            spots.push_back(ParkingSpot(i));
        }
        loadFromFile();
    }

    ~ParkingSystem() {
        saveToFile();
    }

    int getOccupiedCount() const {
        int count = 0;
        for (const auto& spot : spots) {
            if (spot.isOccupied()) count++;
        }
        return count;
    }

    int getAvailableCount() const {
        return totalSlots - getOccupiedCount();
    }

    int findVehicleSlot(const string& regNumber) const {
        string searchKey = toUpperCase(trim(regNumber));
        for (const auto& spot : spots) {
            if (spot.isOccupied() && spot.getVehicle().getRegNumber() == searchKey) {
                return spot.getSlotNumber();
            }
        }
        return -1;
    }

    // ========================================================================
    // FEATURE 1: PARK VEHICLE (CHECK-IN)
    // ========================================================================
    void parkVehicle() {
        int freeSlot = -1;
        for (const auto& spot : spots) {
            if (!spot.isOccupied()) {
                freeSlot = spot.getSlotNumber();
                break;
            }
        }

        if (freeSlot == -1) {
            cout << "\n========================================================\n";
            cout << " [!] PARKING FULL: No available slots at the moment!\n";
            cout << "========================================================\n";
            return;
        }

        cout << "\n========================================================\n";
        cout << "                VEHICLE CHECK-IN (ENTRY)\n";
        cout << "========================================================\n";
        cout << " Next Available Slot: Slot #" << freeSlot << "\n\n";

        string regNumber = getValidString(" Enter Vehicle Registration Number (e.g. DL 01 AB 1234): ");
        regNumber = toUpperCase(regNumber);

        // Check for duplicate vehicle registration
        if (findVehicleSlot(regNumber) != -1) {
            cout << "\n>> [Error]: Vehicle with Number '" << regNumber 
                 << "' is ALREADY parked in Slot #" << findVehicleSlot(regNumber) << "!\n";
            return;
        }

        string ownerName = getValidString(" Enter Owner Full Name: ");
        string contactNumber = getValidString(" Enter Contact Phone Number: ");

        cout << "\n Select Vehicle Type:\n";
        cout << "   1. 2-Wheeler (Bike / Scooter)     [Rate: Rs. 20/hr]\n";
        cout << "   2. 4-Wheeler (Car / SUV / Van)    [Rate: Rs. 50/hr]\n";
        cout << "   3. Heavy Vehicle (Bus / Truck)    [Rate: Rs. 100/hr]\n";
        int typeChoice = getValidInt(1, 3, " Choice (1-3): ");
        VehicleType vType = intToVehicleType(typeChoice);

        time_t entryTime = time(nullptr);
        Vehicle newVehicle(regNumber, ownerName, contactNumber, vType, entryTime);

        spots[freeSlot - 1].park(newVehicle);
        saveToFile();

        // Print Entry Parking Slip
        cout << "\n+------------------------------------------------------+\n";
        cout << "|                 PARKING ENTRY SLIP                   |\n";
        cout << "+------------------------------------------------------+\n";
        cout << "  Slot Number    : #" << freeSlot << "\n";
        cout << "  Vehicle Number : " << newVehicle.getRegNumber() << "\n";
        cout << "  Owner Name     : " << newVehicle.getOwnerName() << "\n";
        cout << "  Contact Number : " << newVehicle.getContactNumber() << "\n";
        cout << "  Vehicle Type   : " << vehicleTypeToString(newVehicle.getType()) << "\n";
        cout << "  Entry Timestamp: " << formatTime(newVehicle.getEntryTime()) << "\n";
        cout << "  Hourly Rate    : Rs. " << fixed << setprecision(2) << getHourlyRate(newVehicle.getType()) << "/hr\n";
        cout << "+------------------------------------------------------+\n";
        cout << ">> Vehicle parked successfully in Slot #" << freeSlot << "!\n";
    }

    // ========================================================================
    // FEATURE 2: UNPARK VEHICLE & GENERATE BILL (CHECK-OUT)
    // ========================================================================
    void unparkVehicle() {
        if (getOccupiedCount() == 0) {
            cout << "\n>> Parking lot is completely empty! No vehicles to unpark.\n";
            return;
        }

        cout << "\n========================================================\n";
        cout << "               VEHICLE CHECK-OUT (EXIT & BILL)\n";
        cout << "========================================================\n";
        cout << " Search vehicle by:\n";
        cout << "   1. Slot Number\n";
        cout << "   2. Vehicle Registration Number\n";
        int searchOpt = getValidInt(1, 2, " Choice (1-2): ");

        int targetSlot = -1;
        if (searchOpt == 1) {
            int slot = getValidInt(1, totalSlots, " Enter Slot Number (1-" + to_string(totalSlots) + "): ");
            if (spots[slot - 1].isOccupied()) {
                targetSlot = slot;
            } else {
                cout << "\n>> Slot #" << slot << " is already EMPTY!\n";
                return;
            }
        } else {
            string reg = getValidString(" Enter Vehicle Registration Number: ");
            targetSlot = findVehicleSlot(reg);
            if (targetSlot == -1) {
                cout << "\n>> Vehicle '" << toUpperCase(reg) << "' NOT found in the parking lot!\n";
                return;
            }
        }

        ParkingSpot& spot = spots[targetSlot - 1];
        Vehicle v = spot.getVehicle();
        time_t exitTime = time(nullptr);

        double elapsedSeconds = difftime(exitTime, v.getEntryTime());
        if (elapsedSeconds < 0) elapsedSeconds = 0;

        // Calculate hours (minimum 1 hour base charge for any entry)
        double durationHours = elapsedSeconds / 3600.0;
        int billableHours = static_cast<int>(ceil(durationHours));
        if (billableHours < 1) billableHours = 1;

        double rate = getHourlyRate(v.getType());
        double totalBill = billableHours * rate;

        // Display Tax Invoice / Receipt
        cout << "\n";
        cout << "========================================================\n";
        cout << "                PARKING BILL & RECEIPT                  \n";
        cout << "========================================================\n";
        cout << " Invoice ID     : INV-" << time(nullptr) << "-" << targetSlot << "\n";
        cout << " Slot Number    : Slot #" << targetSlot << "\n";
        cout << " Vehicle Number : " << v.getRegNumber() << "\n";
        cout << " Owner Name     : " << v.getOwnerName() << "\n";
        cout << " Contact Number : " << v.getContactNumber() << "\n";
        cout << " Vehicle Type   : " << vehicleTypeToString(v.getType()) << "\n";
        cout << " Entry Time     : " << formatTime(v.getEntryTime()) << "\n";
        cout << " Exit Time      : " << formatTime(exitTime) << "\n";
        cout << " Elapsed Time   : " << static_cast<int>(elapsedSeconds / 60) << " min (" 
             << fixed << setprecision(2) << durationHours << " hrs)\n";
        cout << " Billable Hours : " << billableHours << " hr(s)\n";
        cout << " Hourly Rate    : Rs. " << fixed << setprecision(2) << rate << "/hr\n";
        cout << "--------------------------------------------------------\n";
        cout << " TOTAL AMOUNT   : Rs. " << fixed << setprecision(2) << totalBill << "\n";
        cout << "========================================================\n";

        cout << "\n Confirm payment and checkout? (1. Yes / 2. Cancel): ";
        int confirm = getValidInt(1, 2, "");
        if (confirm == 1) {
            // Update accounting & logs
            totalRevenueCollected += totalBill;
            totalVehiclesServiced++;
            logHistory(v, targetSlot, exitTime, durationHours, totalBill);

            spot.vacate();
            saveToFile();
            cout << "\n>> Payment received! Vehicle checked out and Slot #" << targetSlot << " is now FREE.\n";
        } else {
            cout << "\n>> Check-out cancelled. Vehicle remains parked in Slot #" << targetSlot << ".\n";
        }
    }

    // ========================================================================
    // FEATURE 3: DISPLAY ALL PARKED VEHICLES (DETAILED TABLE)
    // ========================================================================
    void displayParkedVehicles() const {
        int occupied = getOccupiedCount();
        if (occupied == 0) {
            cout << "\n>> No vehicles currently parked in the parking lot.\n";
            return;
        }

        cout << "\n====================================================================================================\n";
        cout << "                                  CURRENTLY PARKED VEHICLES (" << occupied << "/" << totalSlots << ")\n";
        cout << "====================================================================================================\n";
        cout << left 
             << setw(6)  << "Slot"
             << setw(16) << "Reg Number"
             << setw(22) << "Owner Name"
             << setw(15) << "Contact"
             << setw(16) << "Type"
             << setw(22) << "Entry Time"
             << "\n";
        cout << string(97, '-') << "\n";

        for (const auto& spot : spots) {
            if (spot.isOccupied()) {
                Vehicle v = spot.getVehicle();
                cout << left 
                     << setw(6)  << ("#" + to_string(spot.getSlotNumber()))
                     << setw(16) << v.getRegNumber()
                     << setw(22) << (v.getOwnerName().length() > 20 ? v.getOwnerName().substr(0, 18) + ".." : v.getOwnerName())
                     << setw(15) << v.getContactNumber()
                     << setw(16) << vehicleTypeToShortString(v.getType())
                     << setw(22) << formatTime(v.getEntryTime())
                     << "\n";
            }
        }
        cout << string(97, '=') << "\n";
    }

    // ========================================================================
    // FEATURE 4: VISUAL PARKING LOT GRID MAP
    // ========================================================================
    void displayParkingGrid() const {
        cout << "\n========================================================================\n";
        cout << "                         PARKING LOT VISUAL MAP                         \n";
        cout << "========================================================================\n";
        cout << " Total: " << totalSlots << " | Occupied: " << getOccupiedCount() 
             << " | Available: " << getAvailableCount() << "\n\n";

        const int cols = 4;
        int rows = (totalSlots + cols - 1) / cols;

        for (int r = 0; r < rows; r++) {
            cout << "+-------------------+-------------------+-------------------+-------------------+\n";
            for (int c = 0; c < cols; c++) {
                int slotIndex = r * cols + c;
                if (slotIndex < totalSlots) {
                    const auto& spot = spots[slotIndex];
                    stringstream cell;
                    cell << " Slot #" << setfill('0') << setw(2) << spot.getSlotNumber() << ": ";
                    if (spot.isOccupied()) {
                        string reg = spot.getVehicle().getRegNumber();
                        if (reg.length() > 8) reg = reg.substr(0, 7) + ".";
                        cell << reg;
                    } else {
                        cell << "[FREE]";
                    }
                    cout << "|" << left << setfill(' ') << setw(19) << cell.str();
                } else {
                    cout << "|" << string(19, ' ');
                }
            }
            cout << "|\n";
        }
        cout << "+-------------------+-------------------+-------------------+-------------------+\n";
    }

    // ========================================================================
    // FEATURE 5: SEARCH VEHICLE (BY NUMBER OR OWNER NAME)
    // ========================================================================
    void searchVehicle() const {
        cout << "\n========================================================\n";
        cout << "                     SEARCH VEHICLE                     \n";
        cout << "========================================================\n";
        cout << " 1. Search by Registration Number\n";
        cout << " 2. Search by Owner Name\n";
        int opt = getValidInt(1, 2, " Choice (1-2): ");

        if (opt == 1) {
            string reg = getValidString(" Enter Registration Number to search: ");
            reg = toUpperCase(reg);
            bool found = false;

            for (const auto& spot : spots) {
                if (spot.isOccupied()) {
                    Vehicle v = spot.getVehicle();
                    if (v.getRegNumber().find(reg) != string::npos) {
                        cout << "\n>> MATCH FOUND in Slot #" << spot.getSlotNumber() << ":\n";
                        cout << "   - Reg Number : " << v.getRegNumber() << "\n";
                        cout << "   - Owner Name : " << v.getOwnerName() << " (" << v.getContactNumber() << ")\n";
                        cout << "   - Type       : " << vehicleTypeToString(v.getType()) << "\n";
                        cout << "   - Entry Time : " << formatTime(v.getEntryTime()) << "\n";
                        found = true;
                    }
                }
            }
            if (!found) {
                cout << "\n>> No vehicle found matching '" << reg << "'.\n";
            }
        } else {
            string name = getValidString(" Enter Owner Name to search: ");
            string searchName = toUpperCase(name);
            bool found = false;

            for (const auto& spot : spots) {
                if (spot.isOccupied()) {
                    Vehicle v = spot.getVehicle();
                    string ownerUpper = toUpperCase(v.getOwnerName());
                    if (ownerUpper.find(searchName) != string::npos) {
                        cout << "\n>> MATCH FOUND in Slot #" << spot.getSlotNumber() << ":\n";
                        cout << "   - Owner Name : " << v.getOwnerName() << " (" << v.getContactNumber() << ")\n";
                        cout << "   - Reg Number : " << v.getRegNumber() << "\n";
                        cout << "   - Type       : " << vehicleTypeToString(v.getType()) << "\n";
                        cout << "   - Entry Time : " << formatTime(v.getEntryTime()) << "\n";
                        found = true;
                    }
                }
            }
            if (!found) {
                cout << "\n>> No vehicle found for owner name matching '" << name << "'.\n";
            }
        }
    }

    // ========================================================================
    // FEATURE 6: PARKING LOT STATISTICS & REVENUE REPORT
    // ========================================================================
    void displayStatistics() const {
        int bikes = 0, cars = 0, heavies = 0;
        for (const auto& spot : spots) {
            if (spot.isOccupied()) {
                switch (spot.getVehicle().getType()) {
                    case VehicleType::TWO_WHEELER: bikes++; break;
                    case VehicleType::FOUR_WHEELER: cars++; break;
                    case VehicleType::HEAVY_VEHICLE: heavies++; break;
                    default: break;
                }
            }
        }

        int occupied = getOccupiedCount();
        double occupancyRate = (static_cast<double>(occupied) / totalSlots) * 100.0;

        cout << "\n========================================================\n";
        cout << "           PARKING LOT ANALYTICS & REVENUE              \n";
        cout << "========================================================\n";
        cout << " Total Parking Capacity     : " << totalSlots << " slots\n";
        cout << " Occupied Slots             : " << occupied << "\n";
        cout << " Available Slots            : " << getAvailableCount() << "\n";
        cout << " Current Occupancy Rate     : " << fixed << setprecision(1) << occupancyRate << "%\n";
        cout << "--------------------------------------------------------\n";
        cout << " Active 2-Wheelers (Bikes)  : " << bikes << "\n";
        cout << " Active 4-Wheelers (Cars)   : " << cars << "\n";
        cout << " Active Heavy Vehicles      : " << heavies << "\n";
        cout << "--------------------------------------------------------\n";
        cout << " Total Checkouts Serviced   : " << totalVehiclesServiced << "\n";
        cout << " Total Revenue Collected    : Rs. " << fixed << setprecision(2) << totalRevenueCollected << "\n";
        cout << "========================================================\n";
    }

    // ========================================================================
    // FEATURE 7: VIEW TRANSACTION HISTORY / AUDIT LOGS
    // ========================================================================
    void displayHistory() const {
        ifstream histFile(HISTORY_FILE);
        if (!histFile.is_open()) {
            cout << "\n>> No checkout history logs found yet.\n";
            return;
        }

        cout << "\n========================================================\n";
        cout << "                 CHECKOUT BILLING LOGS                  \n";
        cout << "========================================================\n";
        string line;
        bool hasLogs = false;
        while (getline(histFile, line)) {
            cout << line << "\n";
            hasLogs = true;
        }
        histFile.close();

        if (!hasLogs) {
            cout << " No transaction records in file.\n";
        }
        cout << "========================================================\n";
    }
};

// ============================================================================
// MAIN FUNCTION & MENU
// ============================================================================
int main() {
    ParkingSystem system(DEFAULT_TOTAL_SLOTS);
    int choice = 0;

    do {
        cout << "\n========================================================\n";
        cout << "             VEHICLE PARKING MANAGEMENT SYSTEM          \n";
        cout << "========================================================\n";
        cout << " [1]  Park Vehicle (Check-In)\n";
        cout << " [2]  Unpark Vehicle & Collect Bill (Check-Out)\n";
        cout << " [3]  Display All Parked Vehicles\n";
        cout << " [4]  View Parking Lot Grid Map\n";
        cout << " [5]  Search Vehicle (By Reg No / Owner Name)\n";
        cout << " [6]  Parking Statistics & Revenue Report\n";
        cout << " [7]  View Transaction History Logs\n";
        cout << " [8]  Exit System\n";
        cout << "========================================================\n";

        choice = getValidInt(1, 8, "Select option (1-8): ");

        switch (choice) {
            case 1:
                system.parkVehicle();
                pauseConsole();
                break;
            case 2:
                system.unparkVehicle();
                pauseConsole();
                break;
            case 3:
                system.displayParkedVehicles();
                pauseConsole();
                break;
            case 4:
                system.displayParkingGrid();
                pauseConsole();
                break;
            case 5:
                system.searchVehicle();
                pauseConsole();
                break;
            case 6:
                system.displayStatistics();
                pauseConsole();
                break;
            case 7:
                system.displayHistory();
                pauseConsole();
                break;
            case 8:
                cout << "\n========================================================\n";
                cout << " Saving data and exiting. Thank you for using the system!\n";
                cout << "========================================================\n";
                break;
            default:
                cout << "\n>> Invalid choice!\n";
        }

    } while (choice != 8);

    return 0;
}