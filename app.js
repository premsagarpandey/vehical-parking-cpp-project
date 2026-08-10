/**
 * ============================================================================
 * PARKEASY - SMART VEHICLE PARKING MANAGEMENT SYSTEM
 * Clean, Object-Oriented, Beginner-Friendly Vanilla JavaScript Logic
 * ============================================================================
 */

// --- 1. CONSTANTS & SYSTEM CONFIGURATION ---
const TOTAL_SLOTS = 20;

const VEHICLE_RATES = {
    1: { name: '2-Wheeler (Bike/Scooter)', shortName: 'Bike', icon: '🏍️', rate: 20.0 },
    2: { name: '4-Wheeler (Car/SUV)', shortName: 'Car', icon: '🚗', rate: 50.0 },
    3: { name: 'Heavy Vehicle (Bus/Truck)', shortName: 'Heavy', icon: '🚛', rate: 100.0 }
};

const STORAGE_KEYS = {
    RECORDS: 'parkeasy_records',
    HISTORY: 'parkeasy_history',
    STATS: 'parkeasy_stats'
};

// --- 2. APPLICATION STATE ---
let parkingSpots = []; // Array of length TOTAL_SLOTS (1 to 20)
let transactionHistory = [];
let parkingStats = {
    totalServed: 0,
    totalRevenue: 0.0
};

// --- 3. HELPER UTILITIES ---
function formatCurrency(amount) {
    return `₹${parseFloat(amount).toFixed(2)}`;
}

