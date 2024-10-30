/// @file
/// @brief ....
/// @date 2024-10-31 (modification)
/// Created by borkowsk on 30.10.24.

#include "mth_fix_float.h"
#include "mem_unique_val.h"
#include <iostream>

using namespace ::merry_tools::math;
using namespace ::merry_tools::memory;

// In global namespace:
//*////////////////////
UNIQUE_GLOBAL(::merry_tools::math::UFloat16,unsigned,THREE,3) ///< Global `unique_val` for `UFloat16` hierarchy and value 3.
UNIQUE_GLOBAL(::merry_tools::math::UFloat16,unsigned,FOUR,4)  ///< Global `unique_val` for `UFloat16` hierarchy and value 4;

/// Test for IN_CLASS_UNIQUE for class in global scope.
class dummy {
public:
    IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,FIVE,5)  ///< In class `unique_val` for `UFloat16` hierarchy and value 5.
    IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,TEN,10)  ///< In class `unique_val` for `UFloat16` hierarchy and value 10.
};

DEFINE_IN_CLASS_UNIQUE(dummy,::merry_tools::math::UFloat16,unsigned,FIVE,5) // Required definition completing declaration in class.
DEFINE_IN_CLASS_UNIQUE(dummy,::merry_tools::math::UFloat16,unsigned,TEN,10) // Required definition completing declaration in class.

/// Tests namespace.
namespace merry_tools::tests {

    IN_NAMESPACE_UNIQUE(::merry_tools::math::UFloat16,unsigned,THREE,3) ///< In namespace `unique_val` for `UFloat16` hierarchy and value 3.
    IN_NAMESPACE_UNIQUE(::merry_tools::math::UFloat16,unsigned,FOUR,4) ///< In namespace `unique_val` for `UFloat16` hierarchy and value 4.

    /// Test for IN_CLASS_UNIQUE for class in `namespace`.
    /// Also declared in `main()` unit of tests, but with different IN_CLASS_UNIQUE 👅
    class dummy2 {
    public:
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,FIVE,5) ///< namespace::class `unique_val` for `UFloat16` hierarchy and value 5.
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,TEN,10) ///< namespace::class `unique_val` for `UFloat16` hierarchy and value 10.
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,ELEV,11) ///< namespace::class `unique_val` for `UFloat16` hierarchy and value 11.
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,TWELF,12) ///< namespace::class `unique_val` for `UFloat16` hierarchy and value 12.
    };

    DEFINE_IN_CLASS_UNIQUE(dummy2,::merry_tools::math::UFloat16,unsigned,FIVE,5) ///< Required definition completing declaration in class.
    DEFINE_IN_CLASS_UNIQUE(dummy2,::merry_tools::math::UFloat16,unsigned,TEN,10) ///< Required definition completing declaration in class.

    class dummy3: public dummy2
    {
        int five=dummy2::FIVE;
    public:
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,TEN,10) ///< this already exist in dummy2 class, so could be declared.
        IN_CLASS_UNIQUE(::merry_tools::math::UFloat16,unsigned,TWENTY,20) ///< namespace::class `unique_val` for `UFloat16` hierarchy and value 20.
    };

    /// @details Such a `TEN` cannot be used because cannot be defined for class `dummy3`.
    /// @code e.x.:
    /// ```C++
    ///   unsigned ten3=dummy3::TEN;
    /// ```
    unsigned ten3=dummy2::TEN;   ///< But this `TEN`, of course, could be used because is already defined in base class `dummy2`.
    unsigned twenty=dummy3::TWENTY; ///< This `TWENTY` could be used because it is new unique value declared in class `dummy3`.

    /// @details En expanding of the macro leads to an error, because TEN is already declared & defined for class `dummy2`
    /// @code e.x.:
    /// ```C++
    ///  DEFINE_IN_CLASS_UNIQUE(dummy3,::merry_tools::math::UFloat16,unsigned,TEN,10) // Impossible definition completing declaration in class for TEN.
    /// ```
    DEFINE_IN_CLASS_UNIQUE(dummy3,::merry_tools::math::UFloat16,unsigned,TWENTY,20) ///< Required definition completing declaration in class.

}


namespace merry_tools::tests {

    UFloat16 fl1 = 123.5;

    const int four=FOUR;                     ///< Example of using unique values as initializers.
    const int three=THREE;                   ///< Example of using unique values as initializers.

    const int five=dummy::FIVE;              ///< Example of using unique values as initializers.
    const int five2=tests::dummy2::FIVE;     ///< Example of using unique values as initializers.
    const int ten=dummy::TEN;                ///< Example of using unique values as initializers.
    const int ten2=tests::dummy2::TEN;       ///< Example of using unique values as initializers.
    const int twenty2=tests::dummy3::TWENTY; ///< Example of using unique values as initializers.



    void print_namespace_variables(std::ostream& o)
    {
        o<<"five:"<<five<<std::endl;
        o<<"five2:"<<five2<<std::endl;
        o<<"ten:"<<ten<<std::endl;
        o<<"ten2:"<<ten2<<std::endl;
    }

}

