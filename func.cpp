#include "poly.h"

void findWanted(const std::string target, const std::string main) {
    size_t i = 0;

    for (i = main.find(target, i++); i != std::string::npos; i = main.find(target, i + 1))
        std::cout << i << " ";

    std::cout << std::endl;
}