#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>

using namespace std;

struct Product {
    int id;
    string name;
    double price;
    int quantity;
    string category;
    Product* prev;
    Product* next;
};

// Custom Stack implementation
class ProductStack {
private:
    struct Node {
        Product data;
        Node* next;
        Node(const Product& val) : data(val), next(nullptr) {}
    };
    Node* topNode;

public:
    ProductStack() : topNode(nullptr) {}
    ~ProductStack();
    
    void push(const Product& val);
    void pop();
    Product& top();
    bool empty() const { return topNode == nullptr; }
};

// Custom Queue implementation
class ProductQueue {
private:
    struct Node {
        int data;
        Node* next;
        Node(int val) : data(val), next(nullptr) {}
    };
    Node* frontNode;
    Node* backNode;

public:
    ProductQueue() : frontNode(nullptr), backNode(nullptr) {}
    ~ProductQueue();
    
    void push(int val);
    void pop();
    int front() const;
    bool empty() const { return frontNode == nullptr; }
};

class Stock {
private:
    Product* head;
    Product* tail;
    ProductStack undoStack;  // For undo functionality
    ProductQueue restockQueue;   // For restock requests

    void swapProductData(Product* a, Product* b);
    void _addProduct(int id, string name, double price, int quantity, string category);
    void _deleteProduct(int id);  // For undo operations

public:
    Stock();
    ~Stock();

    // Core operations
    void addProduct(int id, string name, double price, int quantity, string category);
    void viewProduct(const string& stock);
    void updateProduct(int id);
    void deleteProduct(int id);
    void searchProduct(int id);
    void sortByName();
    void sortById();
    void loadStock();
    void saveStock();

    // New features
    void undoLastOperation();
    void addRestockRequest(int id);
    void processRestockRequests();
};

#endif