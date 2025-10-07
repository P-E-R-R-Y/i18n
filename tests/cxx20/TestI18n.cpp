/**
 * @file TestI18n.cpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-05
 * 
 * @example TestI18n.cpp
 * @{
 */

#include "gtest/gtest.h"
#include "I18n.hpp" 
#include "SupportedLocales.hpp"
#include "SystemCode.hpp"

#if defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(__unix__) || defined(__linux__)
    #include <locale>
#endif

// Test 1: Take systemLocale (ci use FR) + (variadic_locales)
TEST(I18nTest, DefaultLocaleSystem_1) {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    i18n.setSupportedLocales<LocaleFr, LocaleEn>();

    DefaultLocale* current = i18n.getLocale();
    ASSERT_NE(current, nullptr) << "Current locale must be set after injection.";

    EXPECT_EQ(current->languageCode(), getSystemCode()) << "Default locale should be the first injected ('en').";
}

// Test 2: If no systemLocale take 'en' + (variadic_locales)
TEST(I18nTest, DefautLocaleEn_2) {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    i18n.setSupportedLocales<LocaleEs, LocaleEn>();

    DefaultLocale* current = i18n.getLocale();
    ASSERT_NE(current, nullptr) << "Current locale must be set after injection.";

    EXPECT_EQ(current->languageCode(), "en") << "Default locale should be the first injected ('en').";
}

// Test 3: If no systemLocale & No 'en' take first found (variadic_locales)
TEST(I18nTest, DefaultLocaleFirst_3) {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    // Inject the locales using the compile-time tuple method
    i18n.setSupportedLocales<LocaleEs>();

    // Check if the current locale pointer is set
    DefaultLocale* current = i18n.getLocale();
    ASSERT_NE(current, nullptr) << "Current locale must be set after injection.";

    EXPECT_EQ(current->getButtonCancel(), "Cancelar");
}

// Test 4: Set with tuples Locale to 'fr' and usage. + (tuple)
TEST(I18nTest, SetupLocaleFr_4) {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    // Inject the locales using the compile-time tuple method
    i18n.setSupportedLocales<SupportedLocales>();

    // Check if the current locale pointer is set
    EXPECT_EQ(i18n.setLocale("fr"), true);
    DefaultLocale* current = i18n.getLocale();
    ASSERT_NE(current, nullptr) << "Current locale must be set after injection.";

    EXPECT_EQ(current->languageCode(), "fr");

    EXPECT_EQ(current->getButtonCancel(), "Annuler");
    EXPECT_EQ(current->getButtonSubmit(), "Valider");
    EXPECT_EQ(current->getLoginSubTitle(), "Bienvenue !");
    EXPECT_EQ(current->getSignInTitle(), "Connexion");
    EXPECT_EQ(current->getSignUpTitle(), "Inscription");
}