#include "../include/product.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <chrono>
#include <thread>

using namespace std;

ProductStack::~ProductStack() {
    while (!empty()) {
        pop();
    }
}

void ProductStack::push(const Product& val) {
    Node* newNode = new Node(val);
    newNode->next = topNode;
    topNode = newNode;
}
void ProductStack::pop() {
    if (empty()) return;
    Node* temp = topNode->next;
    delete topNode;
    topNode = temp;
}

Product& ProductStack::top() {
    if (empty()) {
        throw runtime_error("Stack is empty!");
    }
    return topNode->data;
}

ProductQueue::~ProductQueue() {
    while (!empty()) {
        pop();
    }
}

void ProductQueue::push(int val) {
    Node* newNode = new Node(val);
    if (empty()) {
        frontNode = backNode = newNode;
    } else {
        backNode->next = newNode;
        backNode = newNode;
    }
}

void ProductQueue::pop() {
    if (empty()) return;
    Node* temp = frontNode;
    frontNode = frontNode->next;
    if (frontNode == nullptr) {
        backNode = nullptr;
    }
    delete temp;
}

int ProductQueue::front() const {
    if (empty()) {
        throw runtime_error("Queue is empty!");
    }
    return frontNode->data;
}

int ProductQueue::size() const {
    int count = 0;
    Node* current = frontNode;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

Stock::Stock() : head(nullptr), tail(nullptr) {}

Stock::~Stock() {
    Product* current = head;
    while (current != nullptr) {
        Product* next = current->next;
        delete current;
        current = next;
    }
}

void Stock::_addProduct(int id, string name, double price, int quantity, string category) {
    Product* p = new Product;
    p->id = id;
    p->name = name;
    p->price = price;
    p->quantity = quantity;
    p->category = category;
    p->next = nullptr;
    p->prev = nullptr;

    if (head == nullptr) {
        head = tail = p;
    } else {
        p->prev = tail;
        tail->next = p;
        tail = p;
    }
}

void Stock::swapProductData(Product* a, Product* b) {
    swap(a->id, b->id);
    swap(a->name, b->name);
    swap(a->price, b->price);
    swap(a->quantity, b->quantity);
    swap(a->category, b->category);
}

void Stock::addProduct(int id, string name, double price, int quantity, string category) {
    _addProduct(id, name, price, quantity, category);
    saveStock();
    cout << "Product added successfully!\n";
}

void Stock::viewProduct(const string& stockFile) {
    loadStock();
    Product* current = head;
    
    // Table header
    cout << "\n";
    cout << "┌────────┬──────────────────────┬───────────┬──────────┬────────────┐\n";
    cout << "│ " << left << setw(7) << "ID" << " │ " << setw(20) << "Name" << " │ " 
         << setw(9) << "Price" << " │ " << setw(8) << "Qty" << " │ " << setw(10) << "Category" << " │\n";
    cout << "├────────┼──────────────────────┼───────────┼──────────┼────────────┤\n";
    
    // Table rows
    while (current) {
        cout << "│ " << left << setw(7) << current->id << " │ " 
             << setw(20) << (current->name.length() > 20 ? current->name.substr(0, 17) + "..." : current->name) << " │ "
             << "$" << right << setw(8) << fixed << setprecision(2) << current->price << " │ "
             << left << setw(8) << current->quantity << " │ "
             << setw(10) << (current->category.length() > 10 ? current->category.substr(0, 7) + "..." : current->category) << " │\n";
        current = current->next;
    }
    
    // Table footer
    cout << "└────────┴──────────────────────┴───────────┴──────────┴────────────┘\n\n";
}

void Stock::_deleteProduct(int id) {
    Product* current = head;
    while (current) {
        if (current->id == id) {
            if (current->prev) current->prev->next = current->next;
            if (current->next) current->next->prev = current->prev;
            if (current == head) head = current->next;
            if (current == tail) tail = current->prev;
            delete current;
            return;
        }
        current = current->next;
    }
}

void Stock::deleteProduct(int id) {
    Product* current = head;
    while (current) {
        if (current->id == id) {
            Product productCopy = *current;
            productCopy.next = nullptr;
            productCopy.prev = nullptr;
            
            undoStack.push(productCopy);
            
            _deleteProduct(id);
            saveStock();
            cout << "Product deleted.\n";
            return;
        }
        current = current->next;
    }
    cout << "Product not found.\n";
}

void Stock::updateProduct(int id) {
    Product* current = head;
    while (current) {
        if (current->id == id) {
            int newQty;
            double newPrice;
            cout << "Enter new quantity: ";
            cin >> newQty;
            cout << "Enter new price: ";
            cin >> newPrice;
            current->quantity = newQty;
            current->price = newPrice;
            saveStock();
            cout << "Product updated.\n";
            return;
        }
        current = current->next;
    }
    cout << "Product not found.\n";
}

void Stock::searchProduct(int id) {
    Product* current = head;
    while (current) {
        if (current->id == id) {
            cout << "Product found: " << current->name << " (Qty: " << current->quantity << ")\n";
            return;
        }
        current = current->next;
    }
    cout << "Product not found.\n";
}

void Stock::sortByName() {
    if (!head) return;
    bool swapped;
    do {
        swapped = false;
        Product* current = head;
        while (current->next) {
            if (current->name > current->next->name) {
                swapProductData(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
    cout << "Products sorted by name.\n";
    saveStock();
}

void Stock::sortById() {
    if (!head) return;
    bool swapped;
    do {
        swapped = false;
        Product* current = head;
        while (current->next) {
            if (current->id > current->next->id) {
                swapProductData(current, current->next);
                swapped = true;
            }
            current = current->next;
        }
    } while (swapped);
    cout << "Products sorted by ID.\n";
    saveStock();
}

void Stock::loadStock() {
    ifstream file("stock.csv");
    if (!file.is_open()) return; 
    
    Product* current = head;
    while (current) {
        Product* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr, name, priceStr, qtyStr, category;
        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, priceStr, ',');
        getline(ss, qtyStr, ',');
        getline(ss, category, ',');
        int id = stoi(idStr);
        double price = stod(priceStr);
        int qty = stoi(qtyStr);
        _addProduct(id, name, price, qty, category);
    }
    file.close();
}

void Stock::saveStock() {
    ofstream file("stock.csv");
    Product* current = head;
    while (current) {
        file << current->id << "," << current->name << "," << current->price << "," << current->quantity << "," << current->category << "\n";
        current = current->next;
    }
    file.close();
}


void Stock::undoLastOperation() {
    if (undoStack.empty()) {
        cout << "Nothing to undo!\n";
        return;
    }

    Product lastState = undoStack.top();
    undoStack.pop();

    bool productExists = false;
    Product* current = head;
    while (current != nullptr) {
        if (current->id == lastState.id) {
            productExists = true;
            break;
        }
        current = current->next;
    }

    if (!productExists) {
        _addProduct(lastState.id, lastState.name, lastState.price,
                   lastState.quantity, lastState.category);
        saveStock();
        cout << "Undo successful! Deleted product ID " << lastState.id << " has been restored.\n";
    } else {
        cout << "Cannot undo: The product already exists in the inventory.\n";
    }
}

void Stock::addRestockRequest(int id) {
    restockQueue.push(id);
    cout << "Restock request added for Product ID: " << id << endl;
}

void Stock::processRestockRequests() {
    if (restockQueue.empty()) {
        cout << "No restock requests in queue!\n";
        this_thread::sleep_for(chrono::seconds(1));
        return;
    }

    cout << "Processing " << restockQueue.size() << " restock requests...\n";
    int processed = 0;

    while (!restockQueue.empty()) {
        int id = restockQueue.front();
        restockQueue.pop();

        Product* current = head;
        bool found = false;

        while (current != nullptr) {
            if (current->id == id) {
                found = true;
                int restockQty;
                cout << "Enter restock quantity for Product ID " << id << ": ";
                while (!(cin >> restockQty)) {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                current->quantity += restockQty;
                cout << "[Request " << ++processed << "] Restocked " << restockQty 
                     << " units of Product ID " << id
                     << ". New quantity: " << current->quantity << endl;
                found = true;
                break;
            }
            current = current->next;
        }

        if (!found) {
            cout << "Product ID " << id << " not found. Restock skipped.\n";
            this_thread::sleep_for(chrono::seconds(1));
        }
    }
    saveStock();
    cout << "\nProcessed " << processed << " restock requests.\n";
}
