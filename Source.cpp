#include "poly.h"

int main() {

    const std::string targetFileName =     "txt/dff_coords.txt";
    const std::string mainFileName =       "txt/large_1.txt";

    Polygon targetPoly;
    Polygon mainPoly;
    Layout  layout;
    
    if (targetPoly.readFile(targetFileName)) {
        std::cout << std::setw(55) << targetFileName << " was successfully read!\n";
    }
    else {
        std::cout << "ERROR in opening " << targetFileName << std::endl;
        return EXIT_FAILURE;
    }

    if (mainPoly.readFile(mainFileName)) {
        std::cout << std::setw(55) << mainFileName << " was successfully read!\n";
    }
    else {
        std::cout << "ERROR in opening " << mainFileName << std::endl;
        return EXIT_FAILURE;
    }

    targetPoly.process();

    std::cout << "Small poly: " << targetPoly.strOffsetType << std::endl;


    mainPoly.process();

    std::cout << "Main poly: " << mainPoly.strOffsetType << std::endl;

    layout.findPosOfTarget(targetPoly.strOffsetType, mainPoly.strOffsetType);
    
    layout.printPosOfTarget();

    layout.checkOffset(layout.posOfTarget,

                       targetPoly.strOffsetType,
                       targetPoly.offsetHeight,
                       targetPoly.offsetWidth,
                       
                       mainPoly.strOffsetType,
                       mainPoly.offsetHeight,
                       mainPoly.offsetWidth);

    if (*std::find(layout.truePosition.begin(), layout.truePosition.end(), true) != 0) {
        std::cout << std::setw(60) << "YES\n";
    }
    else {
        std::cout << std::setw(60) << "NO\n";
    }

    return EXIT_SUCCESS;
}