function formatDateTime(dateObj) {
    const d = new Date(dateObj);
    const year = d.getFullYear();
    const month = String(d.getMonth() + 1).padStart(2, '0');
    const day = String(d.getDate()).padStart(2, '0');
    const hours = String(d.getHours()).padStart(2, '0');
    const minutes = String(d.getMinutes()).padStart(2, '0');
    const seconds = String(d.getSeconds()).padStart(2, '0');
    return `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;
}

function calculateDuration(entryTime, exitTime = new Date()) {
    const start = new Date(entryTime).getTime();
    const end = new Date(exitTime).getTime();
    const diffMs = Math.max(0, end - start);
    
    const totalMinutes = Math.floor(diffMs / (1000 * 60));
    const totalHours = Math.max(1, Math.ceil(totalMinutes / 60)); // Minimum 1 hour charge
    const preciseHours = (diffMs / (1000 * 60 * 60)).toFixed(2);
    
    return {
        minutes: totalMinutes,
        billedHours: totalHours,
        exactHours: preciseHours,
        displayTime: totalMinutes < 60 ? `${totalMinutes} mins` : `${Math.floor(totalMinutes / 60)}h ${totalMinutes % 60}m`
    };
}

// --- 4. PERSISTENCE (LOCAL STORAGE) ---
function loadDataFromStorage() {
    try {
        const savedSpots = localStorage.getItem(STORAGE_KEYS.RECORDS);
        const savedHistory = localStorage.getItem(STORAGE_KEYS.HISTORY);
        const savedStats = localStorage.getItem(STORAGE_KEYS.STATS);

        if (savedSpots) {
            parkingSpots = JSON.parse(savedSpots);
        } else {
            // Initialize 20 empty slots
            parkingSpots = Array.from({ length: TOTAL_SLOTS }, (_, index) => ({
                slotNumber: index + 1,
                isOccupied: false,
                vehicle: null
            }));
            loadDemoData();
        }

        if (savedHistory) {
            transactionHistory = JSON.parse(savedHistory);
        }

        if (savedStats) {
            parkingStats = JSON.parse(savedStats);
        }
    } catch (error) {
        console.error('Error loading data from localStorage:', error);
    }
}

function saveDataToStorage() {
    try {
        localStorage.setItem(STORAGE_KEYS.RECORDS, JSON.stringify(parkingSpots));
        localStorage.setItem(STORAGE_KEYS.HISTORY, JSON.stringify(transactionHistory));
        localStorage.setItem(STORAGE_KEYS.STATS, JSON.stringify(parkingStats));
    } catch (error) {
        console.error('Error saving data to localStorage:', error);
    }
}

// --- 5. INITIAL DEMO DATA GENERATOR ---
function loadDemoData() {
    // Reset slots
    parkingSpots = Array.from({ length: TOTAL_SLOTS }, (_, index) => ({
        slotNumber: index + 1,
        isOccupied: false,
        vehicle: null
    }));

    // Prepopulate with a few realistic vehicles
    const sampleVehicles = [
        {
            slotNumber: 1,
            isOccupied: true,
            vehicle: {
                regNumber: 'DL 01 AB 1234',
                ownerName: 'Rahul Sharma',
                type: 2, // Car
                entryTime: new Date(Date.now() - 2.5 * 3600 * 1000).toISOString() // 2.5 hours ago
            }
        },
        {
            slotNumber: 3,
            isOccupied: true,
            vehicle: {
                regNumber: 'MH 12 CD 5678',
                ownerName: 'Priya Patel',
                type: 1, // Bike
                entryTime: new Date(Date.now() - 1.2 * 3600 * 1000).toISOString() // 1.2 hours ago
            }
        },
        {
            slotNumber: 7,
            isOccupied: true,
            vehicle: {
                regNumber: 'KA 05 EF 9012',
                ownerName: 'Amit Verma',
                type: 3, // Heavy Vehicle
                entryTime: new Date(Date.now() - 4 * 3600 * 1000).toISOString() // 4 hours ago
            }
        }
    ];

    sampleVehicles.forEach(sample => {
        const spot = parkingSpots.find(s => s.slotNumber === sample.slotNumber);
        if (spot) {
            spot.isOccupied = true;
            spot.vehicle = sample.vehicle;
        }
    });

    transactionHistory = [
        {
            invoiceId: 'INV-172330101',
            slotNumber: 2,
            regNumber: 'HR 26 DQ 4433',
            ownerName: 'Vikram Singh',
            type: 2,
            entryTime: new Date(Date.now() - 10 * 3600 * 1000).toISOString(),
            exitTime: new Date(Date.now() - 8 * 3600 * 1000).toISOString(),
            durationText: '2.00 hours (120 mins)',
            amountPaid: 100.0
        }
    ];

    parkingStats = {
        totalServed: 1,
        totalRevenue: 100.0
    };

    saveDataToStorage();
    renderAll();
}

// --- 6. UI RENDERING FUNCTIONS ---

function renderAll() {
    renderStats();
    renderGrid();
    renderActiveTable();
    renderHistoryTable();
    populateSlotDropdown();
}

/** Updates top analytic counters */
function renderStats() {
    const occupiedCount = parkingSpots.filter(s => s.isOccupied).length;
    const freeCount = TOTAL_SLOTS - occupiedCount;
    const occupancyPercentage = Math.round((occupiedCount / TOTAL_SLOTS) * 100);

    document.getElementById('statTotalSlots').textContent = TOTAL_SLOTS;
    document.getElementById('statOccupied').textContent = occupiedCount;
    document.getElementById('statOccupancyRate').textContent = `${occupancyPercentage}% Occupancy`;
    document.getElementById('statFree').textContent = freeCount;
    document.getElementById('statServiced').textContent = parkingStats.totalServed;
    document.getElementById('statRevenue').textContent = formatCurrency(parkingStats.totalRevenue);
    document.getElementById('activeCountBadge').textContent = `${occupiedCount} Active`;
}

/** Renders the 20-spot 2D Interactive Parking Grid */
function renderGrid(filterQuery = '') {
    const gridContainer = document.getElementById('parkingGrid');
    gridContainer.innerHTML = '';

    const cleanQuery = filterQuery.trim().toLowerCase();

    parkingSpots.forEach(spot => {
        const slotEl = document.createElement('div');
        slotEl.className = `parking-slot ${spot.isOccupied ? 'slot-occupied' : 'slot-free'}`;
        slotEl.id = `spot-${spot.slotNumber}`;

        if (spot.isOccupied) {
            const v = spot.vehicle;
            const vInfo = VEHICLE_RATES[v.type] || VEHICLE_RATES[2];
            const duration = calculateDuration(v.entryTime);

            // Check if matches search filter
            const isMatch = !cleanQuery || 
                v.regNumber.toLowerCase().includes(cleanQuery) || 
                v.ownerName.toLowerCase().includes(cleanQuery);

            if (!isMatch) {
                slotEl.style.opacity = '0.35';
            }

            slotEl.innerHTML = `
                <div class="slot-header">
                    <span class="slot-number">Slot #${String(spot.slotNumber).padStart(2, '0')}</span>
                    <span class="slot-status-badge">Occupied</span>
                </div>
                <div class="slot-body">
                    <span class="slot-icon">${vInfo.icon}</span>
                    <div class="slot-reg">${escapeHtml(v.regNumber)}</div>
                    <div class="slot-owner">${escapeHtml(v.ownerName)}</div>
                </div>
                <div class="slot-footer">
                    ⏱️ ${duration.displayTime}
                </div>
            `;

            slotEl.title = `Click to Check Out: ${v.regNumber} (${v.ownerName})`;
            slotEl.addEventListener('click', () => promptUnpark(spot.slotNumber));

        } else {
            slotEl.innerHTML = `
                <div class="slot-header">
                    <span class="slot-number">Slot #${String(spot.slotNumber).padStart(2, '0')}</span>
                    <span class="slot-status-badge">Free</span>
                </div>
                <div class="slot-body">
                    <span class="slot-icon">🟢</span>
                    <div class="slot-reg">AVAILABLE</div>
                    <div class="slot-owner">Empty Slot</div>
                </div>
                <div class="slot-footer">
                    ⚡ Click to Park
                </div>
            `;

            slotEl.title = `Click to Park in Slot #${spot.slotNumber}`;
            slotEl.addEventListener('click', () => {
                document.getElementById('slotSelect').value = String(spot.slotNumber);
                document.getElementById('regNumber').focus();
            });
        }

        gridContainer.appendChild(slotEl);
    });
}

