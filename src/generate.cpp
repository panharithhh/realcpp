#include "../include/generatenum.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

std::string generatenum::computerGeneratedPassword() {
    int password[6];
    std::string returnPassword = "";
    for (int i = 0; i < 6; i++) {
        password[i] = rand() % 10;
        returnPassword += std::to_string(password[i]);
    }
    std::cout << "Registration Code: ";
    for (int i = 0; i < 6; i++) {
        std::cout << password[i];
    }
    std::cout << std::endl;
    return returnPassword;
}