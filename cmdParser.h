#pragma once
#include <iostream>
#include <vector>
#include "types.h"

enum CubeFace {
         CF_FRONT
        , CF_LEFT
        , CF_RIGHT
        , CF_TOP
        , CF_NONE
};

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

private:
    void assignParams();
    void assignMode(std::string inParam);
    void sortDirFromUser();
    void rollDirFromUser();
};
