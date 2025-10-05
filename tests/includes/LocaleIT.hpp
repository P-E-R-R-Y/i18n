/**
 * @file ItalianLocale.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-05
 */

#pragma once

#include "DefaultLocale.hpp"

class LocaleIt: public DefaultLocale {
    public:
        const std::string languageCode() const override { return "it"; }
        const std::string getSignUpTitle() const override { return "Registrati"; }
        const std::string getSignInTitle() const override { return "Accedi"; }
        const std::string getButtonSubmit() const override { return "Invia"; }
        const std::string getLoginSubTitle() const override { return "Benvenuto!"; }
        const std::string getButtonCancel() const override { return "Annulla"; }
};