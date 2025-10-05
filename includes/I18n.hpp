/**
 * @file I18n.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-06
 */

#pragma once

#include <string>
#include <map>
#include <concepts>

#if defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(__unix__) || defined(__linux__)
    #include <locale>
#endif

#include "ILocale.hpp"

/**
 * @brief Trait to detect whether a type is a `std::tuple`.
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
 * @brief Using is_tuple specialisation this concept verify if it's a tuple
 * 
 * @tparam T is the type to check
 * 
 * @see is_tuple<T> Primary template
 * @see is_tuple<std::tuple<Args...>> Partial specialization
 */
template <typename T>
concept IsTuple = is_tuple<T>::value;

/**
 * @brief DerivedFrom is accept supportedLocal class only
 * 
 * @tparam Base is the parent
 * @tparam Derived is the child
 */
template <typename Base, typename Derived>
concept DerivedFrom = std::derived_from<Base, Derived>;

/**
 * @brief Internationalization manager for a specific locale type.
 *
 * Handles the registration of supported locales, selection of the current locale,
 * and retrieval of localized data at runtime. Implements a singleton pattern
 * so there is only one instance per template type.
 *
 * @tparam T The base locale interface type that all supported locales must derive from.
 */
template<LocaleInterface T>
class I18n {

    public:

        /**
         * @brief Get the I18n singleton instance.
         *
         * @return I18n<T>& Reference to the single instance.
         */
        static I18n<T>& getInstance() {
            static I18n<T> instance;
            
            return instance;
        }

        /**
         * @brief delete Copy constructor
         */
        I18n(const I18n&) = delete;

        /**
         * @brief delete Move constructor
         */
        I18n& operator=(const I18n&) = delete;

        /**
         * @brief Register a list of supported locales using template parameter pack.
         * 
         * Each type must derive from `T` and be default-constructible.
         * Sets the default locale if no locale was previously selected.
         * @see DerivedFrom
         * 
         * @tparam T_Child Variadic list of locale types to register.
         * 
         * @see setSupportedLocales(T_Tuple)
         * @see setSupportedLocale<T_Child>()
         */
        template <DerivedFrom<T>... T_Child>
        void setSupportedLocales() {
            // Uses a pack expansion to call injectLocale<T_Locale>() for every type in the parameter pack.
            (this->setSupportedLocale<T_Child>(), ...); 
            if (!_locale)
                setDefault();
        }

        /**
         * @brief Register supported locales using a std::tuple of types.
         * 
         * Each type must derive from `T` and be default-constructible.
         * Sets the default locale if no locale was previously selected.
         * 
         * @tparam T_Child Variadic list of locale types to register.
         * 
         * @see setSupportedLocales(T_Tuple)
         * @see setSupportedLocale<T_Child>()
         */
        template <IsTuple T_Tuple>
        void setSupportedLocales() {
            // 1. Decompose the type T_Tuple into a pack of types.
            std::apply([this](auto... locals) {
                (this->setSupportedLocale<std::decay_t<decltype(locals)>>(), ...);
            }, T_Tuple{}); // T_Tuple{} creates a temporary instance just to enable std::apply
            if (!_locale)
                setDefault();
        }

        /**
         * @brief Sets the default locale to use if no other locale is selected.
         *
         * Priority:
         * 1. System locale (if available)
         * 2. English ("en") fallback
         * 3. First locale accessible.
         */
        void setDefault() {
            if (_supportedLocales.empty())
                return;
            
            if (!_systemCode.empty() && setLocale(_systemCode))
                return;
            if (setLocale("en"))
                return;

            _locale = _supportedLocales.begin()->second.get();
        }

        /**
         * @brief Select a specific locale by code.
         *
         * @param code Two-letter language code (e.g., "en", "fr").
         * @return true if the locale was found and selected; false otherwise.
         */
        bool setLocale(const std::string& code) {
            auto it = _supportedLocales.find(code);

            if (it != _supportedLocales.end()) {
                _locale = it->second.get();
                return true;
            }
            return false;
        }
        
        /**
         * @brief Get the currently selected locale instance.
         *
         * @return T* Pointer to the current locale. nullptr if none selected.
         */
        T* getLocale() const {
            return _locale;
        }

    private:
        std::string _systemCode;
        T* _locale = nullptr;
        std::unordered_map<std::string, std::unique_ptr<T>> _supportedLocales;

    private:
        /**
         * @brief Private constructor initializes the system code.
         */
        I18n() {
            setSystemCode();
        }

        /**
         * @brief Detect and store the system default locale code.
         *
         * Uses platform-specific APIs:
         * - macOS: CoreFoundation CFLocale
         * - Linux/Unix: std::locale
         * - Other: defaults to "en"
         */
        void setSystemCode() {
             #if defined(__APPLE__)
                CFLocaleRef locale = CFLocaleCopyCurrent();
                if (!locale) _systemCode = "en"; // fallback

                CFStringRef identifier = (CFStringRef)CFLocaleGetValue(locale, kCFLocaleIdentifier);

                char buffer[16] = {0};
                if (CFStringGetCString(identifier, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
                    CFRelease(locale);
                    _systemCode = std::string(buffer, 2); // first 2 letters
                }
                CFRelease(locale);
            #elif defined(__unix__) || defined(__linux__)
                try {
                    std::locale loc(""); // system locale
                    std::string name = loc.name(); // e.g., "fr_FR.UTF-8"
                    if (!name.empty() && name != "C" && name != "POSIX")
                        _systemCode = name.substr(0, 2);
                } catch (...) {}
            #else
                _systemCode = "en"; // fallback for other platforms
            #endif
        }

        /**
         * @brief Register a single locale type.
         *
         * @tparam T_Child Locale type derived from `T`. Must be default-constructible.
         *
         * @see setSupportedLocales(T_Child...)
         * @see setSupportedLocales(T_Tuple)
         */
        template <DerivedFrom<T> T_Child>
        void setSupportedLocale() {
            auto newInstance = std::make_unique<T_Child>();
            std::string key = newInstance->languageCode();
            _supportedLocales[key] = std::move(newInstance);
        }

};
