/** @file
 *  @brief Klasy gwarantujące unikalność wartości.
 *  @date 2024-10-31 (modification)
 * Created by borkowsk on 30.10.24. */
#ifndef MERRY_TOOLS_TESTS_MEM_UNIQUE_VAL_H
#define MERRY_TOOLS_TESTS_MEM_UNIQUE_VAL_H

namespace merry_tools::memory {
    template<class HIERARCHY_T, class CARRIER_T, CARRIER_T ini_value> class unique_val;
}

//IT IS NOT REQUIRED.
//template<class HIERARCHY_T, class CARRIER_T, CARRIER_T ini_value>
//CARRIER_T get(const merry_tools::memory::unique_val<HIERARCHY_T,CARRIER_T,ini_value>& self);

namespace merry_tools::memory {

    /// This guarantees that for a given hierarchy and value type, no duplicate values can be declared in the same namespace.
    /// However, it is necessary to use it with macros:
    /// `UNIQUE_GLOBAL`, `IN_NAMESPACE_UNIQUE`, `IN_CLASS_UNIQUE` & `DEFINE_IN_CLASS_UNIQUE`.
    /// @code E.x.:
    /// ```C++
    ///     UNIQUE_GLOBAL(unsigned,unsigned,uONE,1)
    ///     UNIQUE_GLOBAL(UFloat16,unsigned,ONE,1)
    ///     UNIQUE_GLOBAL(UFloat16,unsigned,TWO,2)
    /// ```
    /// \tparam HIERARCHY_T - this type is used only for distinguishing.
    /// \tparam CARRIER_T - type of values.
    /// \tparam ini_value - value which must be unique.
    template<class HIERARCHY_T, class CARRIER_T, CARRIER_T ini_value>
    class unique_val {
        static constexpr const CARRIER_T curr_val = ini_value;
    public:
        typedef CARRIER_T RetType;

        ~unique_val()=default;
        operator const CARRIER_T &() { return curr_val; }
    };

} // merry_tools::memory namespace

/// This macro work only in main global namespace!
#define UNIQUE_GLOBAL(_HIERARCHY_T,_CARRIER_T,_name,_ini_value)                           \
        _CARRIER_T get(                                                                   \
                const unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value>& self ) {            \
                return _ini_value; }                                                      \
        ::merry_tools::memory::unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value> _name;      \

/// This macro work in namespaces.
#define IN_NAMESPACE_UNIQUE(_HIERARCHY_T,_CARRIER_T,_name,_ini_value)                     \
        _CARRIER_T get(const unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value>& self){       \
                return _ini_value; }                                                      \
        ::merry_tools::memory::unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value> _name;

/// This macro work in class XXX {} , but needs DEFINE_IN_CLASS_UNIQUE.
#define IN_CLASS_UNIQUE(_HIERARCHY_T,_CARRIER_T,_name,_ini_value)                         \
        friend                                                                            \
        _CARRIER_T  get(const unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value>& self);      \
        static ::merry_tools::memory::unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value> _name;

/// This macro work for class XXX {} , but outside class.
#define DEFINE_IN_CLASS_UNIQUE(_CLASS,_HIERARCHY_T,_CARRIER_T,_name,_ini_value)           \
        _CARRIER_T get(const unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value>& self ) {     \
                return _ini_value; }                                                      \
        ::merry_tools::memory::unique_val<_HIERARCHY_T,_CARRIER_T,_ini_value> _CLASS::_name;


#endif //MERRY_TOOLS_TESTS_MEM_UNIQUE_VAL_H
