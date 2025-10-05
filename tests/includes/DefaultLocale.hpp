/**
 * @file DefaultLocale.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-05
 */

#pragma once

#include "ILocale.hpp"

class DefaultLocale: public ILocale {
    public:
        virtual const std::string getSignUpTitle() const = 0;
        virtual const std::string getSignInTitle() const = 0;
        virtual const std::string getLoginSubTitle() const = 0;
        virtual const std::string getButtonSubmit() const = 0;
        virtual const std::string getButtonCancel() const = 0;
        virtual const std::string languageCode() const = 0;
};
