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

    std::ifstream file(fileName);
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
        std::cout << "Anticlockwise! Lines was reversed!\n";
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

            //adding start and end
            Line tempLine;
            tempLine.start = { line[i].start.x + min.x, line[i].start.y + min.y };
            tempLine.end =   { line[i].end.x + min.x,   line[i].end.y + min.y };

            //offsetRelativeCoords.push_back({ line[i].start.x, line[i].start.y });
            offsetCoords.push_back(tempLine);
            
        }
    }
}

void Polygon::process() {

    calcLines();

    makeCoordsRelative();

    fixOrderLines();

    deleteExtraLines();

    findOffsets();

#ifdef DEBUG

    printLines();
    printOffsets();

#endif // DEBUG

}

//searching positions of element in layout
void Topology::findPositions(std::string strElement, std::string StrLayout) {
    size_t i = 0;

    for (i = StrLayout.find(strElement, i++); i != std::string::npos; i = StrLayout.find(strElement, i + 1)) {
        potentialPositionsFW.push_back(i);
    }

    i = 0;
    std::reverse(strElement.begin(), strElement.end());
    for (i = StrLayout.find(strElement, i++); i != std::string::npos; i = StrLayout.find(strElement, i + 1)) {
        potentialPositionsRV.push_back(i);
    }
}

//printing positions of element in layout
void Topology::printPositions() {
    using namespace std;
    if (potentialPositionsFW.empty())
        cout << "There is no element in layout!" << endl;
    else
    {
        cout << "Positions of element in layout: ";
        for (size_t i = 0; i < potentialPositionsFW.size(); i++)
            cout << potentialPositionsFW[i] << " ";
        cout << endl;
    }

    if (potentialPositionsRV.empty())
        cout << "There is no reversed element in layout!" << endl;
    else
    {
        cout << "Positions of reversed element in layout: ";
        for (size_t i = 0; i < potentialPositionsRV.size(); i++)
            cout << potentialPositionsRV[i] << " ";
        cout << endl;
    }
}

