#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

// ==========================================
// 1. فحص الأخطاء وبيانات الطرود
// ==========================================

// دالة تزيد حدة الرسالة التحذيرية كل ما أخطأ المستخدم في الإدخال
void handleAnger(int &anger, string politeMsg) {
    if (anger == 0) cout << "\n[Notice] Please: " << politeMsg << "\n";
    else if (anger == 1) cout << "\n[Warning] I said it before! " << politeMsg << " Pay attention!\n";
    else if (anger == 2) cout << "\n[ERROR] ARE YOU BLIND?! " << politeMsg << "\n";
    else cout << "\n[FATAL] I AM LOSING MY MIND! READ THE INSTRUCTIONS! " << politeMsg << "!!!\n";
    anger++; 
}

// هيكل يحتوي على معلومات كل طرد
struct Parcel {
    int trackingID;     // رقم التتبع
    string destination; // المدينة المرسل إليها
    int weight;         // الوزن بالكيلو
};

// ==========================================
// --- كود أحمد: طابور الشاحنات (Truck Queue) ---
// مصفوفة عادية لترتيب الشاحنات خلف بعضها عند البوابة
// ==========================================

class TruckQueue {
private:
    int front, rear, currentSize, maxCapacity;
    int truckIDs[1000]; // مصفوفة تسع حتى 1000 شاحنة
public:
    // تجهيز المتغيرات عند تشغيل الطابور أول مرة
    TruckQueue(int limit) { 
        front = -1; rear = -1; currentSize = 0; maxCapacity = limit; 
    }
    
    // إضافة شاحنة جديدة في نهاية الطابور
    void enqueue(int id, int &anger) {
        // التأكد أن رقم لوحة الشاحنة غير متكرر في الطابور
        for (int i = front; i <= rear && front != -1; i++) {
            if (truckIDs[i] == id) {
                handleAnger(anger, "This Truck Plate is already in the queue! No duplicates allowed.");
                return;
            }
        }
        // التأكد من وجود مساحة في الطابور
        if (currentSize >= maxCapacity) { 
            cout << "Queue Overflow! Facility is at full capacity (" << maxCapacity << " trucks).\n"; 
            return; 
        }
        if (front == -1) front = 0;
        rear++;
        truckIDs[rear] = id; // تسجيل رقم الشاحنة
        currentSize++;
        cout << "Truck Plate [" << id << "] is waiting at the gate. (Capacity: " << currentSize << "/" << maxCapacity << ")\n";
        anger = 0; 
    }
    
    // خروج الشاحنة من مقدمة الطابور بعد التحميل
    void dequeue() {
        if (front == -1 || front > rear) { 
            cout << "Queue Underflow! No trucks waiting.\n"; 
            return; 
        }
        cout << "Truck Plate [" << truckIDs[front] << "] dispatched successfully.\n";
        front++; // تحريك الدور للشاحنة التي خلفها
        currentSize--;
    }
};

// ==========================================
// --- كود عبد الرحمن: مكدس المستودع (Warehouse Stack) ---
// لترتيب الطرود فوق بعضها (آخر طرد يدخل هو أول طرد يخرج لشاحنة التحميل)
// ==========================================

struct StackNode {
    Parcel data;
    StackNode* next;
};

class WarehouseStack {
private:
    StackNode* top; // مؤشر لآخر طرد مضاف فوق
public:
    WarehouseStack() { top = NULL; }
    
    // إضافة طرد جديد فوق المكدس
    void push(Parcel p, bool silent = false) {
        StackNode* newNode = new StackNode();
        newNode->data = p;
        newNode->next = top;
        top = newNode; // تغيير القمة للطرد الجديد
        if (!silent) {
            // طباعة رقم الطرد بـ 4 خانات مثل 0005
            cout << "Parcel " << setfill('0') << setw(4) << p.trackingID << setfill(' ') << " stored in Warehouse Stack.\n";
        }
    }
    
    // سحب الطرد الموجود على القمة لتحميله في الشاحنة
    bool pop() {
        if (top == NULL) { 
            cout << "Warehouse Stack is empty! No parcels to load.\n"; 
            return false; 
        }
        StackNode* temp = top;
        top = top->next; // الطرد الذي تحته يصبح هو القمة
        cout << "Parcel " << setfill('0') << setw(4) << temp->data.trackingID << setfill(' ') << " loaded to truck.\n";
        delete temp; // مسح العقدة القديمة من الذاكرة
        return true;
    }
};

