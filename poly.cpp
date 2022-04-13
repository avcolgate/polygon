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
        file >> tempLine.finish.x >> tempLine.finish.y;

        //calculating length and orientation
        tempLen = abs(tempLine.finish.x - tempLine.start.x);
        if (tempLen == 0) {
            tempLen = abs(tempLine.finish.y - tempLine.start.y);
            tempLine.orient = LineOrientation::vertical;
        }
        else {
            tempLine.orient = LineOrientation::horizontal;
        }
        tempLine.length = tempLen;
        line.push_back(tempLine);

        tempLine.start.x = tempLine.finish.x;
        tempLine.start.y = tempLine.finish.y;

    } while (!(tempLine.finish.x == line.at(0).start.x && tempLine.finish.y == line.at(0).start.y));
    
    file.close();
    return true;
}

void Polygon::printLines() {
    using namespace std;
   
    string tempOrient;
    cout << "Lines table:\n";
    cout << setw(18) << "start X" << setw(15) << "start Y"
        << setw(15) << "finish X" << setw(15) << "finish Y"
        << setw(16) << "length" << setw(16) << "orientation\n";
    for (size_t i = 0; i < line.size(); i++) {
        tempOrient = eLineOrientToStr(line[i].orient);

        cout << "(" << i << ")" << setw(15) << line[i].start.x << setw(15) << line[i].start.y
            << setw(15) << line[i].finish.x << setw(15) << line[i].finish.y
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
    if (line[widthNum].finish.y > line[nextNum].finish.y) {
        widthOnTop = true;
        //max
        if (line[widthNum].finish.x > line[widthNum].start.x) {
            max.x = line[widthNum].finish.x;
            max.y = line[widthNum].finish.y;
        }
        else {
            max.x = line[widthNum].start.x;
            max.y = line[widthNum].start.y;
        }
        //min
        if (line[nextNum].finish.x < line[prevNum].start.x) {
            min.x = line[nextNum].finish.x;
            min.y = line[nextNum].finish.y;
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
        if (line[nextNum].finish.x > line[prevNum].start.x) {
            max.x = line[nextNum].finish.x;
            max.y = line[nextNum].finish.y;
        }
        else {
            max.x = line[prevNum].start.x;
            max.y = line[prevNum].start.y;
        }
        //min
        if (line[widthNum].finish.x < line[widthNum].start.x) {
            min.x = line[widthNum].finish.x;
            min.y = line[widthNum].finish.y;
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

        line[i].finish.x = line[i].finish.x - min.x;
        line[i].finish.y = line[i].finish.y - min.y;
    }

#ifdef DEBUG
    std::cout << std::setw(70) << "Coordinates changed to relative!" << std::endl;
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

void Polygon::fixOrderLines() {
    //Placing coords in right order (0;0) is on the start
    size_t i = 0;
    bool is_clockwise = false;
    while (! (line[i].start.x == 0 && line[i].start.y == 0) ) {
        line.push_back(line[i]);
        i++;
    }
    line.erase(line.begin(), line.begin() + i);

    //size_t i = 0;
    //while (!((line[i].start.x == 0 && line[i].start.y == 0) || (line[i].finish.x == 0 && line[i].finish.y == 0))) {
    //    line.push_back(line[i]);
    //    i++;
    //}
    //line.erase(line.begin(), line.begin() + i);
    //std::cout << std::setw(70) << "Order of coordinates was fixed!" << std::endl;

    if (line[0].finish.x == 0) 
    {
        is_clockwise = true;
    }
    else 
    {
        is_clockwise = false;
        for (int32_t i = 0; i < line.size(); i++)
        {
            Line tempLine;
            tempLine.finish = line[i].finish;
            line[i].finish = line[i].start;
            line[i].start = tempLine.finish;
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
        std::cout << "Anticlockwise! Lines was reversed!\n" << "start <> finish\n";
    }
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

            //condition for pushing -- we don't add side "edge" Offsets
            if (!(strTempOffsetType == "e" && (i == 0 || i == line.size() - 1) ))
            {
                offsetHeight.push_back(tempOffsetHeight);
                offsetWidth.push_back(tempOffsetWidth);
                strOffsetType+=strTempOffsetType;
            }
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
    //printLines();

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

//searching positions of target polygon in main polygon
void Layout::findPosOfTarget(std::string strTarget, std::string StrMain) {
    size_t i = 0;

    for (i = StrMain.find(strTarget, i++); i != std::string::npos; i = StrMain.find(strTarget, i + 1))
        posOfTarget.push_back(i);

    i = 0;
    std::reverse(strTarget.begin(), strTarget.end());
    for (i = StrMain.find(strTarget, i++); i != std::string::npos; i = StrMain.find(strTarget, i + 1))
        posOfReversedTarget.push_back(i);
}

//printing positions of target polygon in main polygon
void Layout::printPosOfTarget() {
    if (posOfTarget.empty())
        std::cout << "There is no target in main!" << std::endl;
    else
    {
        std::cout << "Positions of target in main: ";
        for (size_t i = 0; i < posOfTarget.size(); i++)
            std::cout << posOfTarget[i] << " ";
        std::cout << std::endl;
    }

    if (posOfReversedTarget.empty())
        std::cout << "There is no reversed target in main!" << std::endl;
    else
    {
        std::cout << "Positions of reversed target in main: ";
        for (size_t i = 0; i < posOfReversedTarget.size(); i++)
            std::cout << posOfReversedTarget[i] << " ";
        std::cout << std::endl;
    }
}

void Layout::checkOffset(const Polygon &target, const Polygon &main) {

    truePosition.resize(posOfTarget.size(), true);
    trueReversePosition.resize(posOfReversedTarget.size(), true);

    Polygon reverseTarget = target;
    std::reverse(reverseTarget.offsetHeight.begin(),  reverseTarget.offsetHeight.end());
    std::reverse(reverseTarget.offsetWidth.begin(),   reverseTarget.offsetWidth.end());
    std::reverse(reverseTarget.strOffsetType.begin(), reverseTarget.strOffsetType.end());

    std::cout << std::setw(60) << "FORWARD ORDER\n";
    for (size_t i = 0; i < posOfTarget.size(); i++)
    {
        for (size_t j = 0; j < target.strOffsetType.size(); j++)
        {
            std::cout << "(" << target.strOffsetType[j] << ")\n";
            std::cout << std::setw(7) << "target "
                      << std::setw(3) << j
                      << " H: " << std::setw(5) << target.offsetHeight[j]
                      << " W: " << std::setw(5) << target.offsetWidth[j] << "\n";

            std::cout << std::setw(7) << "main " 
                      << std::setw(3) << j + posOfTarget[i]
                      << " H: " << std::setw(5) << main.offsetHeight[j + posOfTarget[i]]
                      << " W: " << std::setw(5) << main.offsetWidth[j + posOfTarget[i]] << "\n";

            if (target.offsetHeight[j] == main.offsetHeight[j + posOfTarget[i]] &&
                target.offsetWidth[j]  == main.offsetWidth[j + posOfTarget[i]])
            {
                std::cout << std::setw(30) << "EQUAL " << "\n";
            }
            else
            {
                std::cout << std::setw(30) << "NOT EQUAL " << "\n";
                truePosition[i] = false;
            }
        }
        std::cout << "\n\n";
    }

    std::cout << std::setw(60) << "REVERSE ORDER\n";
    for (size_t i = 0; i < posOfReversedTarget.size(); i++)
    {
        for (size_t j = 0; j < reverseTarget.strOffsetType.size(); j++)
        {
            std::cout << "(" << reverseTarget.strOffsetType[j] << ")\n";
            std::cout << std::setw(7) << "target "
                      << std::setw(3) << j
                      << " H: " << std::setw(5) << reverseTarget.offsetHeight[j]
                      << " W: " << std::setw(5) << reverseTarget.offsetWidth[j] << "\n";

            std::cout << std::setw(7) << "main "
                      << std::setw(3) << j + posOfReversedTarget[i]
                      << " H: " << std::setw(5) << main.offsetHeight[j + posOfReversedTarget[i]]
                      << " W: " << std::setw(5) << main.offsetWidth[j + posOfReversedTarget[i]] << "\n";

            if (reverseTarget.offsetHeight[j] == main.offsetHeight[j + posOfReversedTarget[i]] &&
                reverseTarget.offsetWidth[j] == main.offsetWidth[j + posOfReversedTarget[i]])
            {
                std::cout << std::setw(30) << "EQUAL " << "\n";
            }
            else
            {
                std::cout << std::setw(30) << "NOT EQUAL " << "\n";
                trueReversePosition[i] = false;
            }
        }
        std::cout << "\n\n";
    }

    for (size_t i = 0; i < truePosition.size(); i++)
    {
        if (truePosition[i] == true)
            std::cout << "(FW) Position " << i << " is true\n";
        else
            std::cout << "(FW) Position " << i << " is wrong\n";
    }

    for (size_t i = 0; i < trueReversePosition.size(); i++)
    {
        if (trueReversePosition[i] == true)
            std::cout << "(RV) Position " << i << " is true\n";
        else
            std::cout << "(RV) Position " << i << " is wrong\n";
    }
}