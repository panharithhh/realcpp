#include "../include/product.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <algorithm>

using namespace std;

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
    
    Product snapshot{ id, name, price, quantity, category, nullptr, nullptr };
    undoStack.push(snapshot);

    _addProduct(id, name, price, quantity, category);
    saveStock();
    cout << "Product added successfully!\n";
}


void Stock::viewProduct(const string& stockFile) {
    loadStock();
    Product* current = head;
    
    size_t maxNameLen = 4;
    size_t maxCategoryLen = 8;
    while (current) {
        if (current->name.length() > maxNameLen) maxNameLen = current->name.length();
        if (current->category.length() > maxCategoryLen) maxCategoryLen = current->category.length();
        current = current->next;
    }
    current = head;

    maxNameLen = max(maxNameLen, (size_t)20);
    maxCategoryLen = max(maxCategoryLen, (size_t)15);
    
    const int idWidth = 10;
    const int priceWidth = 12;
    const int qtyWidth = 10;
    const int nameWidth = maxNameLen + 2;
    const int categoryWidth = maxCategoryLen + 2;
    
    cout << "\n+";
    cout << string(idWidth, '-') << "+";
    cout << string(nameWidth, '-') << "+";
    cout << string(priceWidth, '-') << "+";
    cout << string(qtyWidth, '-') << "+";
    cout << string(categoryWidth, '-') << "+\n";
    
    cout << "| " << left << setw(idWidth-1) << "ID" << "| ";
    cout << setw(nameWidth-1) << "Name" << "| ";
    cout << setw(priceWidth-1) << "Price" << "| ";
    cout << setw(qtyWidth-1) << "Quantity" << "| ";
    cout << setw(categoryWidth-1) << "Category" << "|\n";
    
    cout << "+";
    cout << string(idWidth, '-') << "+";
    cout << string(nameWidth, '-') << "+";
    cout << string(priceWidth, '-') << "+";
    cout << string(qtyWidth, '-') << "+";
    cout << string(categoryWidth, '-') << "+\n";
    
    while (current) {
        cout << "| " << left << setw(idWidth-1) << current->id << "| ";
        
        string displayName = current->name;
        if (displayName.length() > nameWidth-1) {
            displayName = displayName.substr(0, nameWidth-4) + "...";
        }
        cout << setw(nameWidth-1) << displayName << "| ";
        
        cout << "$" << right << setw(priceWidth-2) << fixed << setprecision(2) << current->price << " | ";
        
        cout << left << setw(qtyWidth-1) << current->quantity << "| ";
        
        string displayCategory = current->category;
        if (displayCategory.length() > categoryWidth-1) {
            displayCategory = displayCategory.substr(0, categoryWidth-4) + "...";
        }
        cout << setw(categoryWidth-1) << displayCategory << "|\n";
        
        current = current->next;
    }
    
    cout << "+";
    cout << string(idWidth, '-') << "+";
    cout << string(nameWidth, '-') << "+";
    cout << string(priceWidth, '-') << "+";
    cout << string(qtyWidth, '-') << "+";
    cout << string(categoryWidth, '-') << "+\n\n";
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
            undoStack.push(*current); 
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
            undoStack.push(*current);
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

    // Check if product exists
    Product* current = head;
    while (current != nullptr) {
        if (current->id == lastState.id) {
            // Restore previous state
            *current = lastState;
            cout << "Undo successful! Product ID " << lastState.id << " restored.\n";
            saveStock();
            return;
        }
        current = current->next;
    }

    // If product doesn't exist (was deleted), add it back
    _addProduct(lastState.id, lastState.name, lastState.price,
               lastState.quantity, lastState.category);
    cout << "Undo successful! Product ID " << lastState.id << " restored.\n";
    saveStock();
}

void Stock::addRestockRequest(int id) {
    restockQueue.push(id);
    cout << "Restock request added for Product ID: " << id << endl;
}

void Stock::processRestockRequests() {
    if (restockQueue.empty()) {
        cout << "No restock requests in queue!\n";
        return;
    }

    while (!restockQueue.empty()) {
        int id = restockQueue.front();
        restockQueue.pop();

        Product* current = head;
        bool found = false;

        while (current != nullptr) {
            if (current->id == id) {
                found = true;
                // Save current state for possible undo
                undoStack.push(*current);

                // Restock logic
                int restockQty;
                cout << "Enter restock quantity for Product ID " << id << ": ";
                while (!(cin >> restockQty)) {
                    cout << "Invalid input. Please enter a number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                current->quantity += restockQty;
                cout << "Restocked " << restockQty << " units of Product ID " << id
                          << ". New quantity: " << current->quantity << endl;
                break;
            }
            current = current->next;
        }

        if (!found) {
            cout << "Product ID " << id << " not found. Restock skipped.\n";
        }
    }
    saveStock();
}