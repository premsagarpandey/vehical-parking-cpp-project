// Vehicle Parking Management System - JavaScript Logic
const TOTAL_SLOTS = 20;

// Rates per hour
const RATES = {
    1: { name: "Bike / Scooter (2-Wheeler)", rate: 20 },
    2: { name: "Car / SUV (4-Wheeler)", rate: 50 },
    3: { name: "Bus / Truck (Heavy)", rate: 100 }
};

// Global application state
let parkedVehicles = [];
let totalRevenue = 0.0;
let totalServiced = 0;

// Helper: Escape HTML to prevent XSS
function escapeHtml(str) {
    if (!str) return "";
    return String(str)
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}

// Helper: Format ISO date string into readable Date & Time
function formatDateTime(dateInput) {
    let d = new Date(dateInput);
    if (isNaN(d.getTime())) return "N/A";
    let datePart = d.toLocaleDateString("en-GB", { day: "2-digit", month: "short", year: "numeric" });
    let timePart = d.toLocaleTimeString("en-US", { hour: "2-digit", minute: "2-digit", hour12: true });
    return `${datePart} ${timePart}`;
}

// Load data from LocalStorage on start
function loadData() {
    try {
        let savedVehicles = localStorage.getItem("my_parked_vehicles");
        let savedRevenue = localStorage.getItem("my_total_revenue");
        let savedServiced = localStorage.getItem("my_total_serviced");

        if (savedVehicles) {
            parkedVehicles = JSON.parse(savedVehicles) || [];
        } else {
            parkedVehicles = [];
        }

        if (savedRevenue) {
            let rev = parseFloat(savedRevenue);
            totalRevenue = isNaN(rev) ? 0.0 : rev;
        } else {
            totalRevenue = 0.0;
        }

        if (savedServiced) {
            let srv = parseInt(savedServiced, 10);
            totalServiced = isNaN(srv) ? 0 : srv;
        } else {
            totalServiced = 0;
        }
    } catch (err) {
        console.error("Error reading localStorage:", err);
        parkedVehicles = [];
        totalRevenue = 0.0;
        totalServiced = 0;
    }

    renderAll();
}

// Save data to LocalStorage
function saveData() {
    try {
        localStorage.setItem("my_parked_vehicles", JSON.stringify(parkedVehicles));
        localStorage.setItem("my_total_revenue", totalRevenue.toFixed(2));
        localStorage.setItem("my_total_serviced", totalServiced.toString());
    } catch (err) {
        console.error("Error saving to localStorage:", err);
    }
}

// Find next available slot (1 to 20)
function getNextFreeSlot() {
    for (let slot = 1; slot <= TOTAL_SLOTS; slot++) {
        let isTaken = parkedVehicles.some(v => v.slotNo === slot);
        if (!isTaken) {
            return slot;
        }
    }
    return -1; // Parking full
}

// Refresh all UI sections
function renderAll() {
    let searchVal = document.getElementById("searchInput").value.trim();
    updateStats();
    displaySlots(searchVal);
    displayTable(searchVal);
}

// Update the top status boxes
function updateStats() {
    let occupied = parkedVehicles.length;
    let free = TOTAL_SLOTS - occupied;

    document.getElementById("totalSlots").innerText = TOTAL_SLOTS;
    document.getElementById("occupiedSlots").innerText = occupied;
    document.getElementById("freeSlots").innerText = free;
    document.getElementById("totalServiced").innerText = totalServiced;
    document.getElementById("totalEarnings").innerText = "Rs " + totalRevenue.toFixed(2);
}

// Draw the 20 parking slots
function displaySlots(filter = "") {
    let container = document.getElementById("slotsGrid");
    container.innerHTML = "";
    let q = filter.toLowerCase();

    for (let slot = 1; slot <= TOTAL_SLOTS; slot++) {
        let vehicle = parkedVehicles.find(v => v.slotNo === slot);

        let slotDiv = document.createElement("div");
        slotDiv.className = "slot-item " + (vehicle ? "occupied" : "free");

        if (vehicle) {
            let match = true;
            if (q !== "") {
                let vNo = vehicle.vehicleNo.toLowerCase();
                let oName = vehicle.ownerName.toLowerCase();
                match = vNo.includes(q) || oName.includes(q);
            }

            slotDiv.style.opacity = (q === "" || match) ? "1" : "0.2";
            if (q !== "" && match) {
                slotDiv.style.transform = "scale(1.04)";
                slotDiv.style.boxShadow = "0 0 10px rgba(231, 76, 60, 0.6)";
            }

            slotDiv.innerHTML = `
                <div class="slot-title">Slot #${slot}</div>
                <div class="slot-vehicle">${escapeHtml(vehicle.vehicleNo)}</div>
                <div style="font-size:10px; color:#c0392b; font-weight:600; margin-top:2px;">[Occupied]</div>
            `;
            slotDiv.title = `Slot #${slot} - ${vehicle.vehicleNo} (${vehicle.ownerName}). Click to unpark.`;
            slotDiv.onclick = function() {
                unparkVehicle(slot);
            };
        } else {
            slotDiv.style.opacity = q !== "" ? "0.3" : "1";
            slotDiv.innerHTML = `
                <div class="slot-title">Slot #${slot}</div>
                <div class="slot-vehicle" style="color:#27ae60;">FREE</div>
                <div style="font-size:10px; color:#27ae60; margin-top:2px;">Available</div>
            `;
            slotDiv.title = `Slot #${slot} is available`;
        }

        container.appendChild(slotDiv);
    }
}

