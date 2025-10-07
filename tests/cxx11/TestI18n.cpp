/**
 * @file TestI18n.cpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief Simple C++11 unit test file using assert() to verify I18n functionality.
 * @date 2025-10-06
 */

#include <iostream>
#include <string>
#include <cassert> // Assertion C++11 standard
#include <cstdlib> // Pour EXIT_FAILURE/EXIT_SUCCESS

// En-têtes de la librairie à tester
#include "I18n.hpp" 
#include "SupportedLocales.hpp"
#include "SystemCode.hpp"

// --- Utilitaire de Test ---

// Fonction d'aide pour exécuter un test et s'assurer qu'il s'arrête en cas d'échec
void runTest(const std::string& name, void (*testFunc)()) {
    std::cout << "Test: " << name << " -> ";
    try {
        testFunc();
        std::cout << "SUCCES" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "ÉCHEC (Exception): " << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    } catch (...) {
        std::cerr << "ÉCHEC (Assertion ou erreur inconnue)" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

// --- Cas de Test ---

// Test 1: Take systemLocale (ci use FR) + (variadic_locales)
void test_DefaultLocaleSystem() {
    auto& i18n = I18n<DefaultLocale>::getInstance();
    i18n.setSupportedLocales<LocaleFr, LocaleEn>(); 

    DefaultLocale* current = i18n.getLocale();
    
    assert(current != nullptr && "T1: La locale actuelle doit être définie après l'injection.");
    assert((current->languageCode() == getSystemCode() || current->languageCode() == "en") && "T1: La locale par défaut doit correspondre au code système ou à 'en'.");

    (void)current;
}

// Test 2: If no systemLocale take 'en' + (variadic_locales)
void test_DefautLocaleEn() {
    auto& i18n = I18n<DefaultLocale>::getInstance();
    
    i18n.setSupportedLocales<LocaleEs, LocaleEn>(); 

    assert(i18n.getLocale()->languageCode() == "en");
    assert(i18n.getLocale()->getButtonCancel() == "Cancel" && "T2: Chaîne localisée 'Cancel' échouée.");
}

// Test 3: If no systemLocale & No 'en' take first found (variadic_locales)
void test_DefaultLocaleFirst() {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    i18n.setSupportedLocales<LocaleEs>(); 

    assert(i18n.getLocale() != nullptr && "T3: La locale actuelle doit être définie.");
    assert(i18n.getLocale()->languageCode() == "es" && "T3: Doit être par défaut à 'es'.");
    assert(i18n.getLocale()->getButtonCancel() == "Cancelar" && "T3: Chaîne localisée 'Cancelar' échouée.");
}

// Test 4: Set with tuples Locale to 'fr' and usage. + (tuple)
void test_SetupLocaleFr() {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    // Injecte plusieurs locales
    i18n.setSupportedLocales<SupportedLocales>();

    assert(i18n.setLocale("fr") == true && "T4: setLocale('fr') a échoué.");
    assert(i18n.getLocale()->languageCode() == "fr" && "T4: Le code de langue doit être 'fr'.");

    assert(i18n.getLocale()->getButtonCancel() == "Annuler" && "T4: Annuler échoué.");
    assert(i18n.getLocale()->getButtonSubmit() == "Valider" && "T4: Valider échoué.");
    assert(i18n.getLocale()->getLoginSubTitle() == "Bienvenue !" && "T4: Bienvenue ! échoué.");
    assert(i18n.getLocale()->getSignInTitle() == "Connexion" && "T4: Connexion échoué.");
    assert(i18n.getLocale()->getSignUpTitle() == "Inscription" && "T4: Inscription échoué.");
}

// --- Main ---
int main() {
    std::cout << "--- Démarrage des tests I18n C++11 ---" << std::endl;
    
    runTest("1. Default Locale System/Fallback", test_DefaultLocaleSystem);
    runTest("2. Explicit Locale 'en' Check", test_DefautLocaleEn);
    runTest("3. Single Locale Default Check", test_DefaultLocaleFirst);
    runTest("4. Specific Locale 'fr' Data Check", test_SetupLocaleFr);

    std::cout << "--- Tous les tests ont réussi ! ---" << std::endl;
    return EXIT_SUCCESS;
}
