#pragma once

#include <iostream>
#include <locale>
#include <string>
#include <map>
#include "ILocale.hpp"

template<IsLocaleImplementation T>
class I18n {
    public:
        // Singleton access
        static I18n& getInstance() {
            static I18n instance;
            return instance;
        }

        // Delete copy/move constructors
        I18n(const I18n&) = delete;
        I18n& operator=(const I18n&) = delete;

        template <class T_Child>
        void injectLocale() {
            static_assert(std::is_base_of_v<T, T_Child>,
                        "All types in the supported_locales tuple must inherit from T define here I18n<T> .");
            
            auto newInstance = std::make_unique<T_Child>();
            std::string key = newInstance->languageCode();
            supportedLocales[key] = std::move(newInstance);
        }

        template <class... T_Childrens>
        void injectLocales() {
            // Uses a pack expansion to call injectLocale<T_Locale>() for every type in the parameter pack.
            (this->injectLocale<T_Childrens>(), ...); 
        }

        template <class T_Tuple>
        void injectTupleOfLocales() {
            static_assert(is_tuple<T_Tuple>::value, 
                        "The type passed to initializeFromConfig must be a std::tuple of locales.");

            // 1. Decompose the type T_Tuple into a pack of types.
            std::apply([](auto... locals) {
                (this->injectLocale<std::decay_t<decltype(locals)>>(), ...);
            }, T_Tuple{}); // T_Tuple{} creates a temporary instance just to enable std::apply
        }

        void ensureDefaultLocaleSet() {
            // Check if a locale is active AND if the supported map is not empty.
            if (this->currentLocale_ == nullptr && !this->supportedLocales_.empty()) {
                // Set the first available locale in the map as the default.
                this->setCurrentLocale(this->supportedLocales_.begin()->first);
            }
        }

    private:
        T& locale; // default
        std::unordered_map<std::string, T> supportedLocales;

        I18n() {
            std::locale loc("");
            std::string locName = loc.name(); // e.g., "fr_FR.UTF-8"
            auto pos = locName.find_first_of("_.");
            if (pos != std::string::npos)
                systemLang = locName.substr(0, 2);
            else if (!locName.empty())
                systemLang = locName.substr(0, 2);
        }
};
