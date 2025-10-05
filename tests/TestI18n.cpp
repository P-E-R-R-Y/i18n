#include "gtest/gtest.h"
#include "I18n.hpp" 
#include "ILocale.hpp"

// --- Setup Aliases and Types ---

// Define the base interface for the test (must match the template argument for I18n)
class DefaultLocale: public ILocale {
    public:
        virtual const std::string getSignUpTitle() const = 0;
        virtual const std::string getSignInTitle() const = 0;
        virtual const std::string getLoginSubTitle() const = 0;
        virtual const std::string getButtonSubmit() const = 0;
        virtual const std::string getButtonCancel() const = 0;
        virtual const std::string languageCode() const = 0;
        virtual const std::string scriptCode() const { return ""; }
};

// Define concrete locale implementations

class EnLocale: public DefaultLocale {
public:
    const std::string languageCode() const override { return "en"; }
    const std::string getSignUpTitle() const override { return "Sign Up";}
    const std::string getSignInTitle() const override { return "Sign In";}
    const std::string getButtonSubmit() const override { return "Submit";}
    const std::string getLoginSubTitle() const override { return "welcome !";}
    const std::string getButtonCancel() const override { return "Cancel";}
};

class FrLocale: public DefaultLocale {
public:
    const std::string languageCode() const override { return "fr"; }
    const std::string getSignUpTitle() const override { return "Inscription";}
    const std::string getSignInTitle() const override { return "Connexion";}
    const std::string getButtonSubmit() const override { return "Valider";}
    const std::string getLoginSubTitle() const override { return "Bienvenue !";}
    const std::string getButtonCancel() const override { return "Annuler";}
};

class EsLocale: public DefaultLocale {
public:
    const std::string languageCode() const override { return "es"; }
    const std::string getSignUpTitle() const override { return "Registro"; }
    const std::string getSignInTitle() const override { return "Iniciar sesión"; }
    const std::string getButtonSubmit() const override { return "Enviar"; }
    const std::string getLoginSubTitle() const override { return "¡Bienvenido!"; }
    const std::string getButtonCancel() const override { return "Cancelar"; }
};

// Alias for the specific I18n instance used in tests
using SupportedLocales = std::tuple<EnLocale, FrLocale>;

// --- Tests ---

// Test 1: Injection and Default Locale Setup (Must run first to initialize the Singleton)
TEST(I18nTest, SystemLocaleSetup) {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    // Inject the locales using the compile-time tuple method
    i18n.setSupportedLocales<FrLocale, EnLocale>();

    DefaultLocale* current = i18n.getLocale();
    ASSERT_NE(current, nullptr) << "Current locale must be set after injection.";

    EXPECT_EQ(current->languageCode(), "en") << "Default locale should be the first injected ('en').";

    EXPECT_EQ(current->getButtonCancel(), "Cancel");
}

TEST(I18nTest, FirstLocaleSetup) {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    // Inject the locales using the compile-time tuple method
    i18n.setSupportedLocales<FrLocale, EsLocale>();

    // Check if the current locale pointer is set
    DefaultLocale* current = i18n.getLocale();
    ASSERT_NE(current, nullptr) << "Current locale must be set after injection.";

//    EXPECT_EQ(current->languageCode(), "en") << "Default locale should be the first injected ('en').";

}

TEST(I18nTest, FrLocaleSetup) {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    // Inject the locales using the compile-time tuple method
    i18n.setSupportedLocales<FrLocale, EsLocale, EnLocale>();

    // Check if the current locale pointer is set
    bool b = i18n.setLocale("fr");
    EXPECT_EQ(b, true);
    DefaultLocale* current = i18n.getLocale();
    ASSERT_NE(current, nullptr) << "Current locale must be set after injection.";

//    EXPECT_EQ(current->languageCode(), "en") << "Default locale should be the first injected ('en').";

    EXPECT_EQ(current->getButtonCancel(), "Annuler");
}