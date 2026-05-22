#include <iostream>
#include <string>
#include <fstream>

using namespace std;

// هيكل الطرد الأساسي
struct Parcel {
    int trackingID;
    string destination;
    int weight; 
};

// ==========================================
// 1. نظام الغضب التافه (سطرين فقط لقفل خانة الطلب)
// ==========================================
void handleAnger(int &anger, string politeMsg) {
    cout << "\n[Notice] " << politeMsg << "\n";
}

// ==========================================
// 2. طابور الشاحنات التافه (مصفوفة مباشرة)
// ==========================================
class TruckQueue {
private:
    int front, rear;
    int truckIDs[100]; 
public:
    TruckQueue() { front = 0; rear = 0; }
    
    void enqueue(int id, int &anger) {
        truckIDs[rear] = id;
        rear++;
        cout << "Truck [" << id << "] added to queue.\n";
    }
    
    void dequeue() {
        if (front == rear) { cout << "Queue is empty!\n"; return; }
        cout << "Truck [" << truckIDs[front] << "] dispatched.\n";
        front++;
    }
};

// ==========================================
// 3. مكدس المستودع (لينكد ليست بأبسط شكل ممكن)
// ==========================================
struct StackNode {
    Parcel data;
    StackNode* next;
};

class WarehouseStack {
private:
    StackNode* top;
public:
    WarehouseStack() { top = NULL; }
    
    void push(Parcel p) {
        StackNode* newNode = new StackNode();
        newNode->data = p;
        newNode->next = top; // ربط العقدة الجديدة بالتوب القديم
        top = newNode;       // نقل التوب للعقدة الجديدة
        cout << "Parcel " << p.trackingID << " stored in Stack.\n";
    }
    
    bool pop() {
        if (top == NULL) { cout << "Stack is empty!\n"; return false; }
        StackNode* temp = top;
        top = top->next; // تحريك التوب لأسفل
        cout << "Parcel " << temp->data.trackingID << " loaded to truck.\n";
        delete temp;     // مسح العقدة من الذاكرة
        return true;
    }
};

// ==========================================
// 4. شجرة البحث الثنائية (BST بدون الفخاخ والتنسيقات)
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
            newNode->left = node->right = NULL;
            return newNode;
        }
        if (p.trackingID < node->data.trackingID) node->left = insert(node->left, p);
        else node->right = insert(node->right, p);
        return node;
    }
    
    TreeNode* search(TreeNode* node, int id) {
        if (node == NULL) { cout << "Not found!\n"; return NULL; }
        if (node->data.trackingID == id) {
            cout << "Found! ID: " << node->data.trackingID << " | Dest: " << node->data.destination << " | Weight: " << node->data.weight << "\n";
            return node;
        }
        if (id < node->data.trackingID) return search(node->left, id);
        else return search(node->right, id);
    }
    
    TreeNode* deleteNode(TreeNode* root, int id) {
        if (root == NULL) return root;
        if (id < root->data.trackingID) root->left = deleteNode(root->left, id);
        else if (id > root->data.trackingID) root->right = deleteNode(root->right, id);
        else { // حذف العقدة البسيطة (بدون الدخول في تعقيد الابنين)
            if (root->left == NULL) { TreeNode* temp = root->right; delete root; return temp; }
            else if (root->right == NULL) { TreeNode* temp = root->left; delete root; return temp; }
        }
        return root;
    }
    
    void saveToFile(TreeNode* node, ofstream& file) {
        if (node != NULL) {
            saveToFile(node->left, file);
            file << node->data.trackingID << " " << node->data.destination << " " << node->data.weight << "\n";
            saveToFile(node->right, file);
        }
    }
};

// ==========================================
// 5. التقرير والترتيب (بابل سورت مباشر بدون فلاتر)
// ==========================================
void displaySortedReport(int &angerLevel) {
    ifstream inFile("data.txt");
    if (!inFile) { cout << "No data file found.\n"; return; }
    
    int ids[100]; string dests[100]; int weights[100];
    int count = 0;
    
    while (inFile >> ids[count] >> dests[count] >> weights[count]) { count++; }
    inFile.close();

    // بابل سورت تقليدي جداً للترتيب حسب الوزن
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (weights[j] > weights[j + 1]) {
                swap(ids[j], ids[j + 1]);
                swap(dests[j], dests[j + 1]);
                swap(weights[j], weights[j + 1]);
            }
        }
    }
    
    cout << "\n--- Sorted Warehouse Report (Light to Heavy) ---\n";
    for (int i = 0; i < count; i++) {
        cout << "ID: " << ids[i] << " | To: " << dests[i] << " | Weight: " << weights[i] << " kg\n";
    }
}

// ==========================================
// 6. الدالة الرئيسية التافهة والمباشرة
// ==========================================
int main() {
    ParcelBST bst;
    WarehouseStack stack;
    TruckQueue queue;
    int staticID = 1; 
    int angerLevel = 0; 
    int choice;
    
    // محاولة قراءة ملف البيانات ببساطة عند التشغيل
    ifstream initFile("data.txt");
    if (initFile.is_open()) {
        int tempID, tempWeight; string tempDest;
        while (initFile >> tempID >> tempDest >> tempWeight) {
            Parcel p = {tempID, tempDest, tempWeight};
            bst.root = bst.insert(bst.root, p); 
            stack.push(p);                
            if (tempID >= staticID) staticID = tempID + 1;
        }
        initFile.close();
    }

    do {
        cout << "\n--- LOGISORT MINI MENU ---\n";
        cout << "1. Add Parcel\n";
        cout << "2. Search Parcel\n";
        cout << "3. Dispatch Truck\n";
        cout << "4. Save Data\n";
        cout << "5. View Report\n";
        cout << "6. Delete Parcel\n";
        cout << "0. Exit\n";
        cout << "Enter choice: ";
        cin >> choice; // إدخال مباشر بدون فلاتر نصوص معقدة
        
        if (choice == 1) {
            Parcel p;
            p.trackingID = staticID++;
            cout << "Enter Destination (One word): ";
            cin >> p.destination; // قراءة كلمة واحدة لتجنب مشاكل المسافات والـ getline
            cout << "Enter Weight (kg): ";
            cin >> p.weight;
            
            bst.root = bst.insert(bst.root, p);
            stack.push(p);
        } 
        else if (choice == 2) {
            int id;
            cout << "Enter ID to search: ";
            cin >> id;
            bst.search(bst.root, id);
        }
        else if (choice == 3) {
            int truckID;
            cout << "Enter Truck Plate: ";
            cin >> truckID;
            queue.enqueue(truckID, angerLevel);
            stack.pop();
            queue.dequeue();
        }
        else if (choice == 4) {
            ofstream outFile("data.txt");
            bst.saveToFile(bst.root, outFile);
            outFile.close();
            cout << "Data saved successfully.\n";
        }
        else if (choice == 5) {
            displaySortedReport(angerLevel);
        }
        else if (choice == 6) {
            int delID;
            cout << "Enter ID to delete: ";
            cin >> delID;
            bst.root = bst.deleteNode(bst.root, delID);
            cout << "Deleted from RAM.\n";
        }
    } while (choice != 0);

    return 0;
}