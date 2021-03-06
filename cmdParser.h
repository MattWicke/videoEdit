#pragma once
#include <iostream>
#include <vector>
#include "types.h"


enum EffectMode {NO
    , RGB_DELAY
        , PIXEL_SORT
        , STACK
        , TRAILS
        , ROLL
        , KALE
        , ERC
};
enum PSortMode {HORZ, VERT};
class CmdParser
{
public:
    std::vector<std::string> args;
    CmdParser(int m_argc, char* m_argv[]);
    std::string operator[](size_t ii);
    EffectMode eMode;
    PSortMode psMode;
    RollMode rollMode;
    CubeFace cf;
    void printHelp();
    int loops;
    int timeLapse;

private:
    void assignParams();
    void assignMode(std::string inParam);
    void sortDirFromUser();
    void rollDirFromUser();
};
