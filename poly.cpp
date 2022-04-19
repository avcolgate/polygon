#include "poly.h"

std::string eLineOrientToStr(const LineOrientation& orient) {
    if (orient == LineOrientation::vertical)
        return "vertical";
    else if (orient == LineOrientation::horizontal)
        return "horizontal";
    return "undefined";
}

std::string eOffsetTypeToStr(const OffsetType& type) {
    if (type == OffsetType::in)
        return "i";
    else if (type == OffsetType::out)
        return "o";
    else if (type == OffsetType::edge)
        return "e";

    return "u";
}

bool Polygon::readFile(const std::string& fileName) {

    std::ifstream file;
    file.open(fileName);
    Line tempLine;
    uint32_t tempLen;

    if (!file)
        return false;
  
    file >> tempLine.start.x >> tempLine.start.y;

    do {
        if (file.eof())
            return false;
        file >> tempLine.end.x >> tempLine.end.y;

        //calculating length and orientation
        tempLen = abs(tempLine.end.x - tempLine.start.x);
        if (tempLen == 0) {
            tempLen = abs(tempLine.end.y - tempLine.start.y);
            tempLine.orient = LineOrientation::vertical;
        }
        else {
            tempLine.orient = LineOrientation::horizontal;
        }
        tempLine.length = tempLen;
        line.push_back(tempLine);

        tempLine.start.x = tempLine.end.x;
        tempLine.start.y = tempLine.end.y;

    } while (!(tempLine.end.x == line.at(0).start.x && tempLine.end.y == line.at(0).start.y));
    
    file.close();
    return true;
}

void Polygon::printLines() {
    using namespace std;
   
    string tempOrient;
    cout << "Lines table:\n";
    cout << setw(18) << "start X" << setw(15) << "start Y"
        << setw(15) << "end X" << setw(15) << "end Y"
        << setw(16) << "length" << setw(16) << "orientation\n";
    for (size_t i = 0; i < line.size(); i++) {
        tempOrient = eLineOrientToStr(line[i].orient);

        cout << "(" << i << ")" << setw(15) << line[i].start.x << setw(15) << line[i].start.y
            << setw(15) << line[i].end.x << setw(15) << line[i].end.y
            << setw(16) << line[i].length << setw(15) << tempOrient << "\n";
    }
    cout << "\n";
}

void Polygon::printOffsets() {
    using namespace std;
    string tempType;
    cout << "Offsets table:\n";
    cout << setw(13) << "Width" << setw(8) << "Height" << setw(8) << "Type\n";
    for (size_t i = 0; i < strOffsetType.size(); i++) {
        cout << "(" << i << ")" << setw(9) << offsetWidth[i] << setw(8) << offsetHeight[i]
            << setw(7) << strOffsetType[i] << "\n";
    }
    cout << "\n";
}

void Polygon::calcLines() {
    size_t widthNum = 0, nextNum = 0, prevNum = 0;

    //calculating width
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i].length > width && line[i].orient == LineOrientation::horizontal) {
            width = line[i].length;
            widthNum = i;
        }   
    }
            
    if (widthNum == line.size() - 1) {
        prevNum = widthNum - 1;
        nextNum = 0;
    }
    else if (widthNum == 0) {
        prevNum = line.size() - 1;
        nextNum = widthNum + 1;
    }
    else {
        prevNum = widthNum - 1;
        nextNum = widthNum + 1;
    }

    //calculating height
    if (line[nextNum].length > line[prevNum].length)
        height = line[nextNum].length;
    else
        height = line[prevNum].length;
    
    /*
    calculating min & max
    */

    //if width is on the top
    if (line[nextNum].end.y < line[widthNum].end.y) {
        widthOnTop = true;
        //max
        if (line[widthNum].end.x > line[widthNum].start.x) {
            max.x = line[widthNum].end.x;
            max.y = line[widthNum].end.y;
        }
        else {
            max.x = line[widthNum].start.x;
            max.y = line[widthNum].start.y;
        }
        //min
        if (line[nextNum].end.x < line[prevNum].start.x) {
            min.x = line[nextNum].end.x;
            min.y = line[nextNum].end.y;
        }
        else {
            min.x = line[prevNum].start.x;
            min.y = line[prevNum].start.y;
        }
    }
    //if width is on the bottom
    else {
        widthOnTop = false;
        //max
        if (line[nextNum].end.x > line[prevNum].start.x) {
            max.x = line[nextNum].end.x;
            max.y = line[nextNum].end.y;
        }
        else {
            max.x = line[prevNum].start.x;
            max.y = line[prevNum].start.y;
        }
        //min
        if (line[widthNum].end.x < line[widthNum].start.x) {
            min.x = line[widthNum].end.x;
            min.y = line[widthNum].end.y;
        }
        else {
            min.x = line[widthNum].start.x;
            min.y = line[widthNum].start.y;
        }
    }

