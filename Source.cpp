#include "poly.h"

int main() {

    const std::string elementFileName =     "txt/dff_coords.txt";
    const std::string layoutFileName =       "txt/large_1.txt";

    Polygon   element;
    Polygon   layout;
    Topology  topology;
    
    if (element.readFile(elementFileName)) {
        std::cout << std::setw(55) << elementFileName << " was successfully read!\n";
    }
    else {
        std::cout << "ERROR in opening " << elementFileName << std::endl;
        return EXIT_FAILURE;
    }

    if (layout.readFile(layoutFileName)) {
        std::cout << std::setw(55) << layoutFileName << " was successfully read!\n";
    }
    else {
        std::cout << "ERROR in opening " << layoutFileName << std::endl;
        return EXIT_FAILURE;
    }

    element.process();
    layout.process();

    std::cout << "Element: " << element.strOffsetType << std::endl;
    std::cout << "Layout:  " << layout.strOffsetType << std::endl;

    topology.findPosOfElement(element.strOffsetType, layout.strOffsetType);
    
    topology.printPosOfElement();

    topology.checkOffset(element, layout);

    auto checkForward =  std::find(topology.truePosition.begin(),        topology.truePosition.end(), true);
    auto checkReverse =  std::find(topology.trueReversePosition.begin(), topology.trueReversePosition.end(), true);

    if (checkForward == topology.truePosition.end() &&
        checkReverse == topology.trueReversePosition.end())
    {
        std::cout << "\nNOT FOUND!!\n";
    }
    else
    {
        std::cout << "\nFOUND!!\n";
    }

   /* if (*std::find(topology.truePosition.begin(),        topology.truePosition.end(),        true) != 0 ||
        *std::find(topology.trueReversePosition.begin(), topology.trueReversePosition.end(), true) != 0)
    {
        std::cout << std::setw(60) << "YES\n";
    }
    else {
        std::cout << std::setw(60) << "NO\n";
    }*/

    return 0;
}