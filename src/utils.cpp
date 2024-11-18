#include <main.hpp>

Color::Color(float red, float green, float blue, float alpha)
    : r(red), g(green), b(blue), a(alpha) {}

Color::Color(int red, int green, int blue, int alpha) {
    r = red / 255.0f;
    g = green / 255.0f;
    b = blue / 255.0f;
    a = alpha / 255.0f;
}

Color::Color(const std::string& hex) {
    int red, green, blue, alpha = 255;

    // Skontroluj správny formát hex (7 alebo 9 znakov)
    if (hex.size() == 7) {
        std::stringstream ss;
        ss << std::hex << hex.substr(1, 2);  // Čítaj dva znaky pre červenú
        ss >> red;
        ss.clear();
        ss << std::hex << hex.substr(3, 2);  // Čítaj dva znaky pre zelenú
        ss >> green;
        ss.clear();
        ss << std::hex << hex.substr(5, 2);  // Čítaj dva znaky pre modrú
        ss >> blue;
    }
    else if (hex.size() == 9) {  // Ak je formát s alfa kanálom (napr. #RRGGBBAA)
        std::stringstream ss;
        ss << std::hex << hex.substr(1, 2);  // Alfa
        ss >> alpha;
        ss.clear();
        ss << std::hex << hex.substr(3, 2);  // Červená
        ss >> red;
        ss.clear();
        ss << std::hex << hex.substr(5, 2);  // Zelená
        ss >> green;
        ss.clear();
        ss << std::hex << hex.substr(7, 2);  // Modrá
        ss >> blue;
    }
    else if (hex.size() == 4) {  // Skvelé pre skrátený hex formát #RGB
        red = std::stoi(hex.substr(1, 1), nullptr, 16) * 17;   // Pre #R
        green = std::stoi(hex.substr(2, 1), nullptr, 16) * 17; // Pre #G
        blue = std::stoi(hex.substr(3, 1), nullptr, 16) * 17;  // Pre #B
    }
    else {
        throw std::invalid_argument("Invalid hex color format");
    }

    r = red / 255.0f;
    g = green / 255.0f;
    b = blue / 255.0f;
    a = alpha / 255.0f;
}

glm::vec3 Color::toVec3() const {
    return glm::vec3(r, g, b);
}

glm::vec4 Color::toVec4() const {
    return glm::vec4(r, g, b, a);
}

void Color::printRGB() const {
    int red = static_cast<int>(r * 255);
    int green = static_cast<int>(g * 255);
    int blue = static_cast<int>(b * 255);
    std::cout << "Color(RGB): " << red << ", " << green << ", " << blue << "\n";
}

void Color::printHex() const {
    int red = static_cast<int>(r * 255);
    int green = static_cast<int>(g * 255);
    int blue = static_cast<int>(b * 255);
    std::cout << "Color(Hex): #" 
              << std::hex << std::setfill('0') << std::setw(2) << red 
              << std::setw(2) << green 
              << std::setw(2) << blue << std::dec << "\n";
}
