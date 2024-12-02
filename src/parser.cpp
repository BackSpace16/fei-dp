#include <main.hpp>
#include <elements.hpp>

Atom::Atom(const std::string& element, const std::array<double, 3>& pos) : position(pos), color("#000") {
    if (elementData.contains(element)) {
        color = Color{elementData.at(element).color};
        scale = elementData.at(element).radius; //0.21 * 
    } 
    else {
        throw std::runtime_error("Unknown element: " + element);
    }
}


glm::vec3 Atom::getPositionVec3() const {
    return glm::vec3(position[0], position[1], position[2]);
}

glm::vec3 Atom::getColor() const {
    return color.toVec3();
}

glm::vec3 Atom::getScale() const {
    return glm::vec3(scale);
}

Parser::Parser(const std::filesystem::path path) : filePath(path) {}

std::vector<Atom> ParserXYZ::parseFile() {
    std::vector<Atom> atoms;
    std::ifstream file(std::filesystem::weakly_canonical(filePath));
    std::string line;

    if (file.is_open()) {
        size_t i = 0;
        while (std::getline(file, line)) {
            //std::cout << i << " ";
            std::istringstream iss(line);
            std::string word;
            std::vector<std::string> words;

            if(i == 0) {
                iss >> word;
                std::cout << word;
                std::cout << std::endl;
            }
            else if(i > 1) {
                iss >> word;
                std::string element = word;
                std::array<double, 3> position;

                if (word.find_first_of("0123456789") == 0) {
                    break;
                }

                for (int j = 0; j < 3; ++j) {
                    iss >> word;
                    position[j] = std::stod(word);
                }

                try {
                    atoms.emplace_back(element, position);
                } 
                catch (const std::runtime_error& e) {
                    std::cerr << "Chyba pri vytváraní atómu: " << e.what() << std::endl;
                }
            }
            i++;
        }
        file.close();
    }
    else {
        std::cerr << "Nepodarilo sa otvoriť súbor!" << std::endl;
        std::cerr << std::filesystem::weakly_canonical(filePath) << std::endl;
    }
    return atoms;
}

Data::Data(Settings& settings) : settings{settings} {
    createParser();
    parse();
}

void Data::createParser() {
    parser = std::make_unique<ParserXYZ>(settings.filePath);
    // F8BT
    // ethylthiol_on_Au221_pulling
    // dithioCAB-Au24_hist
    // rutile_Au_Oad_CO
    // 0.0-7.0
}


void Data::parse() {
    atoms = std::move(parser->parseFile());
}

/*Data::~Data() {
}*/
