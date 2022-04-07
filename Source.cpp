#include "poly.h"

int main() {
    const std::string targetFileName = "txt/dff_coords.txt";
    const std::string mainFileName = "txt/large_1.txt";

    Polygon targetPoly;
    Polygon mainPoly;
    
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


    //targetPoly.printLines();
    targetPoly.calcLines();

    targetPoly.MakeCoordsRelative();
    //targetPoly.printLines();

    targetPoly.FixOrderLines();
    //targetPoly.printLines();

    targetPoly.DeletingExtraLines();
    //targetPoly.printLines();

    targetPoly.findOffsets();
    //targetPoly.printOffsets();

    std::cout << "Small poly: " << targetPoly.strOffsetType << std::endl;

    std::cout << "##########################################################################################################\n";
    std::cout << "##########################################################################################################\n";
    /*
        ОБРАБОТКА БОЛЬШОГО ПОЛИГОНА
    */

    //mainPoly.printLines();
    mainPoly.calcLines();

    mainPoly.MakeCoordsRelative();
    //mainPoly.printLines();

    mainPoly.FixOrderLines();
    //mainPoly.printLines();

    mainPoly.DeletingExtraLines();
    //mainPoly.printLines();

    mainPoly.findOffsets();
    //mainPoly.printOffsets();

    std::cout << "Big poly: " << mainPoly.strOffsetType << std::endl;

    findWanted(targetPoly.strOffsetType, mainPoly.strOffsetType);

    return EXIT_SUCCESS;
}