void Topology::Find(const Polygon &element, const Polygon &layout) {

    correctReversePosition.resize(potentialPositionsRV.size(), true);

    Point tempLayoutOffsetStart;
    Point tempLayoutOffsetEnd; 
    
    int32_t elemDeltaXstart = 0;
    int32_t elemDeltaYstart = 0;

    int32_t elemDeltaXend = 0;
    int32_t elemDeltaYend = 0;

    Polygon reverseElement = element;
    std::reverse(reverseElement.offsetHeight.begin(),  reverseElement.offsetHeight.end());
    std::reverse(reverseElement.offsetWidth.begin(),   reverseElement.offsetWidth.end());
    std::reverse(reverseElement.strOffsetType.begin(), reverseElement.strOffsetType.end());

    //////////////////////////////////////        FORWARD       //////////////////////////////////////////////
    if (!potentialPositionsFW.empty()) {
        std::cout << std::setw(60) << "FORWARD ORDER\n";
    }

    for (size_t i = 0; i < potentialPositionsFW.size(); i++)
    {
        boolCorrectPosition.clear();
        for (size_t j = 0; j < element.strOffsetType.size(); j++)
        {
            std::cout << "(" << element.strOffsetType[j] << ")\n";

            std::cout << std::setw(8) << "element " << std::setw(3) << j
                      << " H: " << std::setw(5) << element.offsetHeight[j]
                      << " W: " << std::setw(5) << element.offsetWidth[j] << "\n";

            std::cout << std::setw(8) << "layout " << std::setw(3) << j + potentialPositionsFW[i]
                      << " H: " << std::setw(5) << layout.offsetHeight[j + potentialPositionsFW[i]]
                      << " W: " << std::setw(5) << layout.offsetWidth[j + potentialPositionsFW[i]] << "\n";

            //special method for zero and last offsets of ELEMENT
            if (j == 0 || j == element.strOffsetType.size() - 1)
            {
                if (element.offsetHeight[j] == layout.offsetHeight[j + potentialPositionsFW[i]] &&
                    element.offsetWidth[j]  <= layout.offsetWidth [j + potentialPositionsFW[i]])
                {
                    //std::cout << j + potentialPositionsFW[i] << std::endl;
                    boolCorrectPosition.push_back(true);

                    //calculating element deltas
                    // 
                    //first offset in element
                    if (j == 0)
                    {
                        elemDeltaXstart = element.offsetWidth[j];        //for addition to X coord 
                        elemDeltaYstart = element.offsetHeight[j + 1];   //for addition to Y coord (abs)

                        //picking the next offset in layout
                        tempLayoutOffsetStart = { layout.offsetCoords[ (j + 1) + potentialPositionsFW[i]].start.x + elemDeltaXstart,
                                                  layout.offsetCoords[ (j + 1) + potentialPositionsFW[i]].start.y + abs(elemDeltaYstart) };

                        std::cout << "elem delta X start: "        << elemDeltaXstart << std::endl;
                        std::cout << "elem delta Y start (next): " << elemDeltaYstart << std::endl;

                        std::cout << "tempLayoutOffsetStart x: " << tempLayoutOffsetStart.x << std::endl;
                        std::cout << "tempLayoutOffsetStart y: " << tempLayoutOffsetStart.y << std::endl;
                    }

                    //last offset in element
                    if (j == element.strOffsetType.size() - 1)
                    {
                        elemDeltaXend = element.offsetWidth[j];         //for subtract from X coord
                        elemDeltaYend = element.offsetHeight[j - 1];    //for subtract from Y coord (abs)

                        //picking the previous offset in layout
                        tempLayoutOffsetEnd = { layout.offsetCoords[ (j - 1) + potentialPositionsFW[i]].end.x - elemDeltaXend,
                                                layout.offsetCoords[ (j - 1) + potentialPositionsFW[i]].end.y - abs(elemDeltaYend)};

                        std::cout << "elem delta X end: "        << elemDeltaXend << std::endl;
                        std::cout << "elem delta Y end (prev): " << elemDeltaYend << std::endl;

                        std::cout << "tempLayoutOffsetEnd x: " << tempLayoutOffsetEnd.x << std::endl;
                        std::cout << "tempLayoutOffsetEnd y: " << tempLayoutOffsetEnd.y << std::endl;
                    }

                }
                else
                {
                    boolCorrectPosition.push_back(false);
                    //break;
                }
            }
            //ordinary method for not first or last
            else
            {
                if (element.offsetHeight[j] == layout.offsetHeight[j + potentialPositionsFW[i]] &&
                    element.offsetWidth[j]  == layout.offsetWidth [j + potentialPositionsFW[i]])
                {
                    boolCorrectPosition.push_back(true);
                    //if (j == 1)
                    //{
                    //    
                    //                                                                                   
                    //    std::cout << j + potentialPositionsFW[i] << std::endl;
                    //}
                    //
                    //if (j == element.strOffsetType.size() - 2)
                    //{
                    //    
                    //
                    //    std::cout << j + potentialPositionsFW[i] << std::endl;
                    //}
                }
                else 
                {
                    boolCorrectPosition.push_back(false);
                    //break;
                }
            }
        }

        if (!potentialPositionsFW.empty()) {
            int numOfTrue = 0;

            for (size_t i = 0; i < boolCorrectPosition.size(); i++) {
                if (boolCorrectPosition[i])
                {
                    numOfTrue++;
                }
            }

            if (numOfTrue == boolCorrectPosition.size()) {
                std::cout << "Offset is true!!!\n";
                correctPoints.push_back(tempLayoutOffsetStart);
                correctPoints.push_back(tempLayoutOffsetEnd);
            }
            else {
                std::cout << "Offset is false!!!\n";
            }
            std::cout << "\n";
        }
    }

    ////////////////////////////////////      REVERSE     //////////////////////////
    if (!potentialPositionsRV.empty()) {
        std::cout << std::setw(60) << "REVERSE ORDER\n";
    }

    for (size_t i = 0; i < potentialPositionsRV.size(); i++)
    {
        boolCorrectPosition.clear();
        for (size_t j = 0; j < reverseElement.strOffsetType.size(); j++)
        {
            std::cout << "(" << reverseElement.strOffsetType[j] << ")\n";

            std::cout << std::setw(8) << "element" << std::setw(3) << j
                      << " H: " << std::setw(5) << reverseElement.offsetHeight[j]
                      << " W: " << std::setw(5) << reverseElement.offsetWidth[j] << "\n";

            std::cout << std::setw(8) << "layout" << std::setw(3) << j + potentialPositionsRV[i]
                      << " H: " << std::setw(5) << layout.offsetHeight[j + potentialPositionsRV[i]]
                      << " W: " << std::setw(5) << layout.offsetWidth[j + potentialPositionsRV[i]] << "\n";

            //special method for zero and last offsets
            if (j == 0 || j == element.strOffsetType.size() - 1)
            {
                if (reverseElement.offsetHeight[j] == layout.offsetHeight[j + potentialPositionsRV[i]] &&
                    reverseElement.offsetWidth[j] <= layout.offsetWidth[j + potentialPositionsRV[i]])
                {
                    boolCorrectPosition.push_back(true);

                    //adding only first point of offset
                    if (j == 0) {
                        //tempLayoutOffsetStart = { layout.offsetCoords[j + potentialPositionsRV[i]].x,
                        //             layout.offsetCoords[j + potentialPositionsRV[i]].y };
                    }
                }
                else
                {
                    boolCorrectPosition.push_back(false);
                    //break;
                }
            }
            //ordinary method for not first or last
            else
            {
                if (reverseElement.offsetHeight[j] == layout.offsetHeight[j + potentialPositionsRV[i]] &&
                    reverseElement.offsetWidth[j] == layout.offsetWidth[j + potentialPositionsRV[i]])
                {
                    boolCorrectPosition.push_back(true);
                }
                else
                {
                    boolCorrectPosition.push_back(false);
                    //break;
                }
            }
        }
        if (!potentialPositionsRV.empty()) {
            int numOfTrue = 0;

            for (size_t i = 0; i < boolCorrectPosition.size(); i++) {
                if (boolCorrectPosition[i])
                {
                    numOfTrue++;
                }
            }

            if (numOfTrue == boolCorrectPosition.size()) {
                std::cout << "Offset is true!!!\n";
                correctPoints.push_back(tempLayoutOffsetStart);
            }
            else {
                std::cout << "Offset is false!!!\n";
            }
            std::cout << "\n";
        }
    }
}

bool writeFile(const std::string& fileName, const Topology& topology)
{
    using namespace std;

    ofstream file(fileName);

    if (!file)
        return EXIT_FAILURE;

    for (size_t i = 0; i < topology.correctPoints.size(); i++)
    {
        file << topology.correctPoints[i].x << " " << topology.correctPoints[i].y;

        cout << topology.correctPoints[i].x << " " << topology.correctPoints[i].y;
        if (i % 4) {
            std::cout << "\n";
            file << "\n";
        }
        else {
            std::cout << " ";
            file << " ";
        }
    }

    file.close();
    return true;
}