/** Renders currently active parked vehicles in table */
function renderActiveTable(filterQuery = '') {
    const tbody = document.getElementById('activeVehiclesBody');
    tbody.innerHTML = '';

    const cleanQuery = filterQuery.trim().toLowerCase();
    const activeSpots = parkingSpots.filter(s => s.isOccupied);

    const filteredSpots = activeSpots.filter(spot => {
        if (!cleanQuery) return true;
        const v = spot.vehicle;
        return v.regNumber.toLowerCase().includes(cleanQuery) || v.ownerName.toLowerCase().includes(cleanQuery);
    });

    if (filteredSpots.length === 0) {
        tbody.innerHTML = `
            <tr>
                <td colspan="7" class="text-center empty-state">
                    ${activeSpots.length === 0 ? 'No vehicles currently parked.' : 'No vehicles match your search.'}
                </td>
            </tr>
        `;
        return;
    }

    filteredSpots.forEach(spot => {
        const v = spot.vehicle;
        const vInfo = VEHICLE_RATES[v.type] || VEHICLE_RATES[2];
        const duration = calculateDuration(v.entryTime);

        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td><strong>#${String(spot.slotNumber).padStart(2, '0')}</strong></td>
            <td><span class="badge badge-primary">${escapeHtml(v.regNumber)}</span></td>
            <td>${escapeHtml(v.ownerName)}</td>
            <td>${vInfo.icon} ${vInfo.name}</td>
            <td>${formatDateTime(v.entryTime)}</td>
            <td><strong>${duration.displayTime}</strong></td>
            <td>
                <button class="btn btn-danger btn-sm" onclick="promptUnpark(${spot.slotNumber})">
                    💳 Check Out
                </button>
            </td>
        `;
        tbody.appendChild(tr);
    });
}

/** Renders history log table */
function renderHistoryTable() {
    const tbody = document.getElementById('historyBody');
    tbody.innerHTML = '';

    if (transactionHistory.length === 0) {
        tbody.innerHTML = `
            <tr>
                <td colspan="8" class="text-center empty-state">No transactions recorded yet.</td>
            </tr>
        `;
        return;
    }

    // Display recent transactions first
    const reversed = [...transactionHistory].reverse();

    reversed.forEach(item => {
        const vInfo = VEHICLE_RATES[item.type] || VEHICLE_RATES[2];
        const tr = document.createElement('tr');
        tr.innerHTML = `
            <td><code>${escapeHtml(item.invoiceId)}</code></td>
            <td>#${String(item.slotNumber).padStart(2, '0')}</td>
            <td><strong>${escapeHtml(item.regNumber)}</strong></td>
            <td>${escapeHtml(item.ownerName)}</td>
            <td>${vInfo.icon} ${vInfo.shortName}</td>
            <td>${escapeHtml(item.durationText)}</td>
            <td class="text-success"><strong>${formatCurrency(item.amountPaid)}</strong></td>
            <td>${formatDateTime(item.exitTime)}</td>
        `;
        tbody.appendChild(tr);
    });
}

/** Populates the assign slot dropdown */
function populateSlotDropdown() {
    const slotSelect = document.getElementById('slotSelect');
    const currentValue = slotSelect.value;
    
    slotSelect.innerHTML = '<option value="auto">⚡ Auto-Assign Nearest Free Slot</option>';

    parkingSpots.forEach(spot => {
        if (!spot.isOccupied) {
            const opt = document.createElement('option');
            opt.value = spot.slotNumber;
            opt.textContent = `Slot #${String(spot.slotNumber).padStart(2, '0')} (Free)`;
            slotSelect.appendChild(opt);
        }
    });

    if (currentValue && currentValue !== 'auto') {
        slotSelect.value = currentValue;
    }
}

