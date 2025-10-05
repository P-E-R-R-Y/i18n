/**
 * @file SupportedLocales.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief 
 * @date 2025-10-05
 * 
 * @example Locale
 * @{
 */


#pragma once

    #include <tuple>

    #include "DefaultLocale.hpp"

    #include "LocaleEN.hpp"
    #include "LocaleES.hpp"
    #include "LocaleFR.hpp"
    #include "LocaleIT.hpp"

/**
 * @ingroup Example
 */
using SupportedLocales = std::tuple<LocaleEn, LocaleEs, LocaleFr, LocaleIt>;