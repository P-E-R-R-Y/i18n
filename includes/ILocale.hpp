/**
 * @file ILocale.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-03
 */

#pragma once

#include <string>
#include <concepts>

/**
 * @brief Base interface for all locale implementations.
 *
 * Defines the minimal contract that a locale type must provide.
 * Each locale should inherit from this class and implement `languageCode()`.
 *
 * Example usage:
 * @code
 * // see LocaleInterface.
 * class DefaultLocale : public ILocale {
 * public:
 *     virtual const std::string homePageTitle() const = 0;
 * };
 * 
 * // in I18n<T> see DerivedFrom<T> where T is DefaultLocale.
 * class EnLocale : public DefaultLocale {
 * public:
 *     const std::string languageCode() const override { return "en"; }
 *     const std::string homePageTitle() const override { return "Welcome"; }
 * };
 * @endcode
 */
class ILocale {
public:

    /**
     * @brief Retrieve the two-letter language code of the locale.
     *
     * @note example: "en" -> English, "fr" -> French, "es" -> Spanish.
     *
     * @return std::string Two-letter ISO language code.
     */
    virtual const std::string languageCode() const = 0;

    /**
     * @brief Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~ILocale() = default;
};

/**
 * @brief Concept to constrain template parameters to valid locale interface and implementation.
 *
 * Ensures that a type derives from `ILocale` and can be used in the I18n system.
 *
 * @tparam T The type to check.
 *
 * @see ILocale
 * @see I18n
 */
template <typename T>
concept LocaleInterface = std::derived_from<T, ILocale>;