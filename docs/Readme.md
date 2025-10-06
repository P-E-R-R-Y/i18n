# 🌍 P-E-R-R-Y i18n  
*A modern C++20 internationalization (i18n) library.*
*A legacy C++11 internationalization (i18n) library.*

[![Build](https://github.com/P-E-R-R-Y/i18n/actions/workflows/cmake.yml/badge.svg)](https://github.com/P-E-R-R-Y/i18n/actions)
[![Docs](https://img.shields.io/badge/docs-doxygen-blue.svg)](https:/P-E-R-R-Y.github.io/i18n/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

---

## ✨ Overview

`P-E-R-R-Y i18n` is a lightweight, **header-only** C++20 library for internationalization.  
It provides a clean, type-safe way to define and switch between language locales at runtime.

Core goals:
- 🧩 **Strongly typed** locales using `concepts` and `type traits`
- ⚙️ **System locale detection** (macOS & Linux)
- 🔄 **Runtime locale switching**
- 🚀 **Zero dependencies**, **header-only**, **C++20 concepts**
- ✅ **Unit-tested** with GoogleTest

---

## 🧱 Features

- Detects the system locale (`fr`, `en`, `es`, …)
- Fallback chain (`system → en → first registered`)
- Singleton pattern for shared locale management
- Compile-time locale registration with `setSupportedLocales`
- Works with tuples or parameter packs

---

## 🧩 Example

```cpp
#include "I18n.hpp"
#include "ILocale.hpp"
#include <tuple>


// Define your base locale interface
class DefaultLocale : public ILocale {
public:
    virtual const std::string getSignUpTitle() const = 0;
    virtual const std::string getSignInTitle() const = 0;
};

// Define concrete implementations
class LocaleEn : public DefaultLocale {
public:
    const std::string languageCode() const override { return "en"; }
    const std::string getSignUpTitle() const override { return "Sign Up"; }
    const std::string getSignInTitle() const override { return "Sign In"; }
};

class LocaleFr : public DefaultLocale {
public:
    const std::string languageCode() const override { return "fr"; }
    const std::string getSignUpTitle() const override { return "Inscription"; }
    const std::string getSignInTitle() const override { return "Connexion"; }
};

using SupportedLocales = std::tuple<LocaleEn, LocaleFr>

// Usage
int main() {
    auto& i18n = I18n<DefaultLocale>::getInstance();

    i18n.setSupportedLocales<SupportedLocales>();
    //cpp `i18n.setSupportedLocales<LocaleEn, LocaleFr>();`

    //set Locale over the default one -> 1st: system, 2nd: "en", 3rd: 1st_one
    i18n.setLocale("en");

    std::cout << i18n.getLocale()->getSignInTitle() << std::endl;
}
```