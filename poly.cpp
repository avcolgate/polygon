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
        return "in";
    else if (type == OffsetType::out)
        return "out";
    else if (type == OffsetType::edge)
        return "edge";

    return "undefined";
}

std::string offsetTypeToShort(const std::string type) {
    if (type == "in")
        return "i";
    else if (type == "out")
        return "o";
    else if (type == "edge")
        return "e";

    return "_";
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

        tempLen = abs(tempLine.finish.x - tempLine.start.x); //trying to get X length (vertical)
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
    for (size_t i = 0; i < offsetType.size(); i++) {
        //tempType = eOffsetTypeToStr(offsetType[i].offsetType);
    
        cout << "(" << i << ")" << setw(9) << offsetWidth[i] << setw(8) << offsetHeight[i]
            << setw(7) << offsetType[i] << "\n";
    }
    cout << "\n";
}

void Polygon::calcLines() {
    size_t widthNum = 0, nextNum = 0, prevNum = 0;

    //WIDTH & HEIGHT search
    for (size_t i = 0; i < line.size(); i++) {
        if (line[i].length > width && line[i].orient == LineOrientation::horizontal) {
            width = line[i].length;
            widthNum = i;
        }
            
        if (widthNum == line.size() - 1) { //if last
            prevNum = widthNum - 1;
            nextNum = 0;
        }
        else if (widthNum == 0) {          //if first
            prevNum = line.size() - 1;
            nextNum = widthNum + 1;
        }
        else {
            prevNum = widthNum - 1;
            nextNum = widthNum + 1;
        }

        if (line[nextNum].length > line[prevNum].length)
            height = line[nextNum].length;
        else
            height = line[prevNum].length;
    }

    std::cout << "Width is line number " << widthNum << std::endl << "Width's length " << line[widthNum].length << std::endl;

    //if width is above
    if (line[widthNum].finish.y > line[nextNum].finish.y) {
        std::cout << "Width is on the top!" << std::endl;
        WidthOnTop = true;
        //max
        if (line[widthNum].finish.x > line[widthNum].start.x) {
            max.x = line[widthNum].finish.x;
            max.y = line[widthNum].finish.y;
        }
        else {
            max.x = line[widthNum].start.x;
            max.y = line[widthNum].start.y;
        }
        //min (x)
        if (line[nextNum].finish.x < line[prevNum].start.x) {
            min.x = line[nextNum].finish.x;
            min.y = line[nextNum].finish.y;
        }
        else {
            min.x = line[prevNum].start.x;
            min.y = line[prevNum].start.y;
        }
    }
    //if width is below
    else {
        std::cout << "Width is on the bottom!" << std::endl;
        WidthOnTop = false;
        //max
        if (line[nextNum].finish.x > line[prevNum].start.x) {
            max.x = line[nextNum].finish.x;
            max.y = line[nextNum].finish.y;
        }
        else {
            max.x = line[prevNum].start.x;
            max.y = line[prevNum].start.y;
        }
        //min (x)
        if (line[widthNum].finish.x < line[widthNum].start.x) {
            min.x = line[widthNum].finish.x;
            min.y = line[widthNum].finish.y;
        }
        else {
            min.x = line[widthNum].start.x;
            min.y = line[widthNum].start.y;
        }
    }
}

void Polygon::MakeCoordsRelative() {
    //Making coords relative
    for (size_t i = 0; i < line.size(); i++) {
        line[i].start.x = line[i].start.x - min.x;
        line[i].start.y = line[i].start.y - min.y;

        line[i].finish.x = line[i].finish.x - min.x;
        line[i].finish.y = line[i].finish.y - min.y;
    }
    std::cout << std::setw(70) << "Coordinates changed to relative!" << std::endl;
}

void Polygon::DeletingExtraLines() {
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

    std::cout << std::setw(70) << "Three sides was deleted!" << std::endl;
}

void Polygon::FixOrderLines() {
    //Placing coords in right order (0;0) is on the start
    size_t i = 0;
    bool is_clockwise = false;
    while (! (line[i].start.x == 0 && line[i].start.y == 0) ) {
        line.push_back(line[i]);
        i++;
    }
    line.erase(line.begin(), line.begin() + i);
    std::cout << std::setw(70) << "Order of coordinates was fixed!" << std::endl;

    //size_t i = 0;
    //while (!((line[i].start.x == 0 && line[i].start.y == 0) || (line[i].finish.x == 0 && line[i].finish.y == 0))) {
    //    line.push_back(line[i]);
    //    i++;
    //}
    //line.erase(line.begin(), line.begin() + i);
    //std::cout << std::setw(70) << "Order of coordinates was fixed!" << std::endl;

    //printLines();
    if (line[0].finish.x == 0) {
        is_clockwise = true;
        std::cout << "Clockwise! No reverse needed.\n";
    }
    else {
        is_clockwise = false;
        std::cout << "Anticlockwise! Lines was reversed!\n";
        std::cout << "start <> finish\n";

        for (int32_t i = 0; i < line.size(); i++) {
            Line tempLine;
            tempLine.finish = line[i].finish;
            line[i].finish = line[i].start;
            line[i].start = tempLine.finish;
        }

        //printLines();

        std::reverse(line.begin(), line.end());
    }
}

void Polygon::findOffsets() {
    //Offset tempOffset;
    std::string   tempOffsetType;
    int32_t      tempOffsetHeight;
    int32_t      tempOffsetWidth;


    for (size_t i = 0; i < line.size(); i++) {
        if (line[i].orient == LineOrientation::horizontal) {

            if (true == WidthOnTop) {   //top
     
                tempOffsetWidth = line[i].length;
                tempOffsetHeight = (line[i].start.y);
                std::cout << tempOffsetHeight << "\n";

                if (tempOffsetHeight == 0)
                    tempOffsetType = eOffsetTypeToStr(OffsetType::edge);
                else if (tempOffsetHeight < 0)
                    tempOffsetType = eOffsetTypeToStr(OffsetType::out);
                else if (tempOffsetHeight > 0)
                    tempOffsetType = eOffsetTypeToStr(OffsetType::in);
            }
            else {                                         //bottom
               
                tempOffsetWidth = line[i].length;
                tempOffsetHeight = (line[i].start.y - (max.y - min.y));
                std::cout << tempOffsetHeight << "\n";

                if (tempOffsetHeight == 0)
                    tempOffsetType = eOffsetTypeToStr(OffsetType::edge);
                else if (tempOffsetHeight > 0)
                    tempOffsetType = eOffsetTypeToStr(OffsetType::out);
                else if (tempOffsetHeight < 0)
                    tempOffsetType = eOffsetTypeToStr(OffsetType::in);
            }

            offsetType.push_back(tempOffsetType);
            offsetHeight.push_back(tempOffsetHeight);
            offsetWidth.push_back(tempOffsetWidth);

        }
    }
}