#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "generatenum.h"
#include "product.h"
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define BLUE    "\033[34m"
#define BOLD    "\033[1m"

void enableColors() {
    #ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    #endif
}

void printCentered(const string& text, int width = 80, const string& color = "") {
    int padding = (width - text.length()) / 2;
    if (padding > 0) cout << string(padding, ' ');
    cout << color << text << RESET << endl;
}

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

void displayHeader(const string& title) {
    clearScreen();
    cout << string(80, '=') << endl;
    printCentered(title, 80, BOLD CYAN);
    cout << string(80, '=') << endl << endl;
}

void displayMainMenu() {
    displayHeader("STOCK MANAGEMENT SYSTEM");
    cout << "\n";
    printCentered(BOLD "1. Login" RESET, 80, GREEN);
    printCentered(BOLD "2. Sign up" RESET, 80, YELLOW);
    printCentered(BOLD "3. Exit" RESET, 80, RED);
    cout << "\n";
    cout << BOLD "Enter your choice: " RESET;
}

void displayLoginForm() {
    displayHeader("LOGIN");
    cout << BOLD "Email: " RESET;
}

void displayProductMenu() {
    displayHeader("PRODUCT MANAGEMENT");
    cout << "\n";
    vector<string> options = {
        "1. Add Product",
        "2. View Products",
        "3. Search Products",
        "4. Update Product",
        "5. Delete Product",
        "6. Restock Product",
        "7. Sort Products By Name",
        "8. Sort Products By ID",
        "9. Undo Last Operation",
        "10. Logout"
    };
    for (const auto& option : options) {
        printCentered(option, 80, BOLD YELLOW);
    }
    cout << "\n";
    cout << BOLD "Please select an option: " RESET;
}

void displaySplash() {
    clearScreen();
    string lines[] = {
        BOLD CYAN "╦ ╦┌─┐┬  ┌─┐┌─┐┌┬┐┌─┐  ┌┬┐┌─┐  ╔═╗┌┬┐┌─┐┌─┐┬┌─  ╔╦╗┌─┐┌┐┌┌─┐┌─┐┌─┐┌┬┐┌─┐┌┐┌┌┬┐  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┌┬┐" RESET,
        BOLD CYAN "║║║├┤ │  │  │ ││││├┤    │ │ │  ╚═╗ │ │ ││  ├┴┐  ║║║├─┤│││├─┤│ ┬├┤ │││├┤ │││ │   ╚═╗└┬┘└─┐ │ ├┤ │││" RESET,
        BOLD CYAN "╚╩╝└─┘┴─┘└─┘└─┘┴ ┴└─┘   ┴ └─┘  ╚═╝ ┴ └─┘└─┘┴ ┴  ╩ ╩┴ ┴┘└┘┴ ┴└─┘└─┘┴ ┴└─┘┘└┘ ┴   ╚═╝ ┴ └─┘ ┴ └─┘┴ ┴" RESET
    };
    for (int i = 0; i < 3; ++i) {
        for (char c : lines[i]) {
            cout << c << flush;
            this_thread::sleep_for(chrono::milliseconds(2));
        }
        cout << endl;
    }
    this_thread::sleep_for(chrono::seconds(2));
}

void displayTableHeader() {
    cout << BOLD << string(80, '-') << RESET << endl;
    cout << BOLD << setw(8) << "ID" << setw(25) << "Name" 
         << setw(12) << "Price" << setw(12) << "Quantity" 
         << setw(20) << "Category" << RESET << endl;
    cout << BOLD << string(80, '-') << RESET << endl;
}

void displayProductRow(int id, const string& name, double price, int quantity, const string& category) {
    cout << setw(8) << id << setw(25) << name 
         << setw(12) << fixed << setprecision(2) << price 
         << setw(12) << quantity 
         << setw(20) << category << endl;
}