// Display the parked vehicles table sorted by slot number
function displayTable(filter = "") {
    let tbody = document.getElementById("tableBody");
    tbody.innerHTML = "";
    let q = filter.toLowerCase();

    if (parkedVehicles.length === 0) {
        tbody.innerHTML = `<tr><td colspan="6" style="text-align:center; padding: 18px; color: #7f8c8d;">No vehicles currently parked in the lot.</td></tr>`;
        return;
    }

    // Sort parked list by slot number
    let sortedList = [...parkedVehicles].sort((a, b) => a.slotNo - b.slotNo);
    let foundAny = false;

    for (let i = 0; i < sortedList.length; i++) {
        let v = sortedList[i];

        if (q !== "") {
            let vNo = v.vehicleNo.toLowerCase();
            let oName = v.ownerName.toLowerCase();
            if (!vNo.includes(q) && !oName.includes(q)) {
                continue;
            }
        }

        foundAny = true;
        let vType = RATES[v.type] ? RATES[v.type].name : "Car (4-Wheeler)";
        let dateStr = formatDateTime(v.entryTime);

        let row = document.createElement("tr");
        row.innerHTML = `
            <td><strong>#${v.slotNo}</strong></td>
            <td><strong>${escapeHtml(v.vehicleNo)}</strong></td>
            <td>${escapeHtml(v.ownerName)}</td>
            <td>${escapeHtml(vType)}</td>
            <td>${escapeHtml(dateStr)}</td>
            <td>
                <button type="button" class="btn btn-danger" onclick="unparkVehicle(${v.slotNo})">Unpark & Bill</button>
            </td>
        `;
        tbody.appendChild(row);
    }

    if (!foundAny) {
        tbody.innerHTML = `<tr><td colspan="6" style="text-align:center; padding: 18px; color: #e74c3c;">No vehicle found matching "${escapeHtml(filter)}".</td></tr>`;
    }
}

