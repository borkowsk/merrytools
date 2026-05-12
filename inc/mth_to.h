/** @file
 *  @brief Szablon funkcji wykonujących wysycające rzutowanie liczb.
 *  @date 2026-05-12 (modification)
 *        ==========================================================
 *  @prototyped by Gemini
*/
#ifndef MTH_TO_HPP
#define MTH_TO_HPP

#include <limits>
#include <type_traits>
#include <iostream>
#include <algorithm>

#ifdef NDEBUG
#define MTH_LOG_SATURATION(val, target) ((void)0)
#else
#define MTH_LOG_SATURATION(val, target) \
std::clog << "[Saturate] Value " << (val) << " out of range. Saturated to " << (target) << std::endl
#endif

namespace merry_tools::math {

/// Szablon wysycającego rzutu wartości liczbowych.
/// @note Ukrywa utraty dokładności więc do użycia zwłaszcza w grafice.
/// @details
///  Prykład użycia:
///  ```C++
///   uint32_t b=8888;
///   auto a = to<int32_t>(b);
///  ```
///  Zasady działania
///  ----------------
///  Dla ograniczenia narzutu w czasie wykonania zaimplementowane z
///  maksymalnym użyciem `constexpr` wg. następujących zasad:
///     - Jeśli liczba cyfr znaczących SOURCE jest mniejsza niż TARGET to po
///       prostu zdajemy się na konwersję rozszerzerzającą.
///     - w przeciwnym wypadku sprawdzamy czy wartość `sour` nie wykracza poza
///       zakres. Jeśli nie to wykonujemy zwykły `static_cast`.
///       -- Jeśli wartość `sour` jest za duża to zwracamy maksymalną wartość
///          typu docelowego.
///       -- Jeśli wartość `sour`jest za mała to zwracamy najmniejszą możliwą
///          wartość typu docelowego.
///   Dla liczb zmiennoprzecinkowych nie przejmujemy się ewentualną
///   utratą dokładności i ukrywamy warningi na ten temat.
///   W trybie DEBUG sytuacje wysycenia wartości są logowane do clog.
template<class TARGET,class SOURCE>
TARGET to(const SOURCE& sour)
{
    // Statyczne upewnienie się, że operujemy na typach numerycznych
    static_assert(std::is_arithmetic_v<SOURCE>, "SOURCE must be a numeric type.");
    static_assert(std::is_arithmetic_v<TARGET>, "TARGET must be a numeric type.");

    // Pobieramy limity typu docelowego
    constexpr TARGET t_max = std::numeric_limits<TARGET>::max();
    constexpr TARGET t_min = std::numeric_limits<TARGET>::lowest(); // lowest() dla float daje -max()

    // 1. Sprawdzamy czy SOURCE zawsze mieści się w TARGET (konwersja rozszerzająca)
    // Porównujemy digits (liczba bitów wartości) oraz zakresy min/max
    if constexpr (std::numeric_limits<SOURCE>::max() <= std::numeric_limits<TARGET>::max() &&
                  std::numeric_limits<SOURCE>::lowest() >= std::numeric_limits<TARGET>::lowest()) {
        return static_cast<TARGET>(sour);
    }
    else {
        // 2. Wartość może wykraczać poza zakres - wymagana logika wysycania

        // Specjalna obsługa NaN dla typów zmiennoprzecinkowych
        if constexpr (std::numeric_limits<SOURCE>::has_quiet_NaN) {
            if (sour != sour) { // Szybki test na NaN
                return static_cast<TARGET>(0);
            }
        }

        // Sprawdzenie górnej granicy
        // Rzutujemy t_max na SOURCE tylko jeśli jest to bezpieczne,
        // inaczej porównujemy bezpośrednio (static_cast może ukryć ostrzeżenia)
        if (sour > static_cast<SOURCE>(t_max)) {
            MTH_LOG_SATURATION(sour, "MAX");
            return t_max;
        }

        // Sprawdzenie dolnej granicy
        if (sour < static_cast<SOURCE>(t_min)) {
            MTH_LOG_SATURATION(sour, "MIN");
            return t_min;
        }

        // Wartość mieści się w zakresie - bezpieczny rzut
        return static_cast<TARGET>(sour);
    }
}

} //namespace end.

#undef MTH_LOG_SATURATION

#endif // MTH_TO_HPP
