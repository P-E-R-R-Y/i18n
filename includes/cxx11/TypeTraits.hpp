/**
 * @file TypeTraits.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-06
 */

#pragma once
#include <type_traits> 
#include <tuple>
#include <cstddef>

/**
 * @brief Trait to check if a type `T` derives from a base `B`.
 *
 * Evaluates to true if `T` inherits from `B`, false otherwise.
 *
 * @tparam T Derived type to check
 * @tparam B Base type
 *
 * @see all_derived Variadic trait to check multiple derived types at once
 */
template<typename T, typename B>
struct is_derived_from: std::integral_constant<bool, std::is_base_of<B, T>::value> {};

/**
 * @brief Trait to check if all types in a pack are derived from a base type.
 * 
 * @tparam Base is a Parent type
 * @tparam Derived... List of Child to check 
 *
 * @see is_derived_from Single-type check trait
 * @see all_derived<Base> Multi-type check trait
 * @see all_derived<Base, First, Rest...> Multi-type check trait
 */
template<typename Base, typename... Derived>
struct all_derived;

/**
 * @brief If the pack of Derived types is empty, we trivially return true. This stops the recursion.
 *
 * @tparam Base The base type
 * 
 * @see is_derived_from Single-type check trait
 * @see all_derived Multi-type check trait
 * @see all_derived<Base, First, Rest...> Multi-type check trait
 */
template<typename Base> // (Base),  n = 0
struct all_derived<Base> : std::true_type {};

/**
 * @brief Uses `std::is_base_of` to check that `First` inherits from `Base`. Recursively evaluates the remaining `Rest...` types via all_derived.
 * 
 * @tparam Base The base type
 * @tparam First The first type in the Derived pack
 * @tparam Rest Remaining types in the Derived pack
 * 
 * @see is_derived_from Single-type check trait
 * @see all_derived Multi-type check trait
 * @see all_derived<Base> Multi-type check trait
 */
template<typename Base, typename First, typename... Rest> // (Base, {Item₀, …, Itemₙ}),  n ≥ 0
struct all_derived<Base, First, Rest...> :
    std::integral_constant<bool, //use as a type
        std::is_base_of<Base, First>::value // is_base_of
        && all_derived<Base, Rest...>::value> {}; // recursive

/**
 * @brief Detect whether a type is a `std::tuple` or not.
 *
 * Primary template: defaults to false.
 *
 * @tparam T any
 * 
 * @see is_tuple<std::tuple<Args...>> Partial specialization for actual tuples
 * @see IsTuple Concept wrapper for template constraints
 */
template <typename T>
struct is_tuple : std::false_type {};

/**
 * @brief Trait to detect whether a type is a `std::tuple`.
 *
 * Partial specialization for `std::tuple<Args...>`.
 * Sets `value = true` for any tuple type.
 *
 * @tparam ...Args are parameter from Tuple
 * 
 * @see is_tuple<T> Primary template
 * @see IsTuple Concept wrapper for template constraints
 */
template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};


/**
 * @brief Base Type to represent a sequence of compile time indice
 * 
 * @tparam list of int
 */
template <std::size_t... I>
struct index_sequence {};

/**
 * @brief recursive template to generate the sequence
 * 
 * @tparam N 
 * @tparam I 
 */
template <std::size_t N, std::size_t... I>
struct make_index_sequence_impl : make_index_sequence_impl<N - 1, N - 1, I...> {};

/**
 * @brief when N = 0, stop recursion and define the type alias
 * 
 * @tparam I 
 */
template <std::size_t... I>
struct make_index_sequence_impl<0, I...> {
    typedef index_sequence<I...> type;
};