// --- 7. CORE BUSINESS LOGIC (CHECK-IN & CHECK-OUT) ---

/** Handle Check-in Form Submission */
function handleParkVehicle(event) {
    event.preventDefault();

    const regInput = document.getElementById('regNumber');
    const ownerInput = document.getElementById('ownerName');
    const typeInput = document.getElementById('vehicleType');
    const slotInput = document.getElementById('slotSelect');

    const regNumber = regInput.value.trim().toUpperCase();
    const ownerName = ownerInput.value.trim();
    const vehicleType = parseInt(typeInput.value, 10);
    const chosenSlot = slotInput.value;

    if (!regNumber || !ownerName) {
        alert('Please fill in both Registration Number and Owner Name.');
        return;
    }

    // Duplicate registration check
    const duplicate = parkingSpots.find(s => s.isOccupied && s.vehicle.regNumber.toUpperCase() === regNumber);
    if (duplicate) {
        alert(`Error: Vehicle with Registration Number "${regNumber}" is already parked in Slot #${duplicate.slotNumber}!`);
        return;
    }

    // Find slot
    let targetSpot = null;
    if (chosenSlot === 'auto') {
        targetSpot = parkingSpots.find(s => !s.isOccupied);
        if (!targetSpot) {
            alert('Parking Lot is Full! All 20 slots are currently occupied.');
            return;
        }
    } else {
        const slotNum = parseInt(chosenSlot, 10);
        targetSpot = parkingSpots.find(s => s.slotNumber === slotNum);
        if (!targetSpot || targetSpot.isOccupied) {
            alert(`Slot #${slotNum} is no longer available. Please select another slot.`);
            return;
        }
    }

    // Create Vehicle Record
    const entryTimestamp = new Date().toISOString();
    targetSpot.isOccupied = true;
    targetSpot.vehicle = {
        regNumber: regNumber,
        ownerName: ownerName,
        type: vehicleType,
        entryTime: entryTimestamp
    };

    saveDataToStorage();
    renderAll();

    // Reset Form
    event.target.reset();
    document.getElementById('vehicleType').value = "2";

    // Show Entry Slip Modal
    showSlipModal(targetSpot.slotNumber, targetSpot.vehicle);
}

