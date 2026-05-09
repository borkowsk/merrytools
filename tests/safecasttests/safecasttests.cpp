/// @file
/// @date 2026-05-09 (last modification)
#include "safecast.hpp"
#include <iostream>
#include <string>
#include <iomanip>


void run_test(const std::string& label, auto action) {
    std::cout << std::left << std::setw(40) << label << " -> ";
    try {
        action();
        std::cout << "SUKCES" << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "WYJĄTEK: " << e.what() << std::endl;
    }
}

int main() {
    // PRZYPADEK 1: Ujemna liczba do typu bez znaku (unsigned)
    run_test("int (-1) do unsigned int", []() {
        int x = -1;/// @date 2026-05-09 (last modification)
        SafeCaster<unsigned int>::cast(x);
    });

    // PRZYPADEK 2: Bardzo duże unsigned do małego signed
    run_test("unsigned long long (max) do int", []() {
        unsigned long long x = std::numeric_limits<unsigned long long>::max();
        SafeCaster<int>::cast(x);
    });

    // PRZYPADEK 3: Zmiennoprzecinkowe poza zakresem int
    run_test("double (1e15) do int", []() {
        double x = 1e15;
        SafeCaster<int>::cast(x);
    });

    // PRZYPADEK 4: Double ujemny do unsigned char
    run_test("double (-10.5) do unsigned char", []() {
        double x = -10.5;
        SafeCaster<unsigned char>::cast(x);
    });

    // PRZYPADEK 5: Poprawne rzutowanie wewnątrz zakresu
    run_test("double (120.5) do signed char", []() {
        double x = 120.5;
        signed char c = SafeCaster<signed char>::cast(x);
        std::cout << "(Wynik: " << (int)c << ") ";
    });
    
    double duzaLiczba = 1e12; //Try make it larger or smaller...
    
    try {
        // Funkcja która wygląda jak wbudowany operator!
        int wynik = safe_cast<int>(duzaLiczba); 
        std::cout << "Result: " << wynik << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error catch: " << e.what() << std::endl;
    }

    try {
        int ujemna = -5;
        unsigned int wynik = safe_cast<unsigned int>(ujemna);
        std::cout << "Result: " << wynik << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error catch: " << e.what() << std::endl;
    }

    return 0;
}

