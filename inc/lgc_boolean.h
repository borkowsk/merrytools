/** @file
 *  @brief Klasa bezpiecznych wartości logicznych.
 *  @date 2025-04-04 (modification)
 *  @details Created by borkowsk on 04.04.25. */

#ifndef MERRY_TOOLS_TESTS_LGC_BOOLEAN_H
#define MERRY_TOOLS_TESTS_LGC_BOOLEAN_H

#include <cassert>

namespace merry_tools {
    namespace logic {
        /** Safe lgc_boolean.
         * It can be undefined if we don't assign it a value, and unlike the `bool` type, it can't be assigned anything
         * other than `bool` or `lgc_boolean`. */
        class lgc_boolean {
        public:
            using carrier_t = unsigned char;
            static constexpr carrier_t  min=0;
            static constexpr carrier_t half=128; //!< "half and half" means unpredictable.
            static constexpr carrier_t  max=255;

        protected:
            carrier_t value; //!< Zakodowana wartość stanu.
        public:
            lgc_boolean(): value(half){}
            lgc_boolean(const lgc_boolean&)=default;
            lgc_boolean(int)=delete;
            lgc_boolean(double)=delete;
            lgc_boolean(void*)=delete;
            lgc_boolean(bool v): value(!v ? min : max){}
            ~lgc_boolean();

            lgc_boolean& operator = (const lgc_boolean&)=default;
            lgc_boolean& operator = (int)=delete;
            lgc_boolean& operator = (double)=delete;
            lgc_boolean& operator = (void*)=delete;
            lgc_boolean& operator = (bool v){ value=(!v ? min : max); return *this;}

            bool isAssigned() { return value!=half; }
            bool      check() { assert(isAssigned()); return value>half; }

            explicit operator bool () { return check(); }
        };

    } // logic
} // merry-tools

#endif //MERRY_TOOLS_TESTS_LGC_BOOLEAN_H
