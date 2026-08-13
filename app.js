// Vehicle Parking Management System - JavaScript Logic
const TOTAL_SLOTS = 20;

// Rates per hour
const RATES = {
    1: { name: "Bike / Scooter", rate: 20 },
    2: { name: "Car / SUV", rate: 50 },
    3: { name: "Bus / Truck", rate: 100 }
};

// Global variables
let parkedVehicles = [];
let totalRevenue = 0;

// Load data from LocalStorage on start
function loadData() {
    let savedVehicles = localStorage.getItem("my_parked_vehicles");
    let savedRevenue = localStorage.getItem("my_total_revenue");

    if (savedVehicles) {
        parkedVehicles = JSON.parse(savedVehicles);
    } else {
        parkedVehicles = [];
    }

    if (savedRevenue) {
        totalRevenue = parseFloat(savedRevenue);
    }

    renderAll();
}

// Save data to LocalStorage
function saveData() {
    localStorage.setItem("my_parked_vehicles", JSON.stringify(parkedVehicles));
    localStorage.setItem("my_total_revenue", totalRevenue.toString());
}

// Find next available slot (1 to 20)
function getNextFreeSlot() {
    for (let slot = 1; slot <= TOTAL_SLOTS; slot++) {
        let isTaken = false;
        for (let i = 0; i < parkedVehicles.length; i++) {
            if (parkedVehicles[i].slotNo === slot) {
                isTaken = true;
                break;
            }
        }
        if (!isTaken) {
            return slot;
        }
    }
    return -1; // Full
}

// Refresh all UI sections
function renderAll() {
    updateStats();
    displaySlots();
    displayTable();
}

// Update the top 4 status boxes
function updateStats() {
    let occupied = parkedVehicles.length;
    let free = TOTAL_SLOTS - occupied;

    document.getElementById("totalSlots").innerText = TOTAL_SLOTS;
    document.getElementById("occupiedSlots").innerText = occupied;
    document.getElementById("freeSlots").innerText = free;
    document.getElementById("totalEarnings").innerText = "Rs " + totalRevenue;
}

// Draw the 20 parking slots
function displaySlots(filter = "") {
    let container = document.getElementById("slotsGrid");
    container.innerHTML = "";

    for (let slot = 1; slot <= TOTAL_SLOTS; slot++) {
        let vehicle = null;
        for (let i = 0; i < parkedVehicles.length; i++) {
            if (parkedVehicles[i].slotNo === slot) {
                vehicle = parkedVehicles[i];
                break;
            }
        }

        let slotDiv = document.createElement("div");
        slotDiv.className = "slot-item " + (vehicle ? "occupied" : "free");

        if (vehicle) {
            // Check if filter matches
            let match = true;
            if (filter !== "") {
                let q = filter.toLowerCase();
                if (!vehicle.vehicleNo.toLowerCase().includes(q) && !vehicle.ownerName.toLowerCase().includes(q)) {
                    match = false;
                }
            }

            slotDiv.style.opacity = match ? "1" : "0.3";
            slotDiv.innerHTML = `
                <div class="slot-title">Slot #${slot}</div>
                <div class="slot-vehicle">${vehicle.vehicleNo}</div>
                <div style="font-size:10px; color:#666;">(Occupied)</div>
            `;
            slotDiv.onclick = function() {
                unparkVehicle(slot);
            };
        } else {
            slotDiv.innerHTML = `
                <div class="slot-title">Slot #${slot}</div>
                <div class="slot-vehicle" style="color:#27ae60;">FREE</div>
            `;
        }

        container.appendChild(slotDiv);
    }
}

// Display the parked vehicles table
function displayTable(filter = "") {
    let tbody = document.getElementById("tableBody");
    tbody.innerHTML = "";

    if (parkedVehicles.length === 0) {
        tbody.innerHTML = `<tr><td colspan="6" style="text-align:center;">No vehicles currently parked.</td></tr>`;
        return;
    }

    let foundAny = false;
    for (let i = 0; i < parkedVehicles.length; i++) {
        let v = parkedVehicles[i];

        if (filter !== "") {
            let q = filter.toLowerCase();
            if (!v.vehicleNo.toLowerCase().includes(q) && !v.ownerName.toLowerCase().includes(q)) {
                continue;
            }
        }

        foundAny = true;
        let vType = RATES[v.type] ? RATES[v.type].name : "Car";
        let dateStr = new Date(v.entryTime).toLocaleTimeString();

        let row = document.createElement("tr");
        row.innerHTML = `
            <td><strong>#${v.slotNo}</strong></td>
            <td>${v.vehicleNo}</td>
            <td>${v.ownerName}</td>
            <td>${vType}</td>
            <td>${dateStr}</td>
            <td>
                <button class="btn btn-danger" onclick="unparkVehicle(${v.slotNo})">Unpark & Bill</button>
            </td>
        `;
        tbody.appendChild(row);
    }

    if (!foundAny) {
        tbody.innerHTML = `<tr><td colspan="6" style="text-align:center;">No vehicle found matching "${filter}".</td></tr>`;
    }
}

