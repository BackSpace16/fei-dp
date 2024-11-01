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

    if (hex.size() == 7) {
        std::stringstream ss;
        ss << std::hex << hex.substr(1);
        ss >> red >> green >> blue;
    }
    else if (hex.size() == 9) {
        std::stringstream ss;
        ss << std::hex << hex.substr(1);
        ss >> alpha >> red >> green >> blue;
    }
    else if (hex.size() == 4) {
        red = std::stoi(hex.substr(1, 1), nullptr, 16) * 17;
        green = std::stoi(hex.substr(2, 1), nullptr, 16) * 17;
        blue = std::stoi(hex.substr(3, 1), nullptr, 16) * 17;
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
