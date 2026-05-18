#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

// ==========================================
// 1. Anger Management System (Nour's Logic) & Structs
// ==========================================
void handleAnger(int &anger, string politeMsg) {
    if (anger == 0) cout << "\n[Notice] Please: " << politeMsg << "\n";
    else if (anger == 1) cout << "\n[Warning] I said it before! " << politeMsg << " Pay attention!\n";
    else if (anger == 2) cout << "\n[ERROR] ARE YOU BLIND?! " << politeMsg << "\n";
    else cout << "\n[FATAL] I AM LOSING MY MIND! READ THE INSTRUCTIONS! " << politeMsg << "!!!\n";
    anger++;
}

struct Parcel {
    int trackingID;
    string destination;
    int weight; 
};

// ==========================================
// --- AHMED CODE HERE ---


class TruckQueue {
private:
    int front, rear, currentSize, maxCapacity;
    int truckIDs[1000]; 
public:
    TruckQueue(int limit) { 
        front = -1; rear = -1; currentSize = 0; maxCapacity = limit; 
    }
    
    void enqueue(int id, int &anger) {
        for (int i = front; i <= rear && front != -1; i++) {
            if (truckIDs[i] == id) {
                handleAnger(anger, "This Truck Plate is already in the queue! No duplicates allowed.");
                return;
            }
        }
        if (currentSize >= maxCapacity) { 
            cout << "Queue Overflow! Facility is at full capacity (" << maxCapacity << " trucks).\n"; 
            return; 
        }
        if (front == -1) front = 0;
        rear++;
        truckIDs[rear] = id;
        currentSize++;
        cout << "Truck Plate [" << id << "] is waiting at the gate. (Capacity: " << currentSize << "/" << maxCapacity << ")\n";
        anger = 0; 
    }
    
    void dequeue() {
        if (front == -1 || front > rear) { 
            cout << "Queue Underflow! No trucks waiting.\n"; 
            return; 
        }
        cout << "Truck Plate [" << truckIDs[front] << "] dispatched successfully.\n";
        front++;
        currentSize--;
    }
};
// ==========================================


// ==========================================
// --- ABDALRAHMAN CODE HERE ---
// ==========================================


// ==========================================
// --- ABDULLAH CODE HERE ---
// ==========================================


// ==========================================
// --- NOUR MAIN FUNCTION HERE ---
// ==========================================
