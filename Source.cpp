#include "poly.h"

int main() {
    const std::string fileName = "dff_coords.txt";

    Polygon polygon;
    
    if (polygon.readFile(fileName) == true) {
        std::cout << "File " << fileName << " was successfully read!\n\n";
    }
    else {
        std::cout << "ERROR in opening file!";
        return EXIT_FAILURE;
    }

    polygon.printLines();
    polygon.calcLines();

    polygon.MakeCoordsRelative();
    polygon.printLines();

    polygon.FixOrderLines();
    polygon.printLines();

    polygon.DeletingExtraLines();
    polygon.printLines();

    polygon.findOffsets();
    polygon.printOffsets();

    for (size_t i = 0; i < polygon.offsetType.size(); i++) {
        std::string tempstr = (polygon.offsetType.at(i));
        std::cout << tempstr.substr(0,1);
    }
    std::cout << std::endl;

    for (size_t i = 0; i < polygon.offsetType.size(); i++) {
        std::cout << polygon.offsetHeight.at(i) << " ";
    }

    return EXIT_SUCCESS;
}