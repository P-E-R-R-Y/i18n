#include "ILocale.hpp"
#include "I18n.hpp"

class DefaultLocale: public ILocale {
    public:
        virtual const std::string getSignUpTitle() const = 0;
        virtual const std::string getSignInTitle() const = 0;
        virtual const std::string getLoginSubTitle() const = 0;
        virtual const std::string getButtonSubmit() const = 0;
        virtual const std::string getButtonCancel() const = 0;
};

class EnLocale: public DefaultLocale {
    public:
        //ILocale
        const std::string languageCode() const override { return "en"; }
//        const std::string scriptCode() const override { return "en"; } //depends on ILocale
        
        //Default
        const std::string getSignUpTitle() const override { return "Sign Up";}
        const std::string getSignInTitle() const override { return "Sign In";}
        const std::string getLoginSubTitle() const override { return "welcome !";}
        const std::string getButtonSubmit() const override { return "Submit";}
        const std::string getButtonCancel() const override { return "Cancel";}
};

class FrLocale: public DefaultLocale {
    public:
        //ILocale
        const std::string languageCode() const override { return "fr"; }
//        const std::string scriptCode() const override { return "en"; } //depends on ILocale
        
        //Default
        const std::string getSignUpTitle() const override { return "Inscription";}
        const std::string getSignInTitle() const override { return "Connexion";}
        const std::string getLoginSubTitle() const override { return "Bienvenue !";}
        const std::string getButtonSubmit() const override { return "Valider";}
        const std::string getButtonCancel() const override { return "Annuler";}
};

//need to find a way to fast init i18n with a list of type

using supportedLocales = std::tuple<EnLocale, FrLocale>;

int main() {
    auto& i18n = I18n<DefaultLocale>::getInstance();
    i18n.initializeFromConfig<supportedLocales>();
    i18n.setLocale();
}