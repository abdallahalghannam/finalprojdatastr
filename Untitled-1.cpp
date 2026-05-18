#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

// ==========================================
// 1. Anger Management System (Nour's Logic)
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
