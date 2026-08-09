# 🚗 Vehicle Parking Management System (C++)

A modern, object-oriented, console-based **Vehicle Parking Management System** built with C++ (C++11/C++14). It provides comprehensive parking lot management including vehicle check-in with entry slips, automated checkout with dynamic duration-based billing, visual 2D parking lot mapping, persistent file storage, vehicle search, and revenue analytics.

---

## 🌟 Key Features

- **Object-Oriented Architecture (OOP)**: Clean, modular design with `Vehicle`, `ParkingSpot`, and `ParkingSystem` classes.
- **Vehicle Check-In (Entry)**:
  - Assigns next available slot.
  - Supports alphanumeric registration numbers (e.g. `DL 01 AB 1234`) and full names with spaces.
  - Duplicate vehicle parking detection.
  - Generates and prints timestamped **Entry Parking Slips**.
- **Vehicle Check-Out & Automated Billing (Exit)**:
  - Check out by **Slot Number** or **Vehicle Registration Number**.
  - Calculates duration and billing based on hourly rates:
    - **2-Wheeler (Bike/Scooter)**: ₹20 / hour
    - **4-Wheeler (Car/SUV)**: ₹50 / hour
    - **Heavy Vehicle (Bus/Truck)**: ₹100 / hour
  - Generates detailed **Invoices / Receipts** with invoice IDs, timestamps, and fees.
- **Visual Parking Grid Map**:
  - Interactive 2D ASCII grid displaying real-time occupancy of all 20 slots (`[FREE]` vs `Vehicle Number`).
- **Data Persistence (File Handling)**:
  - `parking_records.txt`: Stores active parked vehicles automatically.
  - `parking_history.txt`: Maintains an audit trail of all checkout transactions.
  - `parking_stats.txt`: Tracks cumulative revenue and vehicle counts.
- **Search & Analytics Dashboard**:
  - Search parked vehicles by registration number or owner name.
  - Real-time statistics: occupancy rate (%), vehicle breakdown by category, serviced count, and total revenue.
- **Crash-Proof Input Validation**:
  - Protected input streams prevent crashes or infinite loops when invalid characters or menu choices are entered.

---

## 🏗️ Project Structure

```
.
├── main.cpp              # Complete C++ Source Code
├── README.md             # Documentation
├── parking_records.txt   # Active parking records (auto-generated)
├── parking_history.txt   # Transaction & billing audit logs (auto-generated)
└── parking_stats.txt     # Cumulative revenue & statistics (auto-generated)
```

---

## 🚀 Getting Started

### Prerequisites
- Any C++ compiler supporting C++11 or later (`g++`, `clang++`, or MSVC).

### Compilation
Using `g++`:
```bash
g++ -std=c++11 -Wall -Wextra main.cpp -o main.exe
```

### Running the Application
```bash
./main.exe
```

---

## 🖥️ Menu Options

```
========================================================
             VEHICLE PARKING MANAGEMENT SYSTEM          
========================================================
 [1]  Park Vehicle (Check-In)
 [2]  Unpark Vehicle & Collect Bill (Check-Out)
 [3]  Display All Parked Vehicles
 [4]  View Parking Lot Grid Map
 [5]  Search Vehicle (By Reg No / Owner Name)
 [6]  Parking Statistics & Revenue Report
 [7]  View Transaction History Logs
 [8]  Exit System
========================================================
```

---

## 📄 License
This project is open source and available under the [MIT License](LICENSE).
