# 🚗 Smart Vehicle Parking Management System

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-11%20%7C%2014%20%7C%2017%20%7C%2020-00599C?style=for-the-badge&logo=c%2B%2B)](https://en.cppreference.com/)
[![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)](https://developer.mozilla.org/en-US/docs/Web/HTML)
[![CSS3](https://img.shields.io/badge/CSS3-1572B6?style=for-the-badge&logo=css3&logoColor=white)](https://developer.mozilla.org/en-US/docs/Web/CSS)
[![JavaScript](https://img.shields.io/badge/JavaScript-ES6%2B-F7DF1E?style=for-the-badge&logo=javascript&logoColor=black)](https://developer.mozilla.org/en-US/docs/Web/JavaScript)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge)](LICENSE)

A complete **Vehicle Parking Management System** featuring both a high-performance **C++ Console Engine** and a clean, beginner-friendly **Interactive Web Interface (HTML5, CSS3, & Vanilla JavaScript)**.

Designed for parking lots and commercial garages, it features automated check-in with digital entry slips, dynamic duration-based billing, a 2D interactive parking grid map, persistent storage, and real-time revenue analytics.

---

## ✨ Key Features

- **🚗 Multi-Category Vehicle Support**: Handles 2-Wheelers (Bikes/Scooters), 4-Wheelers (Cars/SUVs), and Heavy Vehicles (Buses/Trucks).
- **🎫 Automated Check-In & Entry Slips**: Automatically finds the nearest free slot, assigns spot IDs, prevents duplicate registrations, and generates a formatted entry slip.
- **💳 Smart Checkout & Billing Engine**: Computes exact parking duration (hours/mins) and calculates tiered hourly pricing with printed receipts.
- **🗺️ Interactive 2D Lot Map**: Real-time visualization of occupied vs. free slots (`[FREE]` vs `[Occupied]`).
- **🌐 Beginner-Friendly Web UI**: Open `index.html` in any browser — zero build tools or dependencies needed!
- **💾 Dual Persistence**:
  - **C++ Engine**: Auto-saves to local flat files (`parking_records.txt`, `parking_history.txt`, `parking_stats.txt`).
  - **Web Frontend**: Uses browser `localStorage` to keep data persistent across page reloads.
- **🔍 Fast Search & Lookup**: Instant vehicle search by license plate number or owner's name.
- **📈 Analytics Dashboard**: Live summary of lot occupancy rate, vehicle type breakdown, total vehicles served, and collected revenue.

---

## 💰 Tariff Rates

| Vehicle Category | Supported Types | Hourly Rate |
| :--- | :--- | :--- |
| **2-Wheeler** | Bike, Scooter, Motorcycle | **₹20 / hr** |
| **4-Wheeler** | Car, Sedan, SUV, Van | **₹50 / hr** |
| **Heavy Vehicle** | Bus, Truck, Commercial Carrier | **₹100 / hr** |

---

## 🚀 Quick Start Guide

### 🌐 Option 1: Run the Web Interface (Beginner-Friendly)
No compilation or server setup is required!
1. Clone or download the repository:
   ```bash
   git clone https://github.com/premsagarpandey/vehical-parking-cpp-project.git
   cd vehical-parking-cpp-project
   ```
2. Double-click **`index.html`** or open it with any web browser (Chrome, Edge, Firefox, Safari).

---

### ⚙️ Option 2: Run the C++ Console Engine
**Prerequisites**: Any C++ compiler supporting **C++11 or higher** (`g++`, `clang++`, or `MSVC`).

1. **Compile the Code**:
   - **GCC / MinGW / Clang**:
     ```bash
     g++ -std=c++11 -O2 -Wall main.cpp -o parking_system
     ```
   - **MSVC (Windows Developer Command Prompt)**:
     ```cmd
     cl /EHsc /std:c++14 main.cpp /Fe:parking_system.exe
     ```

2. **Run the Executable**:
   - **Windows**:
     ```powershell
     .\parking_system.exe
     ```
   - **Linux / macOS**:
     ```bash
     ./parking_system
     ```

---

## 🖥️ Terminal Interface Preview

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

### 🧾 Check-Out Invoice Sample
```text
========================================================
               PARKING CHECK-OUT INVOICE               
========================================================
 Invoice ID     : INV-1723301824
 Slot Number    : 3
 Reg Number     : DL 01 AB 1234
 Owner Name     : Rahul Sharma
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

## 🏗️ Project Structure

```
.
├── index.html            # Beginner-friendly Web UI Structure
├── style.css             # Modern, responsive styling system
├── app.js                # Frontend logic & LocalStorage persistence
├── main.cpp              # C++ Engine (OOP classes, file I/O & logic)
├── README.md             # Documentation
├── parking_records.txt   # Active parking records (C++ auto-generated)
├── parking_history.txt   # Audit trail of transactions (C++ auto-generated)
└── parking_stats.txt     # Cumulative revenue & stats (C++ auto-generated)
```

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
