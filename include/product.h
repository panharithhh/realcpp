#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <stack>
#include <queue>
#include <string>

using namespace std;

struct Product {
    int id;
    std::string name;
    double price;
    int quantity;
    std::string category;
    Product* prev;
    Product* next;
};

class Stock {
private:
    Product* head;
    Product* tail;
    std::stack<Product> undoStack;  // For undo functionality
    std::queue<int> restockQueue;   // For restock requests

    void swapProductData(Product* a, Product* b);
    void _addProduct(int id, std::string name, double price, int quantity, std::string category);
    void _deleteProduct(int id);  // For undo operations

public:
    Stock();
    ~Stock();

    // Core operations
    void addProduct(int id, std::string name, double price, int quantity, std::string category);
    void viewProduct(const std::string& stock);
    void updateProduct(int id);
    void deleteProduct(int id);
    void searchProduct(int id);
    void sortByName();
    void loadStock();
    void saveStock();

    // New features
    void undoLastOperation();
    void addRestockRequest(int id);
    void processRestockRequests();
};

#endif