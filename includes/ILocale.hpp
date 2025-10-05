/**
 * @file ILocale.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-03
 */

#pragma once

#include <string>
#include <concepts>

//locale
class ILocale {
public:
    virtual const std::string languageCode() const = 0;

    virtual ~ILocale() = default;
};

template <typename T>
concept LocaleInterface = std::is_base_of_v<ILocale, T>;