/// @file
/// @brief Test suite for `merry_tools`
/// @date 2026-05-05 (modification)
#include "mth_vectors.h"
#include "mth_fix_float.h"
#include "ios_benders.h"
#include "mem_guard.h"
#include "mem_unique_val.h"
#include "lgc_boolean.h"
#include "rust_like.hpp"

#include <iostream>

using namespace ::merry_tools::logic;
using namespace ::merry_tools::math;
using namespace ::merry_tools::memory;
using namespace ::merry_tools::iostreams;

UNIQUE_GLOBAL(unsigned,unsigned,uONE,1) ///< Global `unique_val` for `unsigned` "hierarchy" and value 1.
UNIQUE_GLOBAL(UFloat16,unsigned,ONE,1)  ///< Global `unique_val` for `UFloat16` hierarchy and value 1.
UNIQUE_GLOBAL(UFloat16,unsigned,TWO,2)  ///< Global `unique_val` for `UFloat16` hierarchy and value 2.

namespace merry_tools::tests {
    void print_namespace_variables(std::ostream& o);

    // Also declared in test_uniques, but with different IN_CLASS_UNIQUE 👅
    class dummy2 {
    public:
        lgc_boolean good;
//        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,FIVE,5)
//        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,TEN,10)
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,ELEV,11)
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,TWELF,12)
    };

    DEFINE_IN_CLASS_UNIQUE(dummy2,::merry_tools::math::UFloat16,unsigned,ELEV,11) // Required definition completing declaration in class.
    DEFINE_IN_CLASS_UNIQUE(dummy2,::merry_tools::math::UFloat16,unsigned,TWELF,12) // Required definition completing declaration in class.

    UFloat16 fl=123.5;
    unsigned one=::ONE;

    bool test_uniques_val(std::ostream& o)
    {
        o<<NOCOLO<<"\n"<<COLOR2<<"Now tests for unique values..."<<COLERR<<std::endl;

        auto one_=::get(ONE);
        auto two_=::get(TWO);
        o<<one<<','<<one_<<','<<two_<<std::endl;
        print_namespace_variables(o);

        o<<COLOR2<<"END OF tests for unique values."<<NOCOLO<<std::endl;
        return one_==1 && two_==2;
    }

    bool test_ios_benders(std::ostream& o)
    {
        static unsigned testUInt=0xf0f0f0f;
        guard<u_int16_t,0xface> memGuard;

        o<<NOCOLO<<"\n"<<COLOR2<<"Now tests for stream bending..."<<COLERR<<std::endl;
        text_at_end at_end(o, "Stream benders thank you for your attention\n\n");

        ios_teacher funScope(std::cerr);

        o << funScope << COLOR5 << "Normal print of int as dec:" << COLOR3 << testUInt << NOCOLO << std::endl;
        {
            o<<COLOR4<<"An internal block begins!"<<NOCOLO<<std::endl;
            ios_teacher locScope(std::cerr, "BENDER111");
            keep_io_flags keep_flags_of(o);
            o << ios_teacher("BENDER222") << text_at_end("Hex print END\n") << COLOR5 << "Hex print:" << COLOR3 << std::hex << std::showbase << std::setw(24) << testUInt << NOCOLO << std::endl;
            o<<COLOR4<<"Now the block will end!"<<NOCOLO<<std::endl;
        }
        o<<COLOR5<<"After restore print:"<<COLOR3<<testUInt<<NOCOLO<<std::endl;
        o<<COLOR5<<keep_io_flags()<<"Octal? "<<std::oct<<std::showbase<<COLOR3<<testUInt<<NOCOLO<<std::endl;
        o<<COLOR5<<"Again default:"<<COLOR3<<testUInt<<NOCOLO<<std::endl;
        o<<COLOR2<<"END OF tests for stream benders."<<NOCOLO<<std::endl;

        return true;
    }

    bool test_vectors_bending(std::ostream& o)
    {
        keep_io_flags keep_flags_of(o);
        o<<NOCOLO<<"\n"<<COLOR2<<"Now tests for vectors bending..."<<COLERR<<std::endl;
        text_at_end at_end(o, "Vectors benders thank you for your attention\n\n");

        auto time1=-1_s;
        auto timePeriod1=TimeSpan(time1);
        auto prn=timePeriod1.val;
        o<<"time1:"<<time1<<std::endl;
        o<<"timePeriod1:"<<prn<<std::endl;

        auto time2=10.0_s;
        TimeSpan timePeriod2=time2;
        TimeSpan timePeriod3=333_s;
        TimeSpan timePeriod4=xD(33.5_s,on_time);
        auto timePeriod5=xD(33.5_s);

        auto tsi=TimeSI{1_s}+TimeSI{2_s};

        auto mass1=5.5_kg;
        o<<"mass1:"<<mass1<<std::endl;

        auto mass1d=xD(mass1/3.);
        MassQuan mass1q= mass1;
        auto mass2=3_kg;
        auto mass2q=MassQuan{3_kg};
        MassQuan mass3q=33.5_kg;
        auto mass4q=xD(100_kg);
        if(mass4q!=100_kg) return false;

        MassQuan mass5q=xD(99.9_kg,on_mass);
        MassQuan mass6=mass5q/3.0;
        if(mass5q/3 != mass6 ) return false;

        auto temp=280_K;
        o<<"temp:"<<temp<<std::endl;

        auto tempQ=xD(temp);
        TempQuan tempr=xD(100_K,on_temperature);

        auto dist1=1_m;
        auto dist1X=Longitude(dist1);
        auto dist2=-100.5_m;
        auto dist2X=xD(dist2,is_along);
        auto dist3X=xD(5_m,is_along);

        auto vel1=10_m_s;
        o<<"vel1:"<<vel1<<std::endl;

        auto vel1X=VelAlong{vel1};
        auto vel2=99.99_m_s;
        auto vel2X=xD(vel2,is_along);

        auto acc1=1_m_s2;
        o<<"acc1:"<<acc1<<std::endl;

        auto acc1X=xD(acc1,is_along);

        PlanePosition point0{dist1X,Latitude{1_m}};
        auto point1=xD(dist2X,Latitude{7.7_m});
        auto test1=xD(Latitude{2_m},Altitude{3_m}); // The non-standard plane is accepted but has not predefined type.
        //auto test2=xD(Altitude{2_m},Altitude{3_m}); //fail on static_assert( !strings_equal(AXIS1::name(),AXIS2::name()) ); //Axes need to be different!

        PlaneVelocity velP0{vel2X,vel2};
        auto velP1=xD(VelAlong{10_m_s},VelAcross{1.59_m_s});

        PlaneAcceleration accP0{acc1X,AccAcross{2_m_s2}};
        //auto accP1=xD(AccAlong{2.33_m_s2},acc1);

        VolumeVelocity vel3D{VelAlong{5_m_s},VelAcross{1.22_m_s},VelUpward{0.33_m_s}};
        auto vol3D2=xD(vel1X,vel2,VelUpward{-0.55_m_s});
        auto vol3D3=xD(velP1,1_m_s);
        //auto vol3D3=xD(accP1,1_m_s); //type mismatch

        auto res0=point1-point0;
        auto res1=velP0+velP1;
        auto res2=vol3D2+vol3D3;
        auto res3=vol3D2-vol3D3;
        auto res4=Longitude{1_m}+Latitude{1_m};
        auto res5=Latitude{1_m}+Longitude{1_m}; //Should be reverse order allowed?
        auto res10=velP0+VelUpward(1_m_s);
        auto res11=res10-res3;
        //auto res4=velP1+Altitude{1_m}/TimeSpan{1_s};

        o<<COLOR2<<"END OF tests for vectors benders."<<NOCOLO<<std::endl;
        return true;
    }

    // ============================================================================
    // WSTĘPNA FUNKCJA TESTUJĄCA DLA rust_like
    // ============================================================================
    bool test_rust_like(std::ostream& o)
    {
        using namespace rust_like;
        using namespace std;
#if HAS_F128
        f128 kosmiczna_precyzja = 3.14159265358979323846264338327950288Q;
        // Uwaga: przy __float128 używa się przyrostka 'Q' (np. 1.0Q) zamiast 'f' czy 'L'
        o<<"Wsparcie dla f128 jest aktywne!<\n"<<endl;
#else
        o<<"Ta platforma nie obsługuje natywnie 128-bitowych liczb zmiennoprzecinkowych."<<endl;
#endif
        // Przeciążenie 1: Integral -> Integral
        u8 a = as<u8>(300u); // Standardowe obcięcie bitów (300 % 256) -> wynik: 44
        o << "300 as u8 = " << (int)a << std::endl;

        // Przeciążenie 2: Float -> Integral
        u8 b = as<u8>(300.5f); // Wartość > 255 nasyca do max -> wynik: 255
        u8 c = as<u8>(-50.0);  // Wartość < 0 nasyca do min -> wynik: 0

        o << "300.5f as u8 = " << (int)b << std::endl;
        o << "-50.0 as u8  = " << (int)c << std::endl;

        try {
            // 1. Dokładna konwersja (mieści się w 24 bitach mantysy float)
            int64_t bezpieczna = 16777215; // 2^24 - 1
            float f1 = as<float>(bezpieczna);
            o << "OK: " << f1 << "\n"; // Zadziała bez problemu

            // 2. Konwersja ze stratą precyzji (wymaga więcej niż 24 bity)
            int64_t niebezpieczna = 16777217; // 2^24 + 1
            float f2 = as<float>(niebezpieczna); // Rzuci wyjątek!
            o << "To się nie wypisze: " << f2 << "\n";

        } catch (const std::exception& e) {
            o << "ERROR: " << e.what() << "\n";
        }

        // BŁĘDY KOMPILACJI:
        // /////////////////

        /// Przykład A: Próba rzutowania double -> float
        //float f = as<float>(3.14);

        /// Przykład B: Próba użycia własnego typu
        UFloat16 obiekt;
        int x = as<int>(obiekt);

        return true;
    }

    // ============================================================================
    // GŁÓWNA FUNKCJA TESTUJĄCA DLA rust_like
    // ============================================================================
    bool test_rust_like2(std::ostream& o)
    {
        using namespace rust_like;
        using namespace std;

        bool caly_test_ok = true;
        int numer_testu = 1;

        o << "====================================================================\n";
        o << "             URUCHAMIANIE TESTOW NUMERYCZNYCH 'as<T>()'            \n";
        o << "====================================================================\n\n";

        // Lambda ułatwiająca formatowanie i zbieranie wyników
        auto raportuj = [&](const char* nazwa, bool warunek_sukcesu, const std::string& info) {
            o << "Test #" << numer_testu++ << " [" << nazwa << "]: ";
            if (warunek_sukcesu) {
                o << "[ OK ]\n   -> " << info << "\n";
            } else {
                o << "[ BLAD ]\n   -> " << info << "\n";
                caly_test_ok = false;
            }
            o << "--------------------------------------------------------------------\n";
        };

        // --- TEST 1: Reinterpretacja bitowa (i8 na u8) ---
        {
            i8 wejscie = -1;
            u8 wynik = as<u8>(wejscie);
            bool ok = (wynik == 255);
            raportuj("Bitcast (i8 -> u8)", ok,
                     "Wejscie: " + std::to_string(wejscie) +
                         " | Wynik: " + std::to_string(wynik) + " (Oczekiwano: 255)");
        }

        // --- TEST 2: Obcinanie bitów / Modulo (u16 na u8) ---
        {
            u16 wejscie = 300;
            u8 wynik = as<u8>(wejscie);
            bool ok = (wynik == 44); // 300 % 256 = 44
            raportuj("Modulo (u16 -> u8)", ok,
                     "Wejscie: " + std::to_string(wejscie) +
                         " | Wynik: " + std::to_string(wynik) + " (Oczekiwano: 44)");
        }

        // --- TEST 3: Nasycenie w górę (double na u8) ---
        {
            double wejscie = 500.5;
            u8 wynik = as<u8>(wejscie);
            bool ok = (wynik == 255);
            raportuj("Nasycenie gora (f64 -> u8)", ok,
                     "Wejscie: " + std::to_string(wejscie) +
                         " | Wynik: " + std::to_string(wynik) + " (Oczekiwano: 255)");
        }

        // --- TEST 4: Nasycenie w dół (float na i8) ---
        {
            float wejscie = -1000.f;
            i8 wynik = as<i8>(wejscie);
            bool ok = (wynik == -128);
            raportuj("Nasycenie dol (f32 -> i8)", ok,
                     "Wejscie: " + std::to_string(wejscie) +
                         " | Wynik: " + std::to_string(wynik) + " (Oczekiwano: -128)");
        }

        // --- TEST 5: Obsługa wartości NaN ---
        {
            double wejscie = std::numeric_limits<double>::quiet_NaN();
            i32 wynik = as<i32>(wejscie);
            bool ok = (wynik == 0);
            raportuj("Obsluga NaN (f64 -> i32)", ok,
                     "Wejscie: NaN | Wynik: " + std::to_string(wynik) + " (Oczekiwano: 0)");
        }

        // --- TEST 6: Konwersja bezstratna (i16 na float - compile-time path) ---
        {
            i16 wejscie = 32000;
            float wynik = as<float>(wejscie);
            bool ok = (wynik == 32000.0f);
            raportuj("Bezstratna (i16 -> f32)", ok,
                     "Wejscie: " + std::to_string(wejscie) +
                         " | Wynik: " + std::to_string(wynik) + " (Oczekiwano: 32000.0)");
        }

        // --- TEST 7: Na granicy precyzji - SUKCES (i64 na float) ---
        {
            i64 wejscie = 16777215; // 2^24 - 1 (Miesci sie w 24-bitowej mantysie f32)
            bool wyjatek = false;
            float wynik = 0;
            try {
                wynik = as<float>(wejscie);
            } catch (const std::exception&) {
                wyjatek = true;
            }
            bool ok = (!wyjatek && wynik == 16777215.0f);
            raportuj("Na granicy mantysy - Sukces (i64 -> f32)", ok,
                     "Wejscie: " + std::to_string(wejscie) +
                         " | Wynik: " + std::to_string(wynik) + " (Oczekiwano: Bez wyjatku)");
        }

        // --- TEST 8: Na granicy precyzji - OCZEKIWANY BŁĄD (i64 na float) ---
        {
            i64 wejscie = 16777217; // 2^24 + 1 (Wykracza poza 24-bitowa mantyse f32)
            bool wyjatek = false;
            std::string blad_msg = "";
            try {
                as<float>(wejscie);
            } catch (const std::exception& e) {
                wyjatek = true;
                blad_msg = e.what();
            }
            bool ok = wyjatek;
            raportuj("Przekroczenie mantysy - Blad (i64 -> f32)", ok,
                     "Wejscie: " + std::to_string(wejscie) +
                         " | " + (wyjatek ? "Zlapano oczekiwany wyjatek: \"" + blad_msg + "\"" : "BLAD: Wyjatek nie zostal rzucony!"));
        }

        // --- PODSUMOWANIE ---
        o << "\n====================================================================\n";
        if (caly_test_ok) {
            o << "   STATUS KONCOWY: WSZYSTKIE TESTY ZAKONCZONE SUKCESEM [ OK ]       \n";
        } else {
            o << "   STATUS KONCOWY: NIEKTÓRE TESTY ZAKONCZYLY SIE BLĘDEM [ FAIL ]    \n";
        }
        o << "====================================================================\n";

        return caly_test_ok;
    }

} // tests namespace

int main() {
    using namespace merry_tools::tests;

    std::cout << "Hello, World!" << std::endl << std::flush;

    dummy2 testDummy2;

    if(!test_ios_benders(std::clog) ) return 1;
    if(!test_vectors_bending(std::clog)) return 2;
    if(!test_uniques_val(std::clog)) return 3;
    if(!test_rust_like(std::clog)) return 4;
    if(!test_rust_like2(std::clog)) return 4;

    testDummy2.good=true;  //Valid
    //testDummy2.good=1;   //Not valid
    //testDummy2.good=lgc_boolean{1};  //Not valid
    //testDummy2.good=1.0; //Not valid
    //testDummy2.good=lgc_boolean{1.0};  //Not valid
    //testDummy2.good= nullptr;  //Not valid
    //testDummy2.good=lgc_boolean{ &testDummy2 };  //Not valid

    std::clog.flush(); std::cout.flush();

    std::cout << "SUCCESS! " <<(testDummy2.good?"True":"False")<< std::endl;
    return 0;
}