// Form Submit: Park a vehicle
document.getElementById("parkingForm").addEventListener("submit", function(e) {
    e.preventDefault();

    if (parkedVehicles.length >= TOTAL_SLOTS) {
        alert("Parking lot is full! All 20 slots are occupied.");
        return;
    }

    let vNo = document.getElementById("vehicleNo").value.trim().toUpperCase();
    let oName = document.getElementById("ownerName").value.trim();
    let vType = parseInt(document.getElementById("vehicleType").value);

    if (!vNo || !oName) {
        alert("Please enter both Vehicle Number and Owner Name!");
        return;
    }

    // Check duplicate
    for (let i = 0; i < parkedVehicles.length; i++) {
        if (parkedVehicles[i].vehicleNo === vNo) {
            alert("Error: Vehicle " + vNo + " is already parked in Slot #" + parkedVehicles[i].slotNo);
            return;
        }
    }

    let slot = getNextFreeSlot();
    if (slot === -1) {
        alert("No free slot available!");
        return;
    }

    let newVehicle = {
        slotNo: slot,
        vehicleNo: vNo,
        ownerName: oName,
        type: vType,
        entryTime: new Date().toISOString()
    };

    parkedVehicles.push(newVehicle);
    saveData();
    renderAll();

    // Reset Form
    document.getElementById("parkingForm").reset();
    document.getElementById("vehicleType").value = "2";

    alert("Vehicle parked successfully in Slot #" + slot + "!");
});

// Unpark Vehicle & Generate Bill
function unparkVehicle(slotNo) {
    let index = -1;
    for (let i = 0; i < parkedVehicles.length; i++) {
        if (parkedVehicles[i].slotNo === slotNo) {
            index = i;
            break;
        }
    }

    if (index === -1) {
        alert("Slot #" + slotNo + " is already free!");
        return;
    }

    let v = parkedVehicles[index];
    let now = new Date();
    let entryDate = new Date(v.entryTime);

    // Calculate hours parked
    let diffMs = now.getTime() - entryDate.getTime();
    let diffMins = Math.floor(diffMs / (1000 * 60));
    let diffHours = Math.ceil(diffMins / 60);
    if (diffHours < 1) diffHours = 1; // Minimum 1 hour charge

    let rate = RATES[v.type] ? RATES[v.type].rate : 50;
    let totalBill = diffHours * rate;

    // Update global revenue
    totalRevenue += totalBill;

    // Remove from array
    parkedVehicles.splice(index, 1);
    saveData();
    renderAll();

    // Show Bill Modal
    showBillModal(v, diffMins, diffHours, rate, totalBill);
}

// Show Bill Receipt Modal
function showBillModal(v, mins, hours, rate, bill) {
    let detailsDiv = document.getElementById("billDetails");
    let vTypeName = RATES[v.type] ? RATES[v.type].name : "Car";

    detailsDiv.innerHTML = `
        <div class="receipt-box">
            <h3>PARKING RECEIPT</h3>
            <div class="receipt-row">
                <span>Slot Number:</span>
                <strong>Slot #${v.slotNo}</strong>
            </div>
            <div class="receipt-row">
                <span>Vehicle No:</span>
                <strong>${v.vehicleNo}</strong>
            </div>
            <div class="receipt-row">
                <span>Owner Name:</span>
                <strong>${v.ownerName}</strong>
            </div>
            <div class="receipt-row">
                <span>Vehicle Type:</span>
                <strong>${vTypeName}</strong>
            </div>
            <div class="receipt-row">
                <span>Time Parked:</span>
                <strong>${mins} mins (${hours} hr billed)</strong>
            </div>
            <div class="receipt-row">
                <span>Rate per Hour:</span>
                <strong>Rs ${rate}</strong>
            </div>
            <div class="receipt-row receipt-total">
                <span>TOTAL AMOUNT:</span>
                <span>Rs ${bill}</span>
            </div>
        </div>
    `;

    document.getElementById("billModal").style.display = "block";
}

// Close Modal
function closeModal() {
    document.getElementById("billModal").style.display = "none";
}

// Search input event
document.getElementById("searchInput").addEventListener("input", function(e) {
    let q = e.target.value.trim();
    displaySlots(q);
    displayTable(q);
});

// Live Digital Clock
function updateClock() {
    let clockElement = document.getElementById("liveClock");
    if (clockElement) {
        let now = new Date();
        clockElement.innerText = now.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit', second: '2-digit', hour12: true });
    }
}

// Run when page loads
window.onload = function() {
    loadData();
    updateClock(); // Initial call
    setInterval(updateClock, 1000); // Update every second
};
