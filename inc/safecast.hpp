/// @prototyped by Gemini
/// Kluczowe mechanizmy działania: Porównanie Signed vs Unsigned: Najgroźniejszym błędem w C++ jest porównanie if (-1 < 1u). W surowym kodzie wynik to false, ponieważ -1 jest konwertowane na ogromną liczbę dodatnią. Nasz szablon dzięki if constexpr (std::is_signed_v<Source> && !std::is_signed_v<Target>) wyłapuje to ryzyko przed wykonaniem porównania.std::numeric_limits<T>::lowest(): Jest kluczowe dla typów zmiennoprzecinkowych. Dla float, min() zwraca najmniejszą liczbę dodatnią (bliską zeru), a lowest() zwraca faktyczną granicę ujemną (np. $-3.4 \times 10^{38}$).Wydajność: Dzięki zastosowaniu if constexpr, wszystkie te sprawdzenia "logiczne" dzieją się w trakcie kompilacji. Wygenerowany kod maszynowy zawiera tylko te instrukcje if, które są sensowne dla danej pary typów.Dlaczego to jest lepsze niż static_cast?static_cast w C++ nie wykonuje żadnych sprawdzeń w czasie wykonywania programu. Jeśli rzutujesz 300 na unsigned char, otrzymasz po prostu 44 (wynik ucięcia bitów), a program nie poinformuje Cię o błędzie. SafeCaster gwarantuje integralność danych.
/// @date 2026-05-09 (last modification)

#include <limits>
#include <stdexcept>
#include <type_traits>

template <typename Target>
class SafeCaster {
public:
    template <typename Source>
    static Target cast(Source value) {
        using T_lim = std::numeric_limits<Target>;
        using S_lim = std::numeric_limits<Source>;

        // 1. Sprawdzenie dolnej granicy
        if constexpr (std::is_signed_v<Source> && !std::is_signed_v<Target>) {
            // Jeśli źródło jest ujemne, a cel bez znaku -> błąd
            if (value < 0) throw std::out_of_range("Negative to unsigned cast");
        } else {
            // Standardowe sprawdzenie dla reszty (uwzględnia zmiennoprzecinkowe)
            if (value < static_cast<Source>(T_lim::lowest())) {
                throw std::out_of_range("Underflow: value too small");
            }
        }

        // 2. Sprawdzenie górnej granicy
        // Jeśli cel jest mniejszy niż źródło, musimy uważać na przepełnienie
        if constexpr (sizeof(Source) >= sizeof(Target)) {
            if (value > static_cast<Source>(T_lim::max())) {
                throw std::out_of_range("Overflow: value too large");
            }
        }

        return static_cast<Target>(value);
    }
};


template <typename Target, typename Source>
Target safe_cast(Source value) {
    using T_lim = std::numeric_limits<Target>;

    // 1. Obsługa rzutowania: Liczba ujemna -> Typ bez znaku (Unsigned)
    if constexpr (std::is_signed_v<Source> && !std::is_signed_v<Target>) {
        if (value < 0) {
            throw std::out_of_range("safe_cast error: negative to unsigned cast");
        }
    }

    // 2. Sprawdzenie dolnej granicy (Underflow)
    // Rzutujemy limit do Source, aby porównanie było bezpieczne
    if constexpr (std::is_signed_v<Source> || std::is_signed_v<Target>) {
        if (value < static_cast<Source>(T_lim::lowest())) {
            throw std::out_of_range("safe_cast error: underflow, casted value too small");
        }
    }

    // 3. Sprawdzenie górnej granicy (Overflow)
    if (value > static_cast<Source>(T_lim::max())) {
        throw std::out_of_range("safe_cast error: overflow, casted value too large");
    }

    return static_cast<Target>(value);
}


