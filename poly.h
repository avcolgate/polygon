#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <algorithm>

//#define  DEBUG

enum class LineOrientation {
    undefined = 0,
    vertical,
    horizontal
};

enum class OffsetType {
    undefined = 0,
    edge,
    in,
    out
};

class Point {
public:
    int32_t x;
    int32_t y;

    Point():x(0),y(0) { }

    Point(int32_t x, int32_t y) :x(x), y(y) {}
    
};

class Line {
public:
    Point            start;
    Point            end;
                 
    uint32_t         length;
    LineOrientation  orient;

};

class Polygon{
public: 
    std::vector<Line>          line;
    std::string                strOffsetType;
    std::vector<int32_t>       offsetHeight;
    std::vector<int32_t>       offsetWidth;
    std::vector<Point>         offsetCoords;

    bool                       widthOnTop;
  
    Point                      min, max;
    uint32_t                   width, height;

    Polygon() :max{ std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min() },
               min{ std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max() },
               width{ 0 }, height{ 0 }, widthOnTop{ 0 } { }

    bool readFile(const std::string& fileName);
    void printLines();
    void printOffsets();
    void calcLines();
    void findOffsets();
    void makeCoordsRelative();
    void deleteExtraLines();
    void fixOrderLines();

    void process();
};

class Topology{
public:
    std::vector<uint32_t>      potentialPositions;
    std::vector<uint32_t>      potentialPositionsReversed;
                               
    std::vector<bool>          correctPosition;
    std::vector<bool>          correctReversePosition;
                               
    std::vector<Point>         correctPoints;

    void findPosOfElement(std::string element, std::string layout);
    void printPosOfElement();

    void checkOffset(const Polygon &element, const Polygon &layout);

    std::vector<Point>Find(const Polygon& element, const Polygon& layout);
};
