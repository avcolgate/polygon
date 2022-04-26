#include "poly.h"

int main() {

    const std::string elementFileName =     "txt/dff_coords.txt";
    const std::string layoutFileName =       "txt/large_1.txt";

    Polygon   element;
    Polygon   layout;
    Topology  topology;
    
    if (element.readFile(elementFileName))
    {
        std::cout << std::setw(55) << elementFileName << " was successfully read!\n";
        element.process();
    }
    else {
        std::cout << "ERROR in opening file of element \"" << elementFileName << "\"\n";
        return EXIT_FAILURE;
    }

    if (layout.readFile(layoutFileName)) 
    {
        std::cout << std::setw(55) << layoutFileName << " was successfully read!\n";
        layout.process();
    }
    else {
        std::cout << "ERROR in opening file of layout \"" << layoutFileName << "\"\n";
        return EXIT_FAILURE;
    }

    std::cout << "Element: " << element.strOffsetType << std::endl;
    std::cout << "Layout:  " << layout.strOffsetType << std::endl << std::endl;

    topology.findPosOfElement(element.strOffsetType, layout.strOffsetType);
    
    topology.printPosOfElement();

    topology.checkOffset(element, layout);

    auto checkForward =  std::find(topology.correctPosition.begin(),        topology.correctPosition.end(), true);
    auto checkReverse =  std::find(topology.correctReversePosition.begin(), topology.correctReversePosition.end(), true);

    if (checkForward == topology.correctPosition.end() &&
        checkReverse == topology.correctReversePosition.end())
    {
        std::cout << "\nNOT FOUND!!\n";
    }
    else
    {
        std::cout << "\nFOUND!!\n";
    }

    return 0;
}