// ==========================================
// --- كود عبد الله: شجرة البحث (Parcel BST) ---
// شجرة ثنائية لتسريع عملية البحث عن الطرود وحذفها باستخدام رقم التتبع
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
    
    // دالة لإدخال طرد جديد في مكانه الصحيح داخل الشجرة
    TreeNode* insert(TreeNode* node, Parcel p) {
        if (node == NULL) {
            TreeNode* newNode = new TreeNode();
            newNode->data = p;
            newNode->left = NULL;
            newNode->right = NULL;
            return newNode;
        }
        // الأصغر يروح يسار والأكبر يروح يمين
        if (p.trackingID < node->data.trackingID) node->left = insert(node->left, p);
        else if (p.trackingID > node->data.trackingID) node->right = insert(node->right, p);
        return node;
    }
    
    // دالة تبحث عن طرد برقم التتبع وتطبع معلوماته
    TreeNode* search(TreeNode* node, int id) {
        if (node == NULL) { 
            cout << "Parcel not found in system records!\n"; 
            return NULL; 
        }
        if (node->data.trackingID == id) {
            // استبدال الشرطات السفلية بمسافات عند عرض اسم المدينة
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
    
    // دالة لحذف طرد من الشجرة وإعادة ترتيب الفروع
    TreeNode* deleteNode(TreeNode* root, int id) {
        if (root == NULL) return root;
        if (id < root->data.trackingID) root->left = deleteNode(root->left, id);
        else if (id > root->data.trackingID) root->right = deleteNode(root->right, id);
        else {
            // إذا كانت العقدة بفرع واحد أو بدون فروع
            if (root->left == NULL) { TreeNode* temp = root->right; delete root; return temp; }
            else if (root->right == NULL) { TreeNode* temp = root->left; delete root; return temp; }
            
            // إذا كانت العقدة بفرعين (نستبدلها بأصغر عنصر في جهة اليمين)
            TreeNode* temp = root->right;
            while (temp && temp->left != NULL) temp = temp->left;
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data.trackingID);
        }
        return root;
    }
    
    // كتابة بيانات الشجرة داخل الملف بترتيب الأرقام من الأصغر للأكبر
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
// --- كود نور: التقارير والتحكم الرئيسي بالبرنامج ---
// ==========================================

// دالة تقرأ الملف الخارجي وترتب البيانات بـ Bubble Sort قبل العرض
void displaySortedReport(int &angerLevel) {
    ifstream inFile("data.txt");
    if (!inFile) { cout << "No data found. Save data first.\n"; return; }
    
    // مصفوفات لتخزين البيانات المسترجعة مؤقتاً لترتيبها
    int ids[100]; string dests[100]; int weights[100];
    int count = 0;
    while (inFile >> ids[count] >> dests[count] >> weights[count]) { count++; }
    inFile.close();

    if (count == 0) { cout << "Database is empty.\n"; return; }

    int sortChoice = 0;
    string inputStr;
    
    // التأكد من اختيار نوع ترتيب صحيح (1 أو 2)
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

    // فرز الفقاعات (Bubble Sort) لترتيب المصفوفات
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            bool swapNeeded = false;
            if (sortChoice == 1) { // الترتيب الأبجدي للمدن
                if (dests[j] > dests[j + 1]) swapNeeded = true;
            } else {               // الترتيب حسب الوزن
                if (weights[j] > weights[j + 1]) swapNeeded = true;
            }

            if (swapNeeded) {
                swap(ids[j], ids[j + 1]);
                swap(dests[j], dests[j + 1]);
                swap(weights[j], weights[j + 1]);
            }
        }
    }
    
    // طباعة الجدول النهائي المرتب
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
    int staticID = 1;       // عداد لحساب الـ ID القادم تلقائياً
    int restoredCount = 0;

    cout << "\n============================================\n";
    cout << "        LOGISORT INITIALIZATION SEQUENCE       \n";
    cout << "============================================\n";

    // استرجاع البيانات المخزنة في الملف عند بداية تشغيل البرنامج
    ifstream initFile("data.txt");
    if (initFile.is_open()) {
        int tempID, tempWeight;
        string tempDest;
        while (initFile >> tempID >> tempDest >> tempWeight) {
            Parcel p = {tempID, tempDest, tempWeight};
            bst.root = bst.insert(bst.root, p); // إعادة بناء الشجرة في الذاكرة
            stack.push(p, true);                // إعادة تعبئة المكدس بدون طباعة رسائل        
            if (tempID >= staticID) staticID = tempID + 1; // تحديث العداد التلقائي
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

    // تحديد السعة الإضافية للشاحنات
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
    
    // القائمة الرئيسية والتحكم بالخيارات
    do {
        cout << "\n============================================\n";
        cout << "               LOGISORT MAIN MENU             \n";
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
        
        // التحقق من أن الخيار هو رقم صحيح بين 0 و 6
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

        // خيار 1: إضافة طرد جديد وتخزينه في الشجرة والمكدس
        if (choice == 1) {
            Parcel p;
            p.trackingID = staticID++; 
            cout << "\n--> System Assigned ID: " << setfill('0') << setw(4) << p.trackingID << setfill(' ') << "\n";
            
            cin.ignore(10000, '\n'); 
            
            // فحص صحة اسم مدينة الوجهة
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
                    // تحويل المسافات لشرطة سفلية لمنع مشاكل قراءة الملف النصي
                    for (int i = 0; i < p.destination.length(); i++) {
                        if (p.destination[i] == ' ') p.destination[i] = '_';
                    }
                    angerLevel = 0; break;          
                }
            }

            // فحص صحة وزن الطرد
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
        // خيار 2: البحث عن طرد في الشجرة
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
        // خيار 3: دخول شاحنة، تحميل آخر طرد، ثم مغادرتها فوراً
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
        // خيار 4: حفظ البيانات الحالية في الملف الخارجي
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
        // خيار 5: عرض التقرير المُرتب
        else if (choice == 5) {
            displaySortedReport(angerLevel);
        }
        // خيار 6: حذف طرد من الذاكرة الحالية (RAM)
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
