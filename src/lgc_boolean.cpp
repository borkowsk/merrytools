/** @file
 *  @brief Klasa bezpiecznych wartości logicznych.
 *  @date 2026-05-12 (modification)
 *  @details Created by borkowsk on 04.04.25. */

#include "lgc_boolean.h"

namespace merry_tools {
    namespace logic {

        lgc_boolean::~lgc_boolean() {
            value=half; //Again not assigned... But not for sure, because deletion from heap may change this value.
        }
    } // logic
} // merry-tools
