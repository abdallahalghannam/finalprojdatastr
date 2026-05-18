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


// ==========================================
// --- NOUR MAIN FUNCTION HERE ---
// ==========================================