void showMessage(const string& message, const string& color = "", bool waitForEnter = false) {
    cout << color << message << RESET << endl;
    if (waitForEnter) {
        cout << "Press enter to continue...";
        cin.ignore();
        cin.get();
    } else {
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void handleAddProduct(Stock& stock) {
    int id, quantity; 
    double price; 
    string name, category;
    
    displayHeader("ADD NEW PRODUCT");
    cout << BOLD "Enter product ID: " RESET; 
    cin >> id;
    cout << BOLD "Enter product name: " RESET; 
    cin.ignore();
    getline(cin, name);
    cout << BOLD "Enter product price: " RESET; 
    cin >> price;
    cout << BOLD "Enter product quantity: " RESET; 
    cin >> quantity;
    cout << BOLD "Enter product category: " RESET; 
    cin.ignore();
    getline(cin, category);
    
    stock.addProduct(id, name, price, quantity, category);
    showMessage("\nProduct added successfully!", GREEN);
}

void handleViewProducts(Stock& stock) {
    displayHeader("PRODUCT INVENTORY");
    displayTableHeader();
    stock.viewProduct("stock.csv");
    showMessage("", RESET, true);
}

void handleSearchProduct(Stock& stock) {
    int id; 
    displayHeader("SEARCH PRODUCT");
    cout << BOLD "Enter product ID: " RESET; 
    cin >> id;
    
    displayHeader("SEARCH RESULTS");
    displayTableHeader();
    stock.searchProduct(id);
    showMessage("", RESET, true);
}

void handleUpdateProduct(Stock& stock) {
    int id; 
    displayHeader("UPDATE PRODUCT");
    cout << BOLD "Enter product ID to update: " RESET; 
    cin >> id;
    
    stock.updateProduct(id);
    showMessage("\nProduct updated successfully!", YELLOW);
}

void handleDeleteProduct(Stock& stock) {
    int id; 
    displayHeader("DELETE PRODUCT");
    cout << BOLD "Enter product ID to delete: " RESET; 
    cin >> id;
    
    stock.deleteProduct(id);
    showMessage("\nProduct deleted successfully!", RED);
}

void handleRestockProduct(Stock& stock) {
    int id; 
    displayHeader("RESTOCK PRODUCT");
    cout << BOLD "Enter product ID to restock: " RESET; 
    cin >> id;
    
    stock.addRestockRequest(id);
    stock.processRestockRequests();
    showMessage("\nRestock request processed!", GREEN);
}

void handleSortByName(Stock& stock) {
    stock.sortByName();
    showMessage("\nProducts sorted by name!", GREEN);
}

void handleSortById(Stock& stock) {
    stock.sortById();
    showMessage("\nProducts sorted by ID!", GREEN);
}

void handleUndo(Stock& stock) {
    stock.undoLastOperation();
    showMessage("\nLast operation undone!", YELLOW);
}

int main() {
    enableColors();
    displaySplash();
    srand(time(0));
    bool running = true;
    int choice;
    string email, password, re_entered_password, username, code;
    generatenum g;
    Stock stock;
    string generatedCode = g.computerGeneratedPassword();

    {
        ofstream f("data.csv", ios::app);
    }
    {
        ofstream f("stock.csv", ios::app);
    }

    while (running) {
        displayMainMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            showMessage("Invalid input. Please enter a number.", RED);
            continue;
        }

        if (choice == 1) {
            displayLoginForm();
            cin >> email;
            cout << BOLD "Enter your password: " RESET;
            cin >> password;

            ifstream file("data.csv");
            if (!file.is_open()) {
                showMessage("Error: Could not open data.csv. Please ensure the file exists.", RED);
                continue;
            }

            string line;
            bool loggedIn = false;
            string stored_username;

            while (getline(file, line)) {
                stringstream ss(line);
                string file_email, file_password, file_username;
                getline(ss, file_email, ',');
                getline(ss, file_password, ',');
                getline(ss, file_username, ',');
                if (file_email == email && file_password == password) {
                    loggedIn = true;
                    stored_username = file_username;
                    break;
                }
            }
            file.close();

            if (loggedIn) {
                showMessage("\nWelcome, " + stored_username + "!", GREEN);
                
                int menuChoice;
                do {
                    displayProductMenu();

                    if (!(cin >> menuChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        showMessage("Invalid input. Please enter a number.", RED);
                        continue;
                    }

                    switch (menuChoice) {
                        case 1: handleAddProduct(stock); break;
                        case 2: handleViewProducts(stock); break;
                        case 3: handleSearchProduct(stock); break;
                        case 4: handleUpdateProduct(stock); break;
                        case 5: handleDeleteProduct(stock); break;
                        case 6: handleRestockProduct(stock); break;
                        case 7: handleSortByName(stock); break;
                        case 8: handleSortById(stock); break;
                        case 9: handleUndo(stock); break;
                        case 10: showMessage("\nLogging out...", MAGENTA); break;
                        default: showMessage("Invalid choice. Please try again.", RED); break;
                    }
                } while (menuChoice != 10);
            } else {
                showMessage("Wrong email or password.", RED);
            }

        } else if (choice == 2) {
            displayHeader("SIGN UP");
            cout << BOLD "Enter your email: " RESET;
            cin >> email;
            cout << BOLD "Enter your password: " RESET;
            cin >> password;
            cout << BOLD "Re-enter your password: " RESET;
            cin >> re_entered_password;
            cout << BOLD "Enter a username: " RESET;
            cin >> username;
            cout << BOLD "Enter the registration code: " RESET;
            cin >> code;

            if (password != re_entered_password) {
                showMessage("\nPasswords do not match.", RED);
            } else if (code == generatedCode) {
                ofstream file("data.csv", ios::app);
                if (!file.is_open()) {
                    showMessage("Error: Could not open data.csv for writing.", RED);
                    continue;
                }
                file << email << "," << password << "," << username << "\n";
                file.close();
                showMessage("\nSign up successful!", GREEN);
                generatedCode = g.computerGeneratedPassword();
            } else {
                showMessage("\nIncorrect registration code.", RED);
            }
        } else if (choice == 3) {
            running = false;
            showMessage("\nExiting program. Goodbye!", MAGENTA);
        } else {
            showMessage("Invalid choice. Please try again.", RED);
        }
    }

    return 0;
}