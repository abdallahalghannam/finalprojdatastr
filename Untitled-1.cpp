#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

// ==========================================
// 1. Anger Management System 
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


struct TreeNode {
    Parcel data;
    TreeNode* left;
    TreeNode* right;
};

class ParcelBST {
public:
    TreeNode* root;
    ParcelBST() { root = NULL; }
    
    TreeNode* insert(TreeNode* node, Parcel p) {
        if (node == NULL) {
            TreeNode* newNode = new TreeNode();
            newNode->data = p;
            newNode->left = NULL;
            newNode->right = NULL;
            return newNode;
        }
        if (p.trackingID < node->data.trackingID) node->left = insert(node->left, p);
        else if (p.trackingID > node->data.trackingID) node->right = insert(node->right, p);
        return node;
    }
    
    TreeNode* search(TreeNode* node, int id) {
        if (node == NULL) { 
            cout << "Parcel not found in system records!\n"; 
            return NULL; 
        }
        if (node->data.trackingID == id) {
            string cleanDest = node->data.destination;
            for(int k = 0; k < cleanDest.length(); k++) {
                if(cleanDest[k] == '_') cleanDest[k] = ' ';
            }
            cout << "Found! ID: " << setfill('0') << setw(4) << node->data.trackingID << setfill(' ')
                 << " | Dest: " << cleanDest 
                 << " | Weight: " << node->data.weight << " kg\n";
            return node;
        }
        if (id < node->data.trackingID) return search(node->left, id);
        else return search(node->right, id);
    }
    
    TreeNode* deleteNode(TreeNode* root, int id) {
        if (root == NULL) return root;
        if (id < root->data.trackingID) root->left = deleteNode(root->left, id);
        else if (id > root->data.trackingID) root->right = deleteNode(root->right, id);
        else {
            if (root->left == NULL) { TreeNode* temp = root->right; delete root; return temp; }
            else if (root->right == NULL) { TreeNode* temp = root->left; delete root; return temp; }
            TreeNode* temp = root->right;
            while (temp && temp->left != NULL) temp = temp->left;
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data.trackingID);
        }
        return root;
    }
    
    void saveToFile(TreeNode* node, ofstream& file) {
        if (node != NULL) {
            saveToFile(node->left, file);
            file << setfill('0') << setw(4) << node->data.trackingID << setfill(' ') << " " 
                 << node->data.destination << " " 
                 << node->data.weight << "\n";
            saveToFile(node->right, file);
        }
    }
};

void displaySortedReport(int &angerLevel) {
    ifstream inFile("data.txt");
    if (!inFile) { cout << "No data found. Save data first.\n"; return; }
    
    int ids[100]; string dests[100]; int weights[100];
    int count = 0;
    while (inFile >> ids[count] >> dests[count] >> weights[count]) { count++; }
    inFile.close();

    if (count == 0) { cout << "Database is empty.\n"; return; }

    int sortChoice = 0;
    string inputStr;
    
    while (true) {
        cout << "\n--- Sort Options ---\n";
        cout << "1. Sort Alphabetically by City (Destination)\n";
        cout << "2. Sort by Parcel Weight (Light to Heavy)\n";
        cout << "Select option: ";
        cin >> inputStr;

        if (inputStr.length() == 1 && isdigit(inputStr[0])) {
            sortChoice = inputStr[0] - '0';
            if (sortChoice == 1 || sortChoice == 2) { angerLevel = 0; break; }
            else { handleAnger(angerLevel, "Select strictly 1 or 2."); }
        } else {
            handleAnger(angerLevel, "Input must be the DIGIT 1 or 2!");
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            bool swapNeeded = false;
            if (sortChoice == 1) {
                if (dests[j] > dests[j + 1]) swapNeeded = true;
            } else {
                if (weights[j] > weights[j + 1]) swapNeeded = true;
            }

            if (swapNeeded) {
                swap(ids[j], ids[j + 1]);
                swap(dests[j], dests[j + 1]);
                swap(weights[j], weights[j + 1]);
            }
        }
    }
    
    cout << "\n--- Sorted Warehouse Report ---\n";
    for (int i = 0; i < count; i++) {
        string cleanDest = dests[i];
        for(int k = 0; k < cleanDest.length(); k++) {
            if(cleanDest[k] == '_') cleanDest[k] = ' ';
        }
        cout << "ID: " << setfill('0') << setw(4) << ids[i] << setfill(' ') 
             << " | To: " << setw(15) << left << cleanDest 
             << " | Weight: " << weights[i] << " kg\n";
    }
}

