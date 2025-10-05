/**
 * @file LocaleES.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-05
 */

#pragma once

#include "DefaultLocale.hpp"

class LocaleEs: public DefaultLocale {
    public:
        const std::string languageCode() const override { return "es"; }
        const std::string getSignUpTitle() const override { return "Registro"; }
        const std::string getSignInTitle() const override { return "Iniciar sesión"; }
        const std::string getButtonSubmit() const override { return "Enviar"; }
        const std::string getLoginSubTitle() const override { return "¡Bienvenido!"; }
        const std::string getButtonCancel() const override { return "Cancelar"; }
};