// Form Submit: Park a vehicle
document.getElementById("parkingForm").addEventListener("submit", function(e) {
    e.preventDefault();

    if (parkedVehicles.length >= TOTAL_SLOTS) {
        alert("Parking lot is completely full! All 20 slots are occupied.");
        return;
    }

    let vNoInput = document.getElementById("vehicleNo");
    let oNameInput = document.getElementById("ownerName");
    let vTypeInput = document.getElementById("vehicleType");

    let vNo = vNoInput.value.trim().toUpperCase();
    let oName = oNameInput.value.trim();
    let vType = parseInt(vTypeInput.value, 10);

    if (!vNo || !oName) {
        alert("Please enter both Vehicle Registration Number and Owner Name!");
        return;
    }

    // Check duplicate registration number
    let duplicate = parkedVehicles.find(v => v.vehicleNo === vNo);
    if (duplicate) {
        alert(`Error: Vehicle "${vNo}" is already parked in Slot #${duplicate.slotNo}!`);
        return;
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
    vTypeInput.value = "2";

    alert(`Vehicle "${vNo}" parked successfully in Slot #${slot}!`);
});

// Unpark Vehicle & Generate Bill
function unparkVehicle(slotNo) {
    let index = parkedVehicles.findIndex(v => v.slotNo === slotNo);

    if (index === -1) {
        alert("Slot #" + slotNo + " is already free!");
        return;
    }

    let v = parkedVehicles[index];

    // Confirmation prompt to avoid accidental unparking
    let confirmed = confirm(`Are you sure you want to unpark vehicle ${v.vehicleNo} (Owner: ${v.ownerName}) from Slot #${v.slotNo}?`);
    if (!confirmed) {
        return;
    }

    let now = new Date();
    let entryDate = new Date(v.entryTime);

    // Calculate hours parked
    let diffMs = now.getTime() - entryDate.getTime();
    if (isNaN(diffMs) || diffMs < 0) diffMs = 0;

    let diffMins = Math.floor(diffMs / (1000 * 60));
    let diffHours = Math.ceil(diffMins / 60);
    if (diffHours < 1) diffHours = 1; // Minimum 1 hour charge

    let rate = RATES[v.type] ? RATES[v.type].rate : 50;
    let totalBill = diffHours * rate;

    // Update global statistics
    totalServiced++;
    totalRevenue += totalBill;

    // Remove from array
    parkedVehicles.splice(index, 1);
    saveData();
    renderAll();

    // Show Bill Modal
    showBillModal(v, diffMins, diffHours, rate, totalBill, entryDate, now);
}

// Show Bill Receipt Modal
function showBillModal(v, mins, hours, rate, bill, entryDate, exitDate) {
    let detailsDiv = document.getElementById("billDetails");
    let vTypeName = RATES[v.type] ? RATES[v.type].name : "Car";
    let formattedEntry = formatDateTime(entryDate);
    let formattedExit = formatDateTime(exitDate);

    detailsDiv.innerHTML = `
        <div class="receipt-box">
            <h3>PARKING RECEIPT / BILL</h3>
            <div class="receipt-row">
                <span>Slot Number:</span>
                <strong>Slot #${v.slotNo}</strong>
            </div>
            <div class="receipt-row">
                <span>Vehicle No:</span>
                <strong>${escapeHtml(v.vehicleNo)}</strong>
            </div>
            <div class="receipt-row">
                <span>Owner Name:</span>
                <strong>${escapeHtml(v.ownerName)}</strong>
            </div>
            <div class="receipt-row">
                <span>Vehicle Type:</span>
                <strong>${escapeHtml(vTypeName)}</strong>
            </div>
            <div class="receipt-row">
                <span>Entry Time:</span>
                <span>${escapeHtml(formattedEntry)}</span>
            </div>
            <div class="receipt-row">
                <span>Exit Time:</span>
                <span>${escapeHtml(formattedExit)}</span>
            </div>
            <div class="receipt-row">
                <span>Duration:</span>
                <strong>${mins} mins (${hours} ${hours === 1 ? 'hr' : 'hrs'} billed)</strong>
            </div>
            <div class="receipt-row">
                <span>Rate per Hour:</span>
                <strong>Rs ${rate}.00</strong>
            </div>
            <div class="receipt-row receipt-total">
                <span>TOTAL AMOUNT:</span>
                <span>Rs ${bill.toFixed(2)}</span>
            </div>
            <div style="text-align:center; font-size:11px; color:#7f8c8d; margin-top:8px;">
                Status: PAID (Cash / UPI) • Thank you!
            </div>
        </div>
    `;

    document.getElementById("billModal").style.display = "block";
}

// Close Modal
function closeModal() {
    document.getElementById("billModal").style.display = "none";
}

// Clear All Records (matches Option 7 of C++)
function clearAllRecords() {
    let confirmClear = confirm("WARNING: Are you sure you want to clear ALL parking records?\n\nThis will remove all currently parked vehicles, reset total revenue, and reset total serviced count.");
    if (!confirmClear) return;

    parkedVehicles = [];
    totalRevenue = 0.0;
    totalServiced = 0;
    saveData();
    renderAll();
    alert("All parking records have been cleared successfully! Fresh start initialized.");
}

// Search input event
document.getElementById("searchInput").addEventListener("input", function(e) {
    let q = e.target.value.trim();
    displaySlots(q);
    displayTable(q);
});

// Live Digital Clock (shows date & time)
function updateClock() {
    let clockElement = document.getElementById("liveClock");
    if (clockElement) {
        let now = new Date();
        let datePart = now.toLocaleDateString("en-GB", { weekday: "short", day: "2-digit", month: "short", year: "numeric" });
        let timePart = now.toLocaleTimeString("en-US", { hour: "2-digit", minute: "2-digit", second: "2-digit", hour12: true });
        clockElement.innerText = `${datePart} | ${timePart}`;
    }
}

// Close modal when clicking outside of it
window.onclick = function(event) {
    let modal = document.getElementById("billModal");
    if (event.target === modal) {
        closeModal();
    }
};

// Close modal on Escape key
window.onkeydown = function(event) {
    if (event.key === "Escape") {
        closeModal();
    }
};

// Run when page loads
window.onload = function() {
    loadData();
    updateClock();
    setInterval(updateClock, 1000);
};
