/** @file
 *  @brief Rust like numeric types and conversions.
 *  @date 2026-05-05 (last modification)
 *  @details
 *      Created by borkowsk on 05.05.2026.
 *      In helpful assistance of Gemini :-)
 *  @date 2026-05-05 (last modification)
 */
#ifndef RUST_LIKE_HPP
#define RUST_LIKE_HPP

#include <stdint.h>
#include <stddef.h>
#include <cmath>

#include <concepts>
#include <limits>
#include <stdexcept>

namespace rust_like
{
    /// @name Typy rustowe całkowite bez znaku (Unsigned Integers)
    /// @{
    typedef uint8_t   u8;
    typedef uint16_t  u16;
    typedef uint32_t  u32;
    typedef uint64_t  u64;
    #ifdef __SIZEOF_INT128__
    typedef unsigned __int128 u128; // Dostępne w GCC/Clang na platformach 64-bitowych
    #endif
    /// @}

    /// @name Typy rustowe całkowite ze znakiem (Signed Integers).
    /// @{
    typedef int8_t    i8;
    typedef int16_t   i16;
    typedef int32_t   i32;
    typedef int64_t   i64;
    /// @}

    /// @name  Typy rustowe zmiennoprzecinkowe (Floating Point)
    /// @{
    typedef float     f32;
    typedef double    f64;
    #ifdef __FLOAT128__
    typedef __float128 f128;
    #endif
    /// @})

    /// @name  Typy rustowe zależne od architektury (Pointer-sized Integers)
    typedef uintptr_t usize;
    typedef intptr_t  isize;
    /// @}

    /// @name Niestandardowe typy zmiennoprzecinkowe
    /// @{
    #if defined(__STDC_IEC_60559_TYPES__) && defined(__has_include)
        // Nowoczesny standard C23 / ISO _Float128
        typedef _Float128 f128;
    #define HAS_F128 1

    #elif defined(__FLOAT128__) || defined(__qfloat128)
        // Tradycyjne rozszerzenie GCC/Clang __float128
        typedef __float128 f128;

        /// Gdy 1 to wsparcie dla typu f128 obecne.
        #define HAS_F128 1

    #else
        /// Jeśli 0 to brak bezpośredniego wsparcia sprzętowego/kompilatora dla 128-bit float.
        /// @note możesz sam zmapować f128 na `long double` (jeśli ma 128 bitów)
        ///       lub po prostu nie definiować f128, aby zasygnalizować brak wsparcia.
        #define HAS_F128 0
    #endif
    /// @}

    /// Zapożyczone z rusta nazwa jawnej konwersji typów numerycznych.
    /// @details
    ///    __Zachowuje się jak zwykłe rzutowanie numeryczne w C/C++__
    ///    Jest to bezpieczne (nie spowoduje undefined behavior), ale może
    ///    prowadzić do utraty danych lub przepełnienia, o czym musisz pamiętać.
    ///    * Promocja typu (konwersja bezstratna): Przejście z mniejszego typu do
    ///      większego (np. u8 do u32) jest zawsze bezpieczne.
    ///    * Democja typu (konwersja stratna): Przy przejściu z większego typu do
    ///      mniejszego (np. u32 do u8), "as" po prostu "obetnie" nadmiarowe bity
    ///      (zastosuje modulo $2^n$).
    ///    * Konwersja zmiennoprzecinkowych na całkowite: Odcięta zostanie część ułamkowa.
    ///      Jeśli wartość nie mieści się w typie docelowym, zostanie nasycona
    ///      (przyjmie najbliższą graniczną wartość typu, np. maksymalną możliwą dla
    ///      danego typu całkowitego).
    ///    * Konwersja pomiędzy typami całkowitymi o tym samym rozmiarze, ale różnej
    ///      znakowości, jest bardzo szybkie i proste - kompilator wykonuje tak zwany
    ///      bitcast (reinterpretację bitów).
    ///    * Jeżeli za pomocą jednego `as` zmieniasz zarówno znakowość, jak i rozmiar typu
    ///      (np. i8 as u16), kompilator wykonuje operację dwuetapowo zgodnie z następującymi
    ///      regułami:
    ///      -  Rozszerzanie (np. i8 as u16): Najpierw następuje rozszerzenie rozmiaru ze
    ///         znakiem (tzw. sign extension, czyli powielenie bitu znaku, by zachować wartość
    ///         liczbową), a dopiero potem reinterpretacja na typ bez znaku.
    ///         ```
    ///           i8 o wartości -1 (1111 1111) przy konwersji do 16 bitów staje się
    ///           1111 1111 1111 1111 (nadal -1 w i16), co po reinterpretacji na u16 daje 65535.
    ///         ```
    ///      -  Zwężanie (np. u16 as i8): Najpierw następuje ucięcie nadmiarowych bita do żądanego
    ///         rozmiaru docelowego (truncation), a uzyskany wynik jest interpretowany jako
    ///         wartość ze znakiem.
    ///     * Zachowanie przy przepełnieniu (Floating-point to Integer) to najbardziej namacalna
    ///       różnica w działaniu między "as" rusta a static_cast C++. Pojawia się podczas konwersji
    ///       liczby zmiennoprzecinkowej (float/double) na liczbę całkowitą, gdy wartość wykracza
    ///       poza zakres typu docelowego. Dlatego musi być to zdefiniowane osobno.
    template<class TARGET_T,class SOURCE_T>
        requires std::integral<SOURCE_T> && std::integral<TARGET_T>
    TARGET_T as(SOURCE_T value)
    {
        return static_cast<TARGET_T>(value);
    }