#ifdef DEBUG
    std::cout << "Width is line number " << widthNum << std::endl;
    std::cout << "Width's length " << line[widthNum].length << std::endl;

    if (widthOnTop)
        std::cout << std::setw(70) << "Width is on the top!" << std::endl;
    else
        std::cout << std::setw(70) << "Width is on the bottom!" << std::endl;
#endif // DEBUG
}

void Polygon::makeCoordsRelative() {
    for (size_t i = 0; i < line.size(); i++) {
        line[i].start.x = line[i].start.x - min.x;
        line[i].start.y = line[i].start.y - min.y;

        line[i].end.x = line[i].end.x - min.x;
        line[i].end.y = line[i].end.y - min.y;
    }

#ifdef DEBUG
    std::cout << std::setw(70) << "Coordinates changed to relative!" << std::endl;
#endif // DEBUG
}

void Polygon::fixOrderLines() {
    //Placing coords in right order (0;0) is on the start
    size_t i = 0;
    bool is_clockwise = false;
    while (! (line[i].start.x == 0 && line[i].start.y == 0) ) {
        line.push_back(line[i]);
        i++;
    }
    line.erase(line.begin(), line.begin() + i);

    //не нужно, ведь каждая точка есть, как в старте, так и в энде
    //size_t i = 0;
    //while (!((line[i].start.x == 0 && line[i].start.y == 0) || (line[i].end.x == 0 && line[i].end.y == 0))) {
    //    line.push_back(line[i]);
    //    i++;
    //}
    //line.erase(line.begin(), line.begin() + i);
    //std::cout << std::setw(70) << "Order of coordinates was fixed!" << std::endl;

    if (line[0].end.x == 0) 
    {
        is_clockwise = true;
    }
    else 
    {
        is_clockwise = false;
        for (int32_t i = 0; i < line.size(); i++)
        {
            std::swap(line[i].end, line[i].start);
        }
        std::reverse(line.begin(), line.end());
    }
#ifdef DEBUG
    std::cout << std::setw(70) << "Order of coordinates was fixed!" << std::endl;
    if (is_clockwise) 
    {
        std::cout << "Clockwise! No reverse needed.\n";
    }
    else
    {
        std::cout << "Anticlockwise! Lines was reversed!\n" << "start <> end\n";
    }
#endif // DEBUG
}

void Polygon::deleteExtraLines() {
    int32_t idx = 0;
    //Deleting width
    for (int32_t i = 0; i < line.size(); i++) {
        if (line[i].length == width && line[i].orient == LineOrientation::horizontal) {
            idx = i;
            break;
        }
    }
    line.erase(line.begin() + idx);
    idx--;

    //Deleting the previous
    if (idx == -1) {
        idx = static_cast <int32_t> (line.size() - 1);
    }
    line.erase(line.begin() + idx);

    //Deleting the next
    if (idx == line.size()) {
        idx = 0;
    }
    line.erase(line.begin() + idx);

#ifdef DEBUG
    std::cout << std::setw(70) << "Sides was deleted!" << std::endl;
#endif // DEBUG
}

