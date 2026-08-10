# 🚗 Smart Vehicle Parking Management System

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-11%20%7C%2014%20%7C%2017%20%7C%2020-00599C?style=for-the-badge&logo=c%2B%2B)](https://en.cppreference.com/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-blue?style=for-the-badge)](https://github.com/premsagarpandey/vehical-parking-cpp-project)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](LICENSE)
[![Maintenance](https://img.shields.io/badge/Maintained%3F-yes-green.svg?style=for-the-badge)](https://github.com/premsagarpandey/vehical-parking-cpp-project)

A high-performance, object-oriented, console-based **Vehicle Parking Management System** developed in **C++**. Designed for parking lots and commercial garages, it features automated check-in with digital slips, dynamic duration-based billing, a 2D interactive ASCII parking grid map, persistent file storage, and real-time revenue analytics.

---

## ✨ Key Features

- **🚗 Multi-Category Vehicle Support**: Handles 2-Wheelers (Bikes/Scooters), 4-Wheelers (Cars/SUVs), and Heavy Vehicles (Buses/Trucks).
- **🎫 Automated Check-In & Entry Slips**: Automatically finds the nearest free slot, assigns spot IDs, prevents duplicate registrations, and generates a formatted entry slip.
- **💳 Smart Checkout & Billing Engine**: Computes exact parking duration (hours/mins) and calculates tiered hourly pricing with printed invoices.
- **🗺️ Visual 2D Lot Map**: Real-time ASCII grid visualization of occupied vs. free slots (`[FREE]` vs `[DL-01-AB-1234]`).
- **💾 Automatic Data Persistence**: Preserves active vehicles, historical checkout audits, and lifetime revenue metrics across app restarts using local flat files.
- **🔍 Fast Search & Lookup**: Instant vehicle search by license plate number or owner's name.
- **📈 Analytics Dashboard**: Live summary of lot occupancy rate, vehicle type breakdown, total vehicles served, and collected revenue.
- **🛡️ Crash-Proof Input Validation**: Safe stream parsing protects against invalid user inputs and unexpected crashes.

---

## 💰 Tariff Rates

| Vehicle Category | Supported Types | Hourly Rate |
| :--- | :--- | :--- |
| **2-Wheeler** | Bike, Scooter, Motorcycle | **₹20 / hr** |
| **4-Wheeler** | Car, Sedan, SUV, Van | **₹50 / hr** |
| **Heavy Vehicle** | Bus, Truck, Commercial Carrier | **₹100 / hr** |

---

## 🖥️ Preview & Interface

### 📌 Main Menu
```text
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

### 🗺️ Visual Parking Grid
```text
+---------------------------------------------------------------------------------------+
|                               PARKING LOT REAL-TIME MAP                               |
+---------------------------------------------------------------------------------------+
|  Slot 01: [DL01AB1234]  |  Slot 02: [FREE]        |  Slot 03: [MH12CD5678]  |  Slot 04: [FREE]        |
|  Slot 05: [FREE]        |  Slot 06: [FREE]        |  Slot 07: [KA05EF9012]  |  Slot 08: [FREE]        |
|  Slot 09: [FREE]        |  Slot 10: [FREE]        |  Slot 11: [FREE]        |  Slot 12: [FREE]        |
|  Slot 13: [FREE]        |  Slot 14: [FREE]        |  Slot 15: [FREE]        |  Slot 16: [FREE]        |
|  Slot 17: [FREE]        |  Slot 18: [FREE]        |  Slot 19: [FREE]        |  Slot 20: [FREE]        |
+---------------------------------------------------------------------------------------+
```

### 🧾 Check-Out Invoice Sample
```text
========================================================
               PARKING CHECK-OUT INVOICE               
========================================================
 Invoice ID     : INV-1723301824
 Slot Number    : 3
 Reg Number     : DL 01 AB 1234
 Owner Name     : John Doe
 Vehicle Type   : 4-Wheeler (Car/SUV)
 Entry Time     : 2026-08-10 17:30:00
 Exit Time      : 2026-08-10 19:30:00
 Duration       : 2.00 hours (120 mins)
 Rate Applied   : ₹50.00 / hour
--------------------------------------------------------
 TOTAL AMOUNT   : ₹100.00
 Status         : PAID
========================================================
```

---

## 🚀 Getting Started

### Prerequisites
- Any C++ compiler supporting **C++11 or higher** (`g++`, `clang++`, or `MSVC`).

### 📦 Installation & Setup

1. **Clone the Repository**
   ```bash
   git clone https://github.com/premsagarpandey/vehical-parking-cpp-project.git
   cd vehical-parking-cpp-project
   ```

2. **Compile the Code**
   - **GCC / MinGW / Clang**:
     ```bash
     g++ -std=c++11 -O2 -Wall main.cpp -o parking_system
     ```
   - **MSVC (Windows Developer Command Prompt)**:
     ```cmd
     cl /EHsc /std:c++14 main.cpp /Fe:parking_system.exe
     ```

3. **Run the Executable**
   - **Windows**:
     ```powershell
     .\parking_system.exe
     ```
   - **Linux / macOS**:
     ```bash
     ./parking_system
     ```

---

## 🏗️ Project Architecture

```
.
├── main.cpp              # Core source code (OOP classes, logic & UI)
├── README.md             # Documentation
├── parking_records.txt   # Active parking records (auto-created on run)
├── parking_history.txt   # Audit trail of checkout transactions
└── parking_stats.txt     # Cumulative revenue & stats state
```

### Core OOP Classes
- `Vehicle`: Encapsulates vehicle attributes (registration number, owner name, vehicle category, entry timestamp).
- `ParkingSpot`: Represents individual lot slots, status tracking, and vehicle association.
- `ParkingSystem`: Orchestrates parking operations, search queries, dynamic billing calculations, real-time grid generation, and file I/O.

---

## 🤝 Contributing

Contributions, issues, and feature requests are welcome!
Feel free to check the [issues page](https://github.com/premsagarpandey/vehical-parking-cpp-project/issues).

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

---

## 📄 License

This project is open source and available under the [MIT License](LICENSE).
