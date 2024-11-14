#include <main.hpp>

Parser::Parser(const std::string& path) {
    std::ifstream file(path);
    std::string line;

    if (file.is_open()) {
        size_t i = 0;
        while (std::getline(file, line)) {
            std::cout << i << " ";
            std::istringstream iss(line);
            std::string word;
            std::vector<std::string> words;

            if(i == 0) {
                iss >> word;
                std::cout << word;
                std::cout << std::endl;
            }
            else if(i > 1) {
                for (size_t j = 0; j <= 3; j++) {
                    iss >> word;
                    words.push_back(word);
                }

                for (const auto& w : words) {
                    std::cout << w << " ";
                }
                std::cout << std::endl;
            }

            i++;
        }
        file.close();
    }
    else {
        std::cerr << "Nepodarilo sa otvoriť súbor!" << std::endl;
    }
}


Data::Data(Parser& parser) : parser{parser} {

}

/*Data::~Data() {
}*/