void Polygon::findOffsets() {
    std::string    strTempOffsetType;
    int32_t        tempOffsetHeight;
    int32_t        tempOffsetWidth;

    for (size_t i = 0; i < line.size(); i++) {
        if (line[i].orient == LineOrientation::horizontal) {
            tempOffsetWidth = line[i].length;
            
            //top
            if (true == widthOnTop)   
            {   
                tempOffsetHeight = (line[i].start.y);

                if (tempOffsetHeight == 0)
                    strTempOffsetType = eOffsetTypeToStr(OffsetType::edge);
                else if (tempOffsetHeight < 0)
                    strTempOffsetType = eOffsetTypeToStr(OffsetType::out);
                else if (tempOffsetHeight > 0)
                    strTempOffsetType = eOffsetTypeToStr(OffsetType::in);
            }

            //bottom
            else                      
            {                                         
                tempOffsetHeight = (line[i].start.y - (max.y - min.y));

                if (tempOffsetHeight == 0)
                    strTempOffsetType = eOffsetTypeToStr(OffsetType::edge);
                else if (tempOffsetHeight > 0)
                    strTempOffsetType = eOffsetTypeToStr(OffsetType::out);
                else if (tempOffsetHeight < 0)
                    strTempOffsetType = eOffsetTypeToStr(OffsetType::in);
            }
            offsetHeight.push_back(tempOffsetHeight);
            offsetWidth.push_back(tempOffsetWidth);
            strOffsetType+=strTempOffsetType;
        }
    }
}

void Polygon::process() {
#ifdef DEBUG
    calcLines();
    //printLines();

    makeCoordsRelative();
    //printLines();

    fixOrderLines();
    //printLines();

    deleteExtraLines();
    printLines();

    findOffsets();
    printOffsets();

#endif // DEBUG

#ifndef DEBUG
    calcLines();

    makeCoordsRelative();

    fixOrderLines();

    deleteExtraLines();

    findOffsets();
#endif // !DEBUG

}

//searching positions of element in layout
void Topology::findPosOfElement(std::string strElement, std::string StrLayout) {
    size_t i = 0;

    for (i = StrLayout.find(strElement, i++); i != std::string::npos; i = StrLayout.find(strElement, i + 1)) {
        posOfElement.push_back(i);
    }

    i = 0;
    std::reverse(strElement.begin(), strElement.end());
    for (i = StrLayout.find(strElement, i++); i != std::string::npos; i = StrLayout.find(strElement, i + 1)) {
        posOfReversedElement.push_back(i);
    }
}

//printing positions of element in layout
void Topology::printPosOfElement() {
    if (posOfElement.empty())
        std::cout << "There is no element in layout!" << std::endl;
    else
    {
        std::cout << "Positions of element in layout: ";
        for (size_t i = 0; i < posOfElement.size(); i++)
            std::cout << posOfElement[i] << " ";
        std::cout << std::endl;
    }

    if (posOfReversedElement.empty())
        std::cout << "There is no reversed element in layout!" << std::endl;
    else
    {
        std::cout << "Positions of reversed element in layout: ";
        for (size_t i = 0; i < posOfReversedElement.size(); i++)
            std::cout << posOfReversedElement[i] << " ";
        std::cout << std::endl;
    }
}

