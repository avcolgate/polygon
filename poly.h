#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <algorithm>

#define  DEBUG

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
    bool                       widthOnTop;
  
    Point min, max;

    uint32_t width, height;

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

class Layout{
public:
    std::vector<uint32_t> posOfTarget;
    std::vector<uint32_t> posOfReversedTarget;

    std::vector<bool>     truePosition;
    std::vector<bool>     trueReversePosition;

    void findPosOfTarget(std::string target, std::string main);
    void printPosOfTarget();

    //void checkOffset(
    //    std::vector<uint32_t> positions,
    //
    //    std::string strTarget,
    //    std::vector<int32_t> heightTarget,
    //    std::vector<int32_t> widthTarget,
    //
    //    std::string strMain,
    //    std::vector<int32_t> heightMain,
    //    std::vector<int32_t> widthMain);

    void checkOffset(const Polygon &target, const Polygon &main);

};
