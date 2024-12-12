#include <main.hpp>
#include <elements.hpp>

Atom::Atom(const std::string& element, const std::array<double, 3>& pos) : position(pos), color("#000") {
    if (elementData.contains(element)) {
        color = Color{elementData.at(element).color};
        scale = 0.21 * elementData.at(element).radius; // 
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
        std::cout <<  "Parsing XYZ file" << std::endl;
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
        std::cerr << "Cannot open file " << filePath << std::endl;
        std::cerr << std::filesystem::weakly_canonical(filePath) << std::endl;
    }
    return atoms;
}


std::vector<Atom> ParserCAR::parseFile() {
    std::vector<Atom> atoms;
    std::ifstream file(std::filesystem::weakly_canonical(filePath));
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Cannot open file " << filePath << std::endl;
        return atoms;
    }

    std::cout << "Parsing CAR file" << std::endl;

    std::getline(file, line);

    // Scale a bunka
    std::getline(file, line);
    double scale = std::stod(line);
    std::array<std::array<double, 3>, 3> lattice;

    for (int i = 0; i < 3; ++i) {
        std::getline(file, line);
        std::istringstream iss(line);
        for (int j = 0; j < 3; ++j) {
            iss >> lattice[i][j];
            lattice[i][j] *= scale;
        }
    }

    // Znacky a pocty
    std::getline(file, line);
    std::istringstream elementsStream(line);
    std::vector<std::string> elements;
    std::string element;
    while (elementsStream >> element) {
        elements.push_back(element);
    }

    std::getline(file, line);
    std::istringstream countsStream(line);
    std::vector<int> counts;
    int count;
    while (countsStream >> count) {
        counts.push_back(count);
    }

    if (elements.size() != counts.size()) {
        throw std::runtime_error("Mismatch between element types and counts");
    }

    // Direct
    std::getline(file, line);
    if (line.empty() || (line[0] != 'D' && line[0] != 'd')) {
        throw std::runtime_error("Expected 'Direct' keyword");
    }

    // Suradnice
    size_t elementIndex = 0;
    size_t atomIndex = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::array<double, 3> fractionalPosition, cartesianPosition;

        for (int i = 0; i < 3; ++i) {
            iss >> fractionalPosition[i];
        }

        // TODO if direct
        for (int i = 0; i < 3; ++i) {
            cartesianPosition[i] = 0.0;
            for (int j = 0; j < 3; ++j) {
                cartesianPosition[i] += fractionalPosition[j] * lattice[j][i];
            }
        }

        while (atomIndex >= counts[elementIndex]) {
            atomIndex -= counts[elementIndex];
            ++elementIndex;
        }

        try {
            atoms.emplace_back(elements[elementIndex], cartesianPosition);
        } catch (const std::runtime_error& e) {
            std::cerr << "Chyba pri vytváraní atómu: " << e.what() << std::endl;
        }

        ++atomIndex;
    }

    file.close();
    return atoms;
}

Data::Data(Settings& settings) : settings{settings} {
    createParser();
    parse();
}

void Data::createParser() {
    std::string fileType = settings.filePath.string().substr(settings.filePath.string().size() - 3);
    std::transform(fileType.begin(), fileType.end(), fileType.begin(), ::tolower);
    std::cout <<  fileType << std::endl;
    if (fileType == "xyz")
        parser = std::make_unique<ParserXYZ>(settings.filePath);
    else if (fileType == "car")
        parser = std::make_unique<ParserCAR>(settings.filePath);
    else
        std::cerr << "Unknown file type " << fileType << std::endl;

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
