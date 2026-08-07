#include <iostream>
using namespace std;

const int MAX_SLOTS = 10;

int vehicleNumber[MAX_SLOTS];
string ownerName[MAX_SLOTS];
string vehicleType[MAX_SLOTS];

int parkedVehicles = 0;

// Vehicle park karna
void parkVehicle()
{
    if (parkedVehicles == MAX_SLOTS)
    {
        cout << "\nParking is full!\n";
        return;
    }

    cout << "\nEnter vehicle number: ";
    cin >> vehicleNumber[parkedVehicles];

    cout << "Enter owner name: ";
    cin >> ownerName[parkedVehicles];

    cout << "Enter vehicle type: ";
    cin >> vehicleType[parkedVehicles];

    parkedVehicles++;

    cout << "\nVehicle parked successfully!\n";
}

// Saare vehicles display karna
void displayVehicles()
{
    if (parkedVehicles == 0)
    {
        cout << "\nNo vehicle is parked.\n";
        return;
    }

    cout << "\n----- Parked Vehicles -----\n";

    for (int i = 0; i < parkedVehicles; i++)
    {
        cout << "\nSlot: " << i + 1 << endl;
        cout << "Vehicle Number: " << vehicleNumber[i] << endl;
        cout << "Owner Name: " << ownerName[i] << endl;
        cout << "Vehicle Type: " << vehicleType[i] << endl;
    }
}

// Vehicle search karna
void searchVehicle()
{
    int number;
    bool found = false;

    cout << "\nEnter vehicle number: ";
    cin >> number;

    for (int i = 0; i < parkedVehicles; i++)
    {
        if (vehicleNumber[i] == number)
        {
            cout << "\nVehicle found!\n";
            cout << "Slot: " << i + 1 << endl;
            cout << "Owner Name: " << ownerName[i] << endl;
            cout << "Vehicle Type: " << vehicleType[i] << endl;

            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "\nVehicle not found.\n";
    }
}

// Available slots dikhana
void showSlots()
{
    cout << "\nTotal slots: " << MAX_SLOTS;
    cout << "\nOccupied slots: " << parkedVehicles;
    cout << "\nAvailable slots: " << MAX_SLOTS - parkedVehicles << endl;
}

int main()
{
    int choice;

    do
    {
        cout << "\n\n==============================";
        cout << "\n     VEHICLE PARKING SYSTEM";
        cout << "\n==============================";
        cout << "\n1. Park Vehicle";
        cout << "\n2. Display Vehicles";
        cout << "\n3. Search Vehicle";
        cout << "\n4. Show Available Slots";
        cout << "\n5. Exit";
        cout << "\n==============================";

        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            parkVehicle();
            break;

        case 2:
            displayVehicles();
            break;

        case 3:
            searchVehicle();
            break;

        case 4:
            showSlots();
            break;

        case 5:33
            cout << "\nThank you!\n";
            break;

        default:
            cout << "\nInvalid choice!\n";
        }

    } while (choice != 5);

    return 0;
}