int main() {
    ParcelBST bst;
    WarehouseStack stack;
    int staticID = 1; 
    int restoredCount = 0;

    cout << "\n============================================\n";
    cout << "       LOGISORT INITIALIZATION SEQUENCE       \n";
    cout << "============================================\n";

    ifstream initFile("data.txt");
    if (initFile.is_open()) {
        int tempID, tempWeight;
        string tempDest;
        while (initFile >> tempID >> tempDest >> tempWeight) {
            Parcel p = {tempID, tempDest, tempWeight};
            bst.root = bst.insert(bst.root, p); 
            stack.push(p, true);                
            if (tempID >= staticID) staticID = tempID + 1;
            restoredCount++;
        }
        initFile.close();
        if (restoredCount > 0) {
            cout << "[System Boot] Successfully restored " << restoredCount << " records from database.\n";
            cout << "[System Boot] Next Auto-ID will be: " << setfill('0') << setw(4) << staticID << setfill(' ') << "\n";
        } else {
            cout << "[System Boot] Database is empty. Starting fresh.\n";
        }
    } else {
        cout << "[System Boot] No database found. Starting fresh.\n";
    }

    int userLimit;
    while (true) {
        cout << "\nEnter extra facility capacity for trucks (Min 1, Max 100): ";
        if (!(cin >> userLimit) || userLimit < 1 || userLimit > 100) {
            cin.clear(); cin.ignore(10000, '\n');
            cout << "[Error] Please enter a valid logical number between 1 and 100.\n";
        } else {
            break;
        }
    }
    TruckQueue queue(restoredCount + userLimit); 
    cout << "[Success] Facility configured. Total Capacity: " << (restoredCount + userLimit) << " Trucks.\n";

    int angerLevel = 0; 
    string inputStr;    
    int choice;
    
    do {
        cout << "\n============================================\n";
        cout << "             LOGISORT MAIN MENU             \n";
        cout << "============================================\n";
        cout << "1. Add New Parcel to Warehouse\n";
        cout << "2. Search for a Parcel\n";
        cout << "3. Dispatch Truck (Load & Depart)\n";
        cout << "4. Save Current Data to File\n";
        cout << "5. View Sorted Report\n";
        cout << "6. Delete Parcel (Permanent)\n";
        cout << "0. Exit System\n";
        cout << "--------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> inputStr;
        
        if (inputStr.length() == 1 && isdigit(inputStr[0])) {
            choice = inputStr[0] - '0';
            if (choice < 0 || choice > 6) {
                handleAnger(angerLevel, "Select a valid menu option between 0 and 6.");
                continue;
            }
        } else {
            handleAnger(angerLevel, "Input must be a single DIGIT between 0 and 6!");
            continue;
        }

        angerLevel = 0; 

        if (choice == 1) {
            Parcel p;
            p.trackingID = staticID++; 
            cout << "\n--> System Assigned ID: " << setfill('0') << setw(4) << p.trackingID << setfill(' ') << "\n";
            
            cin.ignore(10000, '\n'); 
            
            while (true) {
                cout << "--> Enter Destination City: ";
                getline(cin, p.destination);
                
                bool hasLetter = false;
                for (int i = 0; i < p.destination.length(); i++) {
                    if (isalpha(p.destination[i])) hasLetter = true;
                }

                if (p.destination.length() < 3) handleAnger(angerLevel, "Destination name is too short.");
                else if (isdigit(p.destination[0])) handleAnger(angerLevel, "Destination name CANNOT start with a number.");
                else if (!hasLetter) handleAnger(angerLevel, "Destination must contain actual letters.");
                else {
                    for (int i = 0; i < p.destination.length(); i++) {
                        if (p.destination[i] == ' ') p.destination[i] = '_';
                    }
                    angerLevel = 0; break;          
                }
            }

            while (true) {
                cout << "--> Enter Parcel Weight (in kg, Max 100kg): ";
                if (!(cin >> p.weight) || p.weight <= 0 || p.weight > 100) {
                    cin.clear(); cin.ignore(10000, '\n');
                    handleAnger(angerLevel, "Weight must be a logical number between 1 and 100 kg!");
                } else {
                    angerLevel = 0; break;
                }
            }
            
            bst.root = bst.insert(bst.root, p);
            stack.push(p); 
        } 
        else if (choice == 2) {
            int id;
            while (true) {
                cout << "\n--> Enter Tracking ID to search: ";
                if (!(cin >> id)) { 
                    cin.clear(); cin.ignore(10000, '\n');
                    handleAnger(angerLevel, "Tracking ID must be a NUMBER!");
                } else {
                    angerLevel = 0; break;
                }
            }
            bst.search(bst.root, id);
        }
        else if (choice == 3) {
            int truckID;
            while (true) {
                cout << "\n--> Enter Truck Plate Number (Numbers only): ";
                if (!(cin >> truckID)) {
                    cin.clear(); cin.ignore(10000, '\n');
                    handleAnger(angerLevel, "Truck Plate must be a NUMBER!");
                } else {
                    angerLevel = 0; break;
                }
            }
            queue.enqueue(truckID, angerLevel);
            if (angerLevel == 0) { 
                stack.pop();
                queue.dequeue();
            }
        }
        else if (choice == 4) {
            ofstream outFile("data.txt");
            if (outFile.is_open()) {
                bst.saveToFile(bst.root, outFile);
                outFile.close();
                cout << "\n[Success] All data securely saved to data.txt.\n";
            } else {
                cout << "\n[Error] Unable to write to file!\n";
            }
        }
        else if (choice == 5) {
            displaySortedReport(angerLevel);
        }
        else if (choice == 6) {
            int delID;
            while (true) {
                cout << "\n--> Enter Tracking ID to DELETE permanently: ";
                if (!(cin >> delID)) {
                    cin.clear(); cin.ignore(10000, '\n');
                    handleAnger(angerLevel, "Tracking ID must be a NUMBER!");
                } else {
                    angerLevel = 0; break;
                }
            }
            if (bst.search(bst.root, delID) != NULL) {
                bst.root = bst.deleteNode(bst.root, delID);
                cout << "[Success] Parcel ID [" << setfill('0') << setw(4) << delID << setfill(' ') << "] deleted from system RAM.\n";
                cout << "[Note] Make sure to select option '4' (Save Data) to erase it from the file permanently.\n";
            }
        }
    } while (choice != 0);

    cout << "\nExiting LogiSort System. Goodbye!\n";
    return 0;
}
