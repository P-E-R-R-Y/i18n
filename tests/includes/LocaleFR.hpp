/**
 * @file LocaleFR.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-05
 * 
 * @example LocaleFR.hpp
 * @{
 */

#pragma once

#include "DefaultLocale.hpp"

/**
 * @ingroup Example
 */
class LocaleFr: public DefaultLocale {
    public:
        const std::string languageCode() const override { return "fr"; }

        const std::string getButtonCancel() const override { return "Annuler";}
        const std::string getButtonSubmit() const override { return "Valider";}
        const std::string getLoginSubTitle() const override { return "Bienvenue !";}
        const std::string getSignInTitle() const override { return "Connexion";}
        const std::string getSignUpTitle() const override { return "Inscription";}
};