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
    mainPoly.process();

    std::cout << "Small poly: " << targetPoly.strOffsetType << std::endl;
    std::cout << "Main poly: " << mainPoly.strOffsetType << std::endl;

    layout.findPosOfTarget(targetPoly.strOffsetType, mainPoly.strOffsetType);
    
    layout.printPosOfTarget();

    layout.checkOffset(targetPoly, mainPoly);

    auto checkForward =  std::find(layout.truePosition.begin(),        layout.truePosition.end(), true);
    auto checkReverse =  std::find(layout.trueReversePosition.begin(), layout.trueReversePosition.end(), true);

    if (checkForward == layout.truePosition.end() &&
        checkReverse == layout.trueReversePosition.end())
    {
        std::cout << "\nNOT FOUND!!\n";
    }
    else
    {
        std::cout << "\nFOUND!!\n";
    }

   /* if (*std::find(layout.truePosition.begin(),        layout.truePosition.end(),        true) != 0 ||
        *std::find(layout.trueReversePosition.begin(), layout.trueReversePosition.end(), true) != 0)
    {
        std::cout << std::setw(60) << "YES\n";
    }
    else {
        std::cout << std::setw(60) << "NO\n";
    }*/

    return 0;
}