# 🚗 Vehicle Parking Management System

A robust and clean **Vehicle Parking Management System** mini-project developed in **C++** (Console Application) and **HTML5 / CSS3 / Modern JavaScript** (Web Application).

This project manages a parking lot of 20 slots with automatic slot allocation, hourly tariff billing, real-time slot visualization, receipt printing, and persistent data storage.

---

## 📌 Features

- **Park Vehicle**: Enter vehicle registration number, owner name, and category (Bike, Car, or Bus/Truck). Automatically detects and assigns the first available parking slot with duplicate prevention.
- **Unpark & Billing**: Computes parking duration from entry timestamp and calculates billing amounts based on hourly vehicle rates (minimum 1-hour charge).
- **Interactive 20-Slot Grid**: Real-time visual status of all 20 parking slots (Available vs Occupied) with direct click-to-unpark confirmation.
- **Live Search**: Instant case-insensitive search by registration number or owner name with matching slot highlights.
- **Reports & Statistics**: Tracks total vehicles serviced, currently occupied slots, available slots, and total revenue collected.
- **Receipt Modal & Printable Voucher**: Pop-up receipt with entry/exit timestamps, duration, rate, and total bill, formatted with dedicated CSS print styles (`@media print`).
- **Live Digital Clock**: Real-time day, date, and digital clock display in the header.
- **Clear All Records**: Reset and initialize a fresh parking state with confirmation dialogs in both C++ and Web versions.
- **Data Persistence**:
  - **C++**: Saves and restores state automatically from `parking_data.txt`.
  - **Web**: Saves state in browser `localStorage`.
- **Fault-Tolerant Input Handling**: C++ CLI uses safe stream input routines to prevent infinite loops, buffer desync, and invalid type crashes.

---

## 💰 Parking Rates (Tariff)

| Vehicle Type | Rate / Hour | Minimum Charge |
| :--- | :--- | :--- |
| **Bike / Scooter** (2-Wheeler) | Rs 20 / hour | Rs 20.00 (1 hr) |
| **Car / SUV** (4-Wheeler) | Rs 50 / hour | Rs 50.00 (1 hr) |
| **Bus / Truck** (Heavy Vehicle) | Rs 100 / hour | Rs 100.00 (1 hr) |

---

## 🚀 How to Run

### 1. Web Version (HTML / CSS / JS)
Simply double-click or open **`index.html`** in any modern web browser (Chrome, Edge, Firefox, Safari). No installation or server required!

### 2. C++ Console Program
Compile and execute `main.cpp` using any standard C++ compiler (GCC / Clang / MSVC):

```bash
# Compile with GCC / G++
g++ -Wall -Wextra main.cpp -o main.exe

# Run on Windows
.\main.exe

# Run on Linux / macOS
g++ -std=c++11 main.cpp -o main
./main
```

---

## 📁 Project Structure

```text
├── index.html       # Web application structure, status cards & modal
├── style.css        # Responsive styling, slot cards, and print styles
├── app.js           # Client-side logic, localStorage handling, search & clock
├── main.cpp         # Complete C++ source code with file handling & safe I/O
└── README.md        # Project overview and documentation
```

---

## 🎓 Technologies Used

- **C++**: Vectors, Structs, File I/O (`fstream`), `<ctime>`, `<iomanip>`, `<algorithm>`
- **HTML5 & CSS3**: Responsive CSS Grid / Flexbox, Print Media Queries, Modal Dialogs
- **JavaScript (ES6+)**: DOM Manipulation, Event Handling, LocalStorage API, Date API