/** Check-in Entry Slip Modal Display */
function showSlipModal(slotNumber, vehicle) {
    const vInfo = VEHICLE_RATES[vehicle.type] || VEHICLE_RATES[2];
    const modalContent = document.getElementById('slipModalContent');

    modalContent.innerHTML = `
        <div class="ticket-box">
            <div class="ticket-header">
                <h4>PARKEASY ENTRY SLIP</h4>
                <p style="font-size:0.75rem; color:#64748b;">Keep this slip safe until check-out</p>
            </div>
            <div class="ticket-row">
                <span class="label">Assigned Slot:</span>
                <span class="value" style="font-size:1.1rem; color:#3b82f6;">Slot #${String(slotNumber).padStart(2, '0')}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Vehicle No:</span>
                <span class="value">${escapeHtml(vehicle.regNumber)}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Owner Name:</span>
                <span class="value">${escapeHtml(vehicle.ownerName)}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Category:</span>
                <span class="value">${vInfo.icon} ${vInfo.name}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Hourly Rate:</span>
                <span class="value">${formatCurrency(vInfo.rate)} / hr</span>
            </div>
            <div class="ticket-row">
                <span class="label">Entry Timestamp:</span>
                <span class="value">${formatDateTime(vehicle.entryTime)}</span>
            </div>
        </div>
    `;

    openModal('slipModal');
}

/** Prompt checkout confirm and calculation */
function promptUnpark(slotNumber) {
    const spot = parkingSpots.find(s => s.slotNumber === slotNumber);
    if (!spot || !spot.isOccupied) {
        alert(`Slot #${slotNumber} is already free.`);
        return;
    }

    const v = spot.vehicle;
    const vInfo = VEHICLE_RATES[v.type] || VEHICLE_RATES[2];
    const exitTime = new Date();
    const duration = calculateDuration(v.entryTime, exitTime);
    const totalAmount = duration.billedHours * vInfo.rate;
    const invoiceId = `INV-${Date.now().toString().slice(-8)}`;

    // Update state
    parkingStats.totalServed += 1;
    parkingStats.totalRevenue += totalAmount;

    // Record in history
    const historyEntry = {
        invoiceId: invoiceId,
        slotNumber: spot.slotNumber,
        regNumber: v.regNumber,
        ownerName: v.ownerName,
        type: v.type,
        entryTime: v.entryTime,
        exitTime: exitTime.toISOString(),
        durationText: `${duration.exactHours} hrs (${duration.minutes}m)`,
        amountPaid: totalAmount
    };

    transactionHistory.push(historyEntry);

    // Free the parking slot
    spot.isOccupied = false;
    spot.vehicle = null;

    saveDataToStorage();
    renderAll();

    // Show Invoice Modal
    showInvoiceModal(historyEntry, vInfo, duration);
}

