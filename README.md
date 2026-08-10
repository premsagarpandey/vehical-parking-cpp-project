# Vehicle Parking Management System

A simple and clean **Vehicle Parking Management System** mini-project created with **C++** and **HTML/CSS/JavaScript**.

This project helps manage a parking lot with 20 slots. It allows parking vehicles, calculating bills during check-out based on hourly rates, viewing active parked vehicles, and checking total earnings.

---

## 📌 Features

- **Park Vehicle**: Enter vehicle registration number, owner name, and vehicle type (Bike, Car, or Bus/Truck). Automatically assigns an empty slot.
- **Unpark & Billing**: Calculates parking duration and calculates the bill according to vehicle type.
- **20 Slots Management**: Real-time status of all 20 parking slots (Occupied vs Free).
- **Search Vehicle**: Search parked vehicles by registration number or owner name.
- **Report & Earnings**: View total vehicles parked and total money collected.
- **Data Persistence**: 
  - In **C++**: Saves data in `parking_data.txt`.
  - In **Web**: Saves data in browser `localStorage`.

---

## 💰 Parking Rates (Tariff)

| Vehicle Type | Rate / Hour |
| :--- | :--- |
| **Bike / Scooter** (2-Wheeler) | Rs 20 / hour |
| **Car / SUV** (4-Wheeler) | Rs 50 / hour |
| **Bus / Truck** (Heavy Vehicle) | Rs 100 / hour |

---

## 🚀 How to Run

### 1. Web Version (HTML / CSS / JS)
Simply double click and open **`index.html`** in any web browser (Chrome, Edge, Firefox, etc.). No setup required!

### 2. C++ Console Program
Compile and run `main.cpp` using any C++ compiler:

```bash
# Compile
g++ main.cpp -o main.exe

# Run (Windows)
.\main.exe

# Run (Linux / Mac)
./main
```

---

## 📁 Files in Project

- `main.cpp` - Complete C++ source code with menu, structs, and file handling.
- `index.html` - Web interface layout and structure.
- `style.css` - Simple and clean styling.
- `app.js` - JavaScript logic for parking, billing, and storage.
- `README.md` - Project documentation.

---

## 🎓 Technology Used

- **C++** (Standard Library, File I/O, Vectors, Structs)
- **HTML5 & CSS3**
- **JavaScript** (DOM Manipulation, LocalStorage)
