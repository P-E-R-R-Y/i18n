/**
 * @file ILocale.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-03
 */

#pragma once

#include <string>
#include <type_traits>

class ILocale {
public:
    // --- Méthodes de base (string_view pour la performance) ---
    virtual const std::string languageCode() const = 0;
//    virtual const std::string scriptCode() const = 0; //todo later
    
    // --- Méthode pour les Pluriels / Variables ---
    // Elle prend des arguments et retourne un std::string (nécessaire car c'est une chaîne dynamique)

    virtual ~ILocale() = default;
};

//concept to use i18n
template <typename T>
concept IsLocaleImplementation = std::is_base_of_v<ILocale, T>;