    /// Implementacja rzutowania z typu zmiennoprzecinkowego na całkowity.
    /// Zgodnie z behawiorem rusta funkcja wykonuje tzw. nasycenie (saturating cast):
    /// - Jeśli wartość jest za duża, zostanie przycięta do maksymalnej wartości typu docelowego (255).
    /// - Jeśli wartość jest za mała (ujemna), zostanie przycięta do minimalnej wartości (0).
    /// - Jeśli wartością jest NaN, konwersja zwróci 0.
    /// Zgodnie ze standardem C++, rzutowanie wartości zmiennoprzecinkowej poza zakres typu całkowitego
    /// (lub wartości NaN/Infinity) to Undefined Behavior (UB). Kompilator może wygenerować dowolny kod – od zwrócenia losowych śmieci z rejestru, przez dziwne zachowanie programu, aż po usunięcie całej gałęzi kodu w ramach optymalizacji.
    /// @note Pobieramy limity typu docelowego jako typ zmiennoprzecinkowy,
    ///       aby uniknąć przepełnienia podczas porównywania.
    template<typename TARGET_T, typename SOURCE_T>
        requires std::floating_point<SOURCE_T> && std::integral<TARGET_T>
    TARGET_T as(SOURCE_T value)
    {
        // 1. Obsługa wartości NaN -> w Rust daje to 0
        if (std::isnan(value)) {
            return static_cast<TARGET_T>(0);
        }

        // 2. Pobieramy limity typu docelowego jako typ zmiennoprzecinkowy,
        // aby uniknąć przepełnienia podczas porównywania.
        constexpr SOURCE_T target_max = static_cast<SOURCE_T>(std::numeric_limits<TARGET_T>::max());
        constexpr SOURCE_T target_min = static_cast<SOURCE_T>(std::numeric_limits<TARGET_T>::min());

        // 3. Nasycenie (Saturating cast)
        if (value >= target_max) {
            return std::numeric_limits<TARGET_T>::max();
        }
        if (value <= target_min) {
            return std::numeric_limits<TARGET_T>::min();
        }

        // 4. Bezpieczna konwersja (wartość mieści się w zakresie)
        return static_cast<TARGET_T>(value);
    }

    /// Implementacja konwersji z typu całkowitego na zmiennoprzecinkowy.
    /// Sprawdzamy czy nie dochodzi do utraty precyzji, ale jest to kosztowne.
    /// Na szczęćie możemy sprawdzić W CZASIE KOMPILACJI, czy liczba bitów typu źródłowego
    /// jest mniejsza lub równa liczbie bitów mantysy typu docelowego.
    /// (Dla typów ze znakiem odejmujemy 1 bit znaku z ogólnego rozmiaru. Kluczem do
    /// zrozumienia tego jest to, jak standard C++ definiuje pole digits dla typów
    /// całkowitych ze znakiem i bez znaku.)
    template<typename TARGET_T, typename SOURCE_T>
        requires std::integral<SOURCE_T> && std::floating_point<TARGET_T>
    TARGET_T as(SOURCE_T value)
    {
        constexpr int source_bits = std::numeric_limits<SOURCE_T>::digits;
        constexpr int target_mantissa_bits = std::numeric_limits<TARGET_T>::digits;

        if constexpr (source_bits <= target_mantissa_bits)
        {
            // KOMPILATOR WYGENERUJE TYLKO TĘ LINIJKĘ (Zero overhead!):
            return static_cast<TARGET_T>(value);
        }
        else
        {
            // Dokonujemy próbnej konwersji
            TARGET_T converted = static_cast<TARGET_T>(value);

            // Rzutujemy z powrotem na typ całkowity, aby sprawdzić czy wartość się zmieniła
            SOURCE_T reverted = static_cast<SOURCE_T>(converted);

            if (value != reverted) {
                // Opcja A: Rzucenie wyjątku (najbezpieczniejsze podejście w C++)
                throw std::runtime_error("Precision lost!");

                // Opcja B: Jeśli nie używasz wyjątków, możesz np. zalogować błąd
                // i po prostu zwrócić zaokrągloną wartość 'converted'.
            }

            return converted;
        }
    }