/** Display Checkout Invoice Modal */
function showInvoiceModal(historyEntry, vInfo, duration) {
    const modalContent = document.getElementById('invoiceModalContent');

    modalContent.innerHTML = `
        <div class="ticket-box">
            <div class="ticket-header">
                <h4>PARKEASY CHECK-OUT RECEIPT</h4>
                <p style="font-size:0.75rem; color:#64748b;">Payment Status: <span style="color:#10b981; font-weight:700;">PAID</span></p>
            </div>
            <div class="ticket-row">
                <span class="label">Invoice ID:</span>
                <span class="value"><code>${escapeHtml(historyEntry.invoiceId)}</code></span>
            </div>
            <div class="ticket-row">
                <span class="label">Slot Number:</span>
                <span class="value">Slot #${String(historyEntry.slotNumber).padStart(2, '0')}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Vehicle No:</span>
                <span class="value">${escapeHtml(historyEntry.regNumber)}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Owner Name:</span>
                <span class="value">${escapeHtml(historyEntry.ownerName)}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Category:</span>
                <span class="value">${vInfo.icon} ${vInfo.name}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Entry Time:</span>
                <span class="value">${formatDateTime(historyEntry.entryTime)}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Exit Time:</span>
                <span class="value">${formatDateTime(historyEntry.exitTime)}</span>
            </div>
            <div class="ticket-row">
                <span class="label">Duration:</span>
                <span class="value">${duration.exactHours} hrs (${duration.minutes} mins)</span>
            </div>
            <div class="ticket-row">
                <span class="label">Tariff Rate:</span>
                <span class="value">${formatCurrency(vInfo.rate)} / hr</span>
            </div>
            <div class="ticket-total">
                <span>TOTAL AMOUNT:</span>
                <span>${formatCurrency(historyEntry.amountPaid)}</span>
            </div>
        </div>
    `;

    openModal('invoiceModal');
}

// --- 8. MODAL CONTROLS ---
function openModal(modalId) {
    const modal = document.getElementById(modalId);
    if (modal) {
        modal.classList.add('active');
        modal.setAttribute('aria-hidden', 'false');
    }
}

function closeModal(modalId) {
    const modal = document.getElementById(modalId);
    if (modal) {
        modal.classList.remove('active');
        modal.setAttribute('aria-hidden', 'true');
    }
}

// Close modals when clicking backdrop
window.addEventListener('click', (e) => {
    if (e.target.classList.contains('modal-overlay')) {
        e.target.classList.remove('active');
        e.target.setAttribute('aria-hidden', 'true');
    }
});

// Escape HTML for XSS safety
function escapeHtml(str) {
    if (!str) return '';
    return str
        .replace(/&/g, '&amp;')
        .replace(/</g, '&lt;')
        .replace(/>/g, '&gt;')
        .replace(/"/g, '&quot;')
        .replace(/'/g, '&#039;');
}

// --- 9. EVENT LISTENERS & INITIALIZATION ---
document.addEventListener('DOMContentLoaded', () => {
    // 1. Load Data
    loadDataFromStorage();

    // 2. Form submission
    const parkForm = document.getElementById('parkForm');
    if (parkForm) {
        parkForm.addEventListener('submit', handleParkVehicle);
    }

    // 3. Search Filter
    const searchInput = document.getElementById('searchInput');
    if (searchInput) {
        searchInput.addEventListener('input', (e) => {
            const q = e.target.value;
            renderGrid(q);
            renderActiveTable(q);
        });
    }

    // 4. Demo Data Reset Button
    const btnDemo = document.getElementById('btnDemoData');
    if (btnDemo) {
        btnDemo.addEventListener('click', () => {
            if (confirm('Load sample parked vehicles and reset current state?')) {
                loadDemoData();
            }
        });
    }

    // 5. Clear History Button
    const btnClear = document.getElementById('btnClearHistory');
    if (btnClear) {
        btnClear.addEventListener('click', () => {
            if (confirm('Are you sure you want to clear transaction history?')) {
                transactionHistory = [];
                saveDataToStorage();
                renderHistoryTable();
            }
        });
    }

    // 6. Live Clock
    const clockEl = document.getElementById('liveClock');
    function updateClock() {
        const now = new Date();
        clockEl.textContent = now.toLocaleTimeString();
    }
    updateClock();
    setInterval(updateClock, 1000);

    // Initial render
    renderAll();
});
