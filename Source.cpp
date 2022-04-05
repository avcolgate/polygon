#include "poly.h"

int main() {
    const std::string wantedFileName = "txt/dff_coords.txt";

    Polygon wanted_poly;
    
    if (wanted_poly.readFile(wantedFileName) == true) {
        std::cout << "File " << wantedFileName << " was successfully read!\n\n";
    }
    else {
        std::cout << "ERROR in opening file!";
        return EXIT_FAILURE;
    }

    //wanted_poly.printLines();
    wanted_poly.calcLines();

    wanted_poly.MakeCoordsRelative();
    //wanted_poly.printLines();

    wanted_poly.FixOrderLines();
    //wanted_poly.printLines();

    wanted_poly.DeletingExtraLines();
    //wanted_poly.printLines();

    wanted_poly.findOffsets();
    //wanted_poly.printOffsets();

    for (size_t i = 0; i < wanted_poly.offsetType.size(); i++) {
        wanted_poly.strOffsetType = wanted_poly.strOffsetType + wanted_poly.offsetType.at(i);
    }
    std::cout << wanted_poly.strOffsetType << std::endl;

    //for (size_t i = 0; i < wanted_poly.offsetType.size(); i++) {
    //    std::cout << wanted_poly.offsetHeight.at(i) << " ";
    //}

    std::cout << "-------------------------------------------------------------------------------------------\n";
    /*
        ОБРАБОТКА БОЛЬШОГО ПОЛИГОНА
    */

    const std::string bigFileName = "txt/large_1.txt";

    Polygon big_poly;

    if (big_poly.readFile(bigFileName) == true) {
        std::cout << "File " << bigFileName << " was successfully read!\n\n";
    }
    else {
        std::cout << "ERROR in opening " << bigFileName << std::endl;
        return EXIT_FAILURE;
    }

    //big_poly.printLines();
    big_poly.calcLines();

    big_poly.MakeCoordsRelative();
    //big_poly.printLines();

    big_poly.FixOrderLines();
    //big_poly.printLines();

    big_poly.DeletingExtraLines();
    //big_poly.printLines();

    big_poly.findOffsets();
    //big_poly.printOffsets();

    for (size_t i = 0; i < big_poly.offsetType.size(); i++) {
        big_poly.strOffsetType = big_poly.strOffsetType + big_poly.offsetType.at(i);
    }
    std::cout << big_poly.strOffsetType << std::endl;

    //for (size_t i = 0; i < big_poly.offsetType.size(); i++) {
    //    std::string tempstr = (big_poly.offsetType.at(i));
    //    std::cout << tempstr.substr(0, 1);
    //}
    //std::cout << std::endl;



    return EXIT_SUCCESS;
}