    // ============================================================================
    // POMOCNICZY SZABLON DO OPÓŹNIENIA ASERCJI STATYCZNEJ PRZY CHWYTANIU BŁĘDÓW.
    // Zawsze równy "false".
    // ============================================================================
    template <typename...>
    inline constexpr bool Houston_we_have_a_problem = false;

    /// PRZECHWYCENIE BŁĘDÓW (Domyślny szablon wywoływany przy niepasujących typach).
    /// @details
    /// Jak poprawnie dostosować szablon "as" do własnych klas?
    /// Jeśli chcesz, aby szablon "as" obsługiwał również twoje własne, zaawansowane typy matematyczne,
    /// musisz stworzyć dla nich osobne przeciążenie szablonu. Możesz to zrobić w duchu C++20, tworząc
    /// swój własny, bardziej ogólny koncept (np. "Number"), który pozwala na konwersje:
    /// ```C++
    ///        #include <concepts>
    ///
    ///        // Definiujemy koncept typu, który można rzutować za pomocą static_cast
    ///        template<typename From, typename To>
    ///            concept ExplicitlyConvertibleTo = requires(From f) {
    ///                static_cast<To>(f);
    ///        };
    ///
    ///        // Nowe przeciążenie dla niestandardowych klas matematycznych
    ///        template<typename TARGET_T, typename SOURCE_T>
    ///            requires ExplicitlyConvertibleTo<SOURCE_T, TARGET_T>
    ///                     && (!std::integral<SOURCE_T> && !std::floating_point<SOURCE_T>) // Wykluczamy wbudowane
    ///        TARGET_T as(SOURCE_T value) {
    ///            // Tutaj definiujesz bezpieczną logikę rzutowania dla klas typu "Ułamek" czy "Kąt"
    ///            // np. wywołując dedykowaną metodę rzutowania z Twojej klasy.
    ///            //...
    ///            return static_cast<TARGET_T>(value);
    ///        }
    /// ```
    template<typename TARGET_T, typename SOURCE_T>
    TARGET_T as(SOURCE_T value) {
        // Sprawdzamy konkretne kombinacje, aby dać programiście precyzyjną wskazówkę:
        if constexpr (std::floating_point<SOURCE_T> && std::floating_point<TARGET_T>) {
            static_assert(Houston_we_have_a_problem<SOURCE_T, TARGET_T>,
                  "\n\n[BŁĄD SZABLONU as()]: Próbujesz konwertować typ zmiennoprzecinkowy na inny zmiennoprzecinkowy (np. double -> float).\n"
                  "Użyj standardowego static_cast, ponieważ ta operacja nie wymaga nasycania ani ochrony przed utratą precyzji całkowitej.\n");
        }
        else if constexpr (!std::integral<SOURCE_T> && !std::floating_point<SOURCE_T>) {
            static_assert(Houston_we_have_a_problem<SOURCE_T, TARGET_T>,
                  "\n\n[BŁĄD SZABLONU as()]: Typ źródłowy (SOURCE_T) nie jest wbudowanym typem numerycznym (int/float/double).\n"
                  "Szablon as() wspiera wyłącznie natywne typy numeryczne C++, chyba że sam zdefinujesz implementacje.\n");
        }
        else if constexpr (!std::integral<TARGET_T> && !std::floating_point<TARGET_T>) {
            static_assert(Houston_we_have_a_problem<SOURCE_T, TARGET_T>,
                  "\n\n[BŁĄD SZABLONU as()]: Typ docelowy (TARGET_T) nie jest wbudowanym typem numerycznym (int/float/double).\n"
                  "Szablon as() wspiera wyłącznie natywne typy numeryczne C++, chyba że sam zdefinujesz implementacje.\n");
        }
        else {
            static_assert(Houston_we_have_a_problem<SOURCE_T, TARGET_T>,
                  "\n\n[BŁĄD SZABLONU as()]: Próba wykonania nieobsługiwanej konwersji pomiędzy podanymi typami.\n"
                  "Sprawdź poprawność przekazywanych parametrów szablonu.\n");
        }

        return 0;
    }
};
#endif // RUST_LIKE_HPP
