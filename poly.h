#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <algorithm>

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

//class Offset {
//public:
//    OffsetType offsetType;
//    
//    uint32_t width, height;
//
//    Offset() : offsetType{ OffsetType::undefined }, width { 0 }, height{ 0 } { }
//};

class Line {
public:
    Point        start;
    Point        finish;
                 
    uint32_t     length;
    LineOrientation  orient;

};

class Polygon {
public:
    std::vector<Line> line;
    //std::vector<Offset> offset;
    std::vector<std::string>   offsetType;
    std::vector<int32_t>      offsetHeight;
    std::vector<int32_t>      offsetWidth;
    bool                       WidthOnTop;
  
    Point min, max;

    uint32_t width, height;

    Polygon() :max{ std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::min() },
        min{ std::numeric_limits<int32_t>::max(), std::numeric_limits<int32_t>::max() },
        width{ 0 }, height{ 0 }, WidthOnTop{ 0 } { }

    bool readFile(const std::string& fileName);
    void printLines();
    void printOffsets();
    void calcLines();
    void findOffsets();
    void MakeCoordsRelative();
    void DeletingExtraLines();
    void FixOrderLines();
};

class Layout {
    std::vector<Polygon> polygons;
};