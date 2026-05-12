/// @file
/// @brief Test szablonów "bezpiecznych" rzutów.
/// @date 2026-05-12 (last modification)
///       ======================================
///
#include "safecast.hpp"
#include "ios_benders.h"
#include <iostream>
#include <string>
#include <iomanip>

using namespace merry_tools::streams;
using namespace merry_tools::math;

namespace merry_tools::tests {

void run_test(std::ostream& o,const std::string& label, auto action) {

    o << std::left << std::setw(40) << label << " -> ";
    try {
        action();
        o <<COLOR2<< "SUKCES" <<NOCOLO<< std::endl;
    } catch (const std::out_of_range& e) {
        o <<COLOR3<< "WYJĄTEK: " <<COLOR1<< e.what() <<NOCOLO<< std::endl;
    }
}

int test_safe_casts(std::ostream& o) {

    o << COLOR2;
    o << "====================================================================\n";
    o << "           URUCHAMIANIE TESTÓW BEZPIECZNEGO RZUTOWANIA              \n";
    o << "====================================================================\n\n";
    o << NOCOLO;

    // PRZYPADEK 0: Dodatnia liczba do typu bez znaku (unsigned)
    run_test(o,"0: int (11) do unsigned int", []() {
        int x = 11;
        SafeCaster<unsigned int>::cast(x);
    });

    // PRZYPADEK 1: Ujemna liczba do typu bez znaku (unsigned)
    run_test(o,"1: int (-1) do unsigned int", []() {
        int x = -1;
        SafeCaster<unsigned int>::cast(x);
    });

    // PRZYPADEK 2: Bardzo duże unsigned do małego signed
    run_test(o,"2: unsigned long long (max) do int", []() {
        unsigned long long x = std::numeric_limits<unsigned long long>::max();
        SafeCaster<int>::cast(x);
    });

    // PRZYPADEK 3: Zmiennoprzecinkowe poza zakresem int
    run_test(o,"3: double (1e15) do int", []() {
        double x = 1e15;
        SafeCaster<int>::cast(x);
    });

    // PRZYPADEK 4: Double ujemny do unsigned char
    run_test(o,"4: double (-10.5) do unsigned char", []() {
        double x = -10.5;
        SafeCaster<unsigned char>::cast(x);
    });

    // PRZYPADEK 5: Poprawne rzutowanie wewnątrz zakresu
    run_test(o,"5: double (120.5) do signed char", []() {
        double x = 120.5;
        signed char c = SafeCaster<signed char>::cast(x);
        std::cout << "\t(Wynik: " << (int)c << ") "<<std::endl;
    });
    
    double duzaLiczba = 1e12; //Try make it larger or smaller...
    
    try {
        // Funkcja która wygląda jak wbudowany operator!
        auto wynik = safe_cast<long double>(duzaLiczba);
        o <<COLOR2<< "Result: " << wynik <<NOCOLO<< std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr <<COLOR3<< "Error catch: " <<COLOR1<< e.what() <<NOCOLO<< std::endl;
    }

    try {
        // Funkcja która wygląda jak wbudowany operator!
        auto wynik = safe_cast<int>(duzaLiczba);
        o << "Result: " << wynik << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr <<COLOR3<< "Error catch: " <<COLOR1<< e.what() <<NOCOLO<< std::endl;
    }

    try {
        int ujemna = -5;
        auto wynik = safe_cast<unsigned int>(ujemna);
        o << "Result: " << wynik << std::endl;
    } catch (const std::out_of_range& e) {
        std::cerr <<COLOR3<< "Error catch: " <<COLOR1<< e.what() <<NOCOLO<< std::endl;
    }

    return true;
}

} //end of namespace

