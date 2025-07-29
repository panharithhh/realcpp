#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include "generatenum.h"
#include "product.h"

using namespace std;



int main() {
    srand(time(0));
    bool running = true;
    int choice;
    string email, password, re_entered_password, username, code;
    generatenum g;
    Stock stock;
    string generatedCode = g.computerGeneratedPassword();

    // Ensure required CSV files exist
    {
        ofstream f("data.csv", ios::app);
    }
    {
        ofstream f("stock.csv", ios::app);
    }

    while (running) {
        cout << "*** WELCOME TO STOCK MANAGEMENT SYSTEM ***\n";
        cout << R"(╦ ╦┌─┐┬  ┌─┐┌─┐┌┬┐┌─┐  ┌┬┐┌─┐  ╔═╗┌┬┐┌─┐┌─┐┬┌─  ╔╦╗┌─┐┌┐┌┌─┐┌─┐┌─┐┌┬┐┌─┐┌┐┌┌┬┐  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┌┬┐\n";
        cout << "║║║├┤ │  │  │ ││││├┤    │ │ │  ╚═╗ │ │ ││  ├┴┐  ║║║├─┤│││├─┤│ ┬├┤ │││├┤ │││ │   ╚═╗└┬┘└─┐ │ ├┤ │││\n";
        cout << "╚╩╝└─┘┴─┘└─┘└─┘┴ ┴└─┘   ┴ └─┘  ╚═╝ ┴ └─┘└─┘┴ ┴  ╩ ╩┴ ┴┘└┘┴ ┴└─┘└─┘┴ ┴└─┘┘└┘ ┴   ╚═╝ ┴ └─┘ ┴ └─┘┴ ┴\n)";
        cout << "\n\n\n\n";
        cout << "1. Login\n";
        cout << "2. Sign up\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        // Input validation for choice
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n\n";
            continue;
        }

        if (choice == 1) {
            cout << "Login\n";
            cout << "Enter your email: ";
            cin >> email;
            cout << "Enter your password: ";
            cin >> password;

            ifstream file("data.csv");
            if (!file.is_open()) {
                cout << "Error: Could not open data.csv. Please ensure the file exists.\n";
                continue;
            }

            string line;
            bool loggedIn = false;

            while (getline(file, line)) {
                stringstream ss(line);
                string file_email, file_password, file_username;
                getline(ss, file_email, ',');
                getline(ss, file_password, ',');
                getline(ss, file_username, ',');
                if (file_email == email && file_password == password) {
                    loggedIn = true;
                    cout << "Welcome myfriend " << file_username << "\n";
                    break;
                }
            }
            file.close();

            if (!loggedIn) {
                cout << "Wrong email or password.\n";
            }

            if (loggedIn) {
                int menuChoice;
                do {
                    cout << "\n*** WELCOME TO STOCK MANAGEMENT SYSTEM ***\n";
                    cout << "== Menu ==\n";
                    cout << "1. Add Product\n";
                    cout << "2. View Product\n";
                    cout << "3. Search Products\n";
                    cout << "4. Update Product\n";
                    cout << "5. Delete Product\n";
                    cout << "6. Restock Product\n";
                    cout << "7. Logout\n";
                    cout << "Please select an option: ";

                    // Input validation for menu choice
                    if (!(cin >> menuChoice)) {
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        cout << "Invalid input. Please enter a number.\n";
                        continue;
                    }

                    switch (menuChoice) {
                        case 1: {
                             int id, quantity; double price; string name, category;
                             cout << "Enter product ID: "; cin >> id;
                             cout << "Enter product name: "; cin >> name;
                             cout << "Enter product price: "; cin >> price;
                             cout << "Enter product quantity: "; cin >> quantity;
                             cout << "Enter product category: "; cin >> category;
                             stock.addProduct(id, name, price, quantity, category);
                             break; }
                        case 2:
                             stock.viewProduct("stock.csv");
                             break;
                        case 3: {
                             int id; cout << "Enter product ID to search: "; cin >> id;
                             stock.searchProduct(id);
                             break; }
                        case 4: {
                             int id; cout << "Enter product ID to update: "; cin >> id;
                             stock.updateProduct(id);
                             break; }
                        case 5: {
                             int id; cout << "Enter product ID to delete: "; cin >> id;
                             stock.deleteProduct(id);
                             break; }
                        case 6: {
                             int id; cout << "Enter product ID to restock: "; cin >> id;
                             stock.addRestockRequest(id);
                             stock.processRestockRequests();
                             break; }
                        case 7:
                            cout << "Logging out...\n";
                            break;
                        default:
                            cout << "Invalid choice. Please try again.\n";
                            break;
                    }
                } while (menuChoice != 7);
            }

        } else if (choice == 2) {
            cout << "Sign up\n";
            cout << "Enter your email: ";
            cin >> email;
            cout << "Enter your password: ";
            cin >> password;
            cout << "Enter your password again: ";
            cin >> re_entered_password;
            cout << "Enter a username: ";
            cin >> username;
            cout << "Enter the registration code: ";
            cin >> code;

            if (password != re_entered_password) {
                cout << "Passwords do not match.\n";
            } else if (code == generatedCode) {
                ofstream file("data.csv", ios::app);
                if (!file.is_open()) {
                    cout << "Error: Could not open data.csv for writing.\n";
                    continue;
                }
                file << email << "," << password << "," << username << "\n";
                file.close();
                cout << "Sign up successful.\n";
                generatedCode = g.computerGeneratedPassword();
            } else {
                cout << "Incorrect registration code.\n";
            }
        } else if (choice == 3) {
            running = false;
            cout << "Exiting program.\n";
        } else {
            cout << "Invalid choice. Please try again.\n";
        }
        cout << "\n";
    }

    return 0;
}