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

//i18n
template <typename T>
struct is_tuple : std::false_type {};

template <typename... Args>
struct is_tuple<std::tuple<Args...>> : std::true_type {};

template <typename U>
concept IsTuple = is_tuple<U>::value;

// Global scope / namespace
template <typename Base, typename Derived>
concept LocaleImplementation = std::derived_from<Base, Derived>;

template<LocaleInterface T>
class I18n {
    
    public:

        // Singleton access
        static I18n<T>& getInstance() {
            static I18n<T> instance;
            
            return instance;
        }
        // 
        /**
         * @brief delete Copy constructor
         */
        I18n(const I18n&) = delete;

        /**
         * @brief delete Move constructor
         */
        I18n& operator=(const I18n&) = delete;

        /**
         * @brief Set a list of Supported Locales
         * 
         * @tparam T_Child
         */
        template <LocaleImplementation<T>... T_Child>
        void setSupportedLocales() {
            // Uses a pack expansion to call injectLocale<T_Locale>() for every type in the parameter pack.
            (this->setSupportedLocale<T_Child>(), ...); 
            if (!_locale)
                setDefault();
        }

        /**
         * @brief Set a tuple of Supported Locales
         * 
         * @tparam T_Tuple 
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
         * @brief Set a Default Locale
         * 
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
         * @brief Set the Locale
         * 
         * @param key 
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
         * @brief Get the Locale
         * 
         * @return T* 
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
         * @brief Construct a new I18n
         */
        I18n() {
            setSystemCode();
        }

        /**
         * @brief Set the System Code object
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
            #endif

            _systemCode = "en"; // fallback for other platforms
        }

        /**
         * @brief Set a Supported Locale
         * 
         * @tparam T_Child 
         */
        template <LocaleImplementation<T> T_Child>
        void setSupportedLocale() {
            auto newInstance = std::make_unique<T_Child>();
            std::string key = newInstance->languageCode();
            _supportedLocales[key] = std::move(newInstance);
        }

};