void Topology::checkOffset(const Polygon &element, const Polygon &layout) {

    truePosition.resize(posOfElement.size(), true);
    trueReversePosition.resize(posOfReversedElement.size(), true);

    Polygon reverseElement = element;
    std::reverse(reverseElement.offsetHeight.begin(),  reverseElement.offsetHeight.end());
    std::reverse(reverseElement.offsetWidth.begin(),   reverseElement.offsetWidth.end());
    std::reverse(reverseElement.strOffsetType.begin(), reverseElement.strOffsetType.end());

    if (!posOfElement.empty()) {
        std::cout << std::setw(60) << "FORWARD ORDER\n";
        for (size_t i = 0; i < posOfElement.size(); i++)
        {
            for (size_t j = 0; j < element.strOffsetType.size(); j++)
            {
                std::cout << "(" << element.strOffsetType[j] << ")\n";
                std::cout << std::setw(8) << "element "
                          << std::setw(3) << j
                          << " H: " << std::setw(5) << element.offsetHeight[j]
                          << " W: " << std::setw(5) << element.offsetWidth[j] << "\n";

                std::cout << std::setw(8) << "layout " 
                          << std::setw(3) << j + posOfElement[i]
                          << " H: " << std::setw(5) << layout.offsetHeight[j + posOfElement[i]]
                          << " W: " << std::setw(5) << layout.offsetWidth[j + posOfElement[i]] << "\n";

                //special method for 0 and last offsets
                if (j == 0 || j == element.strOffsetType.size() - 1)
                {
                    if (element.offsetHeight[j] == layout.offsetHeight[j + posOfElement[i]] &&
                        element.offsetWidth[j] <= layout.offsetWidth[j + posOfElement[i]])
                    {
                        std::cout << std::setw(30) << "OK " << "\n";
                    }
                    else
                    {
                        std::cout << std::setw(30) << "WRONG " << "\n";
                        truePosition[i] = false;
                    }
                }
                //not first or last
                else
                {
                    if (element.offsetHeight[j] == layout.offsetHeight[j + posOfElement[i]] &&
                        element.offsetWidth[j]  == layout.offsetWidth[j + posOfElement[i]])
                    {
                        std::cout << std::setw(30) << "OK " << "\n";
                    }
                    else
                    {
                        std::cout << std::setw(30) << "WRONG " << "\n";
                        truePosition[i] = false;
                    }
                }
            }
            std::cout << "\n\n";
        }
    }

    if (!posOfReversedElement.empty()) {
        std::cout << std::setw(60) << "REVERSE ORDER\n";
        for (size_t i = 0; i < posOfReversedElement.size(); i++)
        {
            for (size_t j = 0; j < reverseElement.strOffsetType.size(); j++)
            {
                std::cout << "(" << reverseElement.strOffsetType[j] << ")\n";
                std::cout << std::setw(8) << "element "
                          << std::setw(3) << j
                          << " H: " << std::setw(5) << reverseElement.offsetHeight[j]
                          << " W: " << std::setw(5) << reverseElement.offsetWidth[j] << "\n";

                std::cout << std::setw(8) << "layout "
                          << std::setw(3) << j + posOfReversedElement[i]
                          << " H: " << std::setw(5) << layout.offsetHeight[j + posOfReversedElement[i]]
                          << " W: " << std::setw(5) << layout.offsetWidth[j + posOfReversedElement[i]] << "\n";

                //special method for 0 and last offsets
                if (j == 0 || j == element.strOffsetType.size() - 1)
                {
                    if (reverseElement.offsetHeight[j] == layout.offsetHeight[j + posOfReversedElement[i]] &&
                        reverseElement.offsetWidth[j] <= layout.offsetWidth[j + posOfReversedElement[i]])
                    {
                        std::cout << std::setw(30) << "OK " << "\n";
                    }
                    else
                    {
                        std::cout << std::setw(30) << "WRONG " << "\n";
                        trueReversePosition[i] = false;
                    }
                }
                //not first or last
                else
                {
                    if (reverseElement.offsetHeight[j] == layout.offsetHeight[j + posOfReversedElement[i]] &&
                        reverseElement.offsetWidth[j] == layout.offsetWidth[j + posOfReversedElement[i]])
                    {
                        std::cout << std::setw(30) << "OK " << "\n";
                    }
                    else
                    {
                        std::cout << std::setw(30) << "WRONG " << "\n";
                        trueReversePosition[i] = false;
                    }
                }
            }
            std::cout << "\n\n";
        }
    }

    

    for (size_t i = 0; i < truePosition.size(); i++)
    {
        if (truePosition[i] == true)
            std::cout << "(FORWARD) Position " << i << " is true\n";
        else
            std::cout << "(FORWARD) Position " << i << " is wrong\n";
    }

    for (size_t i = 0; i < trueReversePosition.size(); i++)
    {
        if (trueReversePosition[i] == true)
            std::cout << "(REVERSED) Position " << i << " is true\n";
        else
            std::cout << "(REVERSED) Position " << i << " is wrong\n";
    }
}