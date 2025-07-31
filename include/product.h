#ifndef PRODUCT_H
#define PRODUCT_H

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

template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* topNode;

public:
    Stack() : topNode(nullptr) {}
    ~Stack() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        newNode->next = topNode;
        topNode = newNode;
    }

    void pop() {
        if (empty()) return;
        Node* temp = topNode;
        topNode = topNode->next;
        delete temp;
    }

    T top() const {
        if (empty()) {
            throw runtime_error("Stack is empty");
        }
        return topNode->data;
    }

    bool empty() const {
        return topNode == nullptr;
    }
};

template <typename T>
class Queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };
    Node* frontNode;
    Node* backNode;

public:
    Queue() : frontNode(nullptr), backNode(nullptr) {}
    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            frontNode = backNode = newNode;
        } else {
            backNode->next = newNode;
            backNode = newNode;
        }
    }

    void pop() {
        if (empty()) return;
        Node* temp = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) {
            backNode = nullptr;
        }
        delete temp;
    }

    T front() const {
        if (empty()) {
            throw runtime_error("Queue is empty");
        }
        return frontNode->data;
    }

    bool empty() const {
        return frontNode == nullptr;
    }
};

class Stock {
private:
    Product* head;
    Product* tail;
    Stack<Product> undoStack;
    Queue<int> restockQueue;

    void swapProductData(Product* a, Product* b);
    void _addProduct(int id, string name, double price, int quantity, string category);
    void _deleteProduct(int id);

public:
    Stock();
    ~Stock();

    void addProduct(int id, string name, double price, int quantity, string category);
    void viewProduct(const string& stock);
    void updateProduct(int id);
    void deleteProduct(int id);
    void searchProduct(int id);
    void sortByName();
    void sortById();
    void loadStock();
    void saveStock();

    void undoLastOperation();
    void addRestockRequest(int id);
    void processRestockRequests();
};

#endif