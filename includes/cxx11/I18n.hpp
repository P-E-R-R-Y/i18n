/**
 * @file I18n.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-06
 */

#pragma once

#include <string>
#include <memory>
#include <unordered_map>
#include <tuple>
#include <utility>
#include <type_traits>

#include "ILocale.hpp"
#include "TypeTraits.hpp"

#if defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(__unix__) || defined(__linux__)
    #include <locale>
#endif


/**
 * @brief Internationalization manager for a specific locale type.
 *
 * Handles the registration of supported locales, selection of the current locale,
 * and retrieval of localized data at runtime. Implements a singleton pattern
 * so there is only one instance per template type.
 *
 * @tparam T is the base locale interface derived from ILocale, all supported locales must derive from
 */
template<typename T, typename = typename std::enable_if<is_derived_from<T, ILocale>::value>::type>
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
         * * Each type must derive from `T` and be default-constructible.
         * Sets the default locale if no locale was previously selected.
         * @see DerivedFrom
         * * @tparam T_Child Variadic list of locale types to register.
         * * @see setSupportedLocales(T_Tuple)
         * @see setSupportedLocale<T_Child>()
         */       
        template<typename... T_Child>
        typename std::enable_if<all_derived<T, T_Child...>::value, void>::type
        setSupportedLocales() {
            // C++11 pack expansion via initializer list trick
            auto l = { (setSupportedLocale<T_Child>(), 0)... }; 
            (void)l; //silence !
            if (!_locale) setDefault();
        }


        /**
         * @brief Register supported locales using a std::tuple of types.
         * * Each type must derive from `T` and be default-constructible.
         * Sets the default locale if no locale was previously selected.
         * * @tparam T_Tuple Variadic list of locale types to register.
         * * @see setSupportedLocales(T_Tuple)
         * @see setSupportedLocale<T_Child>()
         */
        template<typename T_Tuple>
        typename std::enable_if<is_tuple<T_Tuple>::value, void>::type
        setSupportedLocales() {
            registerTupleLocales_using_index<T_Tuple>(
                typename make_index_sequence_impl<std::tuple_size<T_Tuple>::value>::type{}
            );
            if (!_locale) setDefault();
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
            typename std::unordered_map<std::string, std::unique_ptr<T>>::iterator it = _supportedLocales.find(code);

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
                // Use explicit casts and checks for C++11 compatibility
                CFLocaleRef locale = CFLocaleCopyCurrent();
                if (!locale) {
                    _systemCode = "en"; // fallback
                    return;
                }

                CFStringRef identifier = (CFStringRef)CFLocaleGetValue(locale, kCFLocaleIdentifier);

                char buffer[16] = {0};
                if (CFStringGetCString(identifier, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
                    _systemCode = std::string(buffer, 2); // first 2 letters
                } else {
                    _systemCode = "en"; // second fallback
                }
                CFRelease(locale);
            #elif defined(__unix__) || defined(__linux__)
                try {
                    // C++11 locale handling
                    std::locale loc(""); // system locale
                    std::string name = loc.name(); // e.g., "fr_FR.UTF-8"
                    if (!name.empty() && name != "C" && name != "POSIX")
                        _systemCode = name.substr(0, 2);
                } catch (...) {
                    _systemCode = "en"; // fallback on error
                }
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
        template <typename T_Child, typename = typename std::enable_if<is_derived_from<T_Child, T>::value>::type>
        void setSupportedLocale() {
            // C++11 replacement for std::make_unique (C++14)
            std::unique_ptr<T_Child> newInstance(new T_Child());
            std::string key = newInstance->languageCode();
            _supportedLocales[key] = std::move(newInstance);
        }

        template<typename Tuple, std::size_t... Is>
        void registerTupleLocales_using_index(index_sequence<Is...>) {
            // C++11 Pack Expansion via initializer list trick
            auto l = { (setSupportedLocale<typename std::tuple_element<Is, Tuple>::type>(), 0)... };
            (void)l;
        }

};