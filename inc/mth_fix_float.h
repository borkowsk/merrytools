/** @file
 *  @brief Klasa/y okrojonych float-ów
 *  @date 2026-05-12 (modification)
 *        ============================
 */
#ifndef MTH_FLOAT16_H
#define MTH_FLOAT16_H

#include <cstdint>
#include <cassert>

/// @brief Math types & calculations
namespace merry_tools::math {
    /** @defgroup FloatDoubleTools The Group of float/double related class and functions
    *  @{
    */

    /// Unsigned UFloat16.
    /// 2bytes positive float with resolution of 0.5[m], max(UFloat16)= ~0.5*65km min(UFloat16)=0km.
    /// Range is checked in constructor & operators only in DEBUG mode (by `assert()`).
    /// @note In RELEASE mode the difference with regular `float` is small, but in DEBUG mode it works 5 times slower.
    /// @todo Na wzór tego pomysłu stworzyć szablon `fix_flat<type TARGETFLOAT,type DISCRETE,int MULTIPLIER>`
    class UFloat16
    {
    public:
        UFloat16():discreetValue(UINT16_MAX) {}
        UFloat16(float value):discreetValue(static_cast<uint16_t>(_toInt*value)) { // NOLINT(*-explicit-constructor)
                                                                                     assert(value*_toInt<(UINT16_MAX-1));
        }

        UFloat16& operator = (float value) {                                         assert(value*_toInt<(UINT16_MAX-1));
            discreetValue=static_cast<uint16_t>(_toInt*value);
            return *this;
        }


        UFloat16& operator += (float value) {
            value+=static_cast<float>(*this);                                        assert(value*_toInt<(UINT16_MAX-1));
            discreetValue=static_cast<uint16_t>(_toInt*value);
            return *this;
        }
                                                                       // ReSharper disable once CppFunctionalStyleCast
        operator float() const { return _toFloat * float(discreetValue); } // NOLINT(*-explicit-constructor)

        bool isAssigned() const;
        static bool isFloatingPoint() { return true; }

    private:
        uint16_t discreetValue;
        static constexpr float _toInt=2.0f;
        static constexpr float _toFloat=1/_toInt;
        static constexpr int _maxVal=UINT16_MAX-1;

    };

    // METHOD IMPLEMENTATIONS:
    //*///////////////////////

    inline bool UFloat16::isAssigned() const
    {
        return discreetValue!=UINT16_MAX;
    }

    /** @} */
} // merry_tools namespace

#endif // MTH_FLOAT16_H
