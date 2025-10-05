/**
 * @file LocaleEN.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-05
 */

#pragma once

#include "DefaultLocale.hpp"

class LocaleEn: public DefaultLocale {
    public:
        const std::string languageCode() const override { return "en"; }
        const std::string getSignUpTitle() const override { return "Sign Up";}
        const std::string getSignInTitle() const override { return "Sign In";}
        const std::string getButtonSubmit() const override { return "Submit";}
        const std::string getLoginSubTitle() const override { return "welcome !";}
        const std::string getButtonCancel() const override { return "Cancel";}
};
