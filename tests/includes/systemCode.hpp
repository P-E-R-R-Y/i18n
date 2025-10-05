#include <string>

#if defined(__APPLE__)
    #include <CoreFoundation/CoreFoundation.h>
#elif defined(__unix__) || defined(__linux__)
    #include <locale>
#endif

std::string getSystemCode() {
    #if defined(__APPLE__)
        CFLocaleRef locale = CFLocaleCopyCurrent();
        if (!locale)
            return "en"; // fallback

        CFStringRef identifier = (CFStringRef)CFLocaleGetValue(locale, kCFLocaleIdentifier);

        char buffer[16] = {0};
        if (CFStringGetCString(identifier, buffer, sizeof(buffer), kCFStringEncodingUTF8)) {
            CFRelease(locale);
            return std::string(buffer, 2); // first 2 letters
        }
        CFRelease(locale);
    #elif defined(__unix__) || defined(__linux__)
        try {
            std::locale loc(""); // system locale
            std::string name = loc.name(); // e.g., "fr_FR.UTF-8"
            if (!name.empty() && name != "C" && name != "POSIX")
                return name.substr(0, 2);
        } catch (...) {}
    #endif
    
    return "en";
}