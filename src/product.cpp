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

void Stock::_addProduct(int id, std::string name, double price, int quantity, std::string category) {
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
    std::swap(a->id, b->id);
    std::swap(a->name, b->name);
    std::swap(a->price, b->price);
    std::swap(a->quantity, b->quantity);
    std::swap(a->category, b->category);
}

void Stock::addProduct(int id, std::string name, double price, int quantity, std::string category) {
    // Save current state for undo
    Product snapshot{ id, name, price, quantity, category, nullptr, nullptr };
    undoStack.push(snapshot);

    _addProduct(id, name, price, quantity, category);
    saveStock();
    std::cout << "Product added successfully!\n";
}

void Stock::viewProduct(const std::string& stockFile) {
    loadStock();
    Product* current = head;
    std::cout << "\nID\tName\tPrice\tQty\tCategory\n";
    std::cout << "---------------------------------------------\n";
    while (current) {
        std::cout << current->id << "\t" << current->name << "\t" << current->price << "\t" << current->quantity << "\t" << current->category << "\n";
        current = current->next;
    }
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
            undoStack.push(*current); // save for undo
            _deleteProduct(id);
            saveStock();
            std::cout << "Product deleted.\n";
            return;
        }
        current = current->next;
    }
    std::cout << "Product not found.\n";
}

void Stock::updateProduct(int id) {
    Product* current = head;
    while (current) {
        if (current->id == id) {
            undoStack.push(*current);
            int newQty;
            double newPrice;
            std::cout << "Enter new quantity: ";
            std::cin >> newQty;
            std::cout << "Enter new price: ";
            std::cin >> newPrice;
            current->quantity = newQty;
            current->price = newPrice;
            saveStock();
            std::cout << "Product updated.\n";
            return;
        }
        current = current->next;
    }
    std::cout << "Product not found.\n";
}

void Stock::searchProduct(int id) {
    Product* current = head;
    while (current) {
        if (current->id == id) {
            std::cout << "Product found: " << current->name << " (Qty: " << current->quantity << ")\n";
            return;
        }
        current = current->next;
    }
    std::cout << "Product not found.\n";
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
    std::cout << "Products sorted by name.\n";
    saveStock();
}

void Stock::loadStock() {
    std::ifstream file("stock.csv");
    if (!file.is_open()) return; // no stock yet
    // clear existing list
    Product* current = head;
    while (current) {
        Product* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, name, priceStr, qtyStr, category;
        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, qtyStr, ',');
        std::getline(ss, category, ',');
        int id = std::stoi(idStr);
        double price = std::stod(priceStr);
        int qty = std::stoi(qtyStr);
        _addProduct(id, name, price, qty, category);
    }
    file.close();
}

void Stock::saveStock() {
    std::ofstream file("stock.csv");
    Product* current = head;
    while (current) {
        file << current->id << "," << current->name << "," << current->price << "," << current->quantity << "," << current->category << "\n";
        current = current->next;
    }
    file.close();
}


void Stock::undoLastOperation() {
    if (undoStack.empty()) {
        std::cout << "Nothing to undo!\n";
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
            std::cout << "Undo successful! Product ID " << lastState.id << " restored.\n";
            saveStock();
            return;
        }
        current = current->next;
    }

    // If product doesn't exist (was deleted), add it back
    _addProduct(lastState.id, lastState.name, lastState.price,
               lastState.quantity, lastState.category);
    std::cout << "Undo successful! Product ID " << lastState.id << " restored.\n";
    saveStock();
}

void Stock::addRestockRequest(int id) {
    restockQueue.push(id);
    std::cout << "Restock request added for Product ID: " << id << std::endl;
}

void Stock::processRestockRequests() {
    if (restockQueue.empty()) {
        std::cout << "No restock requests in queue!\n";
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
                std::cout << "Enter restock quantity for Product ID " << id << ": ";
                while (!(std::cin >> restockQty)) {
                    std::cout << "Invalid input. Please enter a number: ";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                current->quantity += restockQty;
                std::cout << "Restocked " << restockQty << " units of Product ID " << id
                          << ". New quantity: " << current->quantity << std::endl;
                break;
            }
            current = current->next;
        }

        if (!found) {
            std::cout << "Product ID " << id << " not found. Restock skipped.\n";
        }
    }
    saveStock();
}