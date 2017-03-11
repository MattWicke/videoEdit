#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <unistd.h>
#include <vector>
#include <string>
#include "VideoWrapper.h"



enum State {ST_START, ST_STOP};
class CmdParser
{
public:
    std::vector<std::string> args;
    CmdParser(int m_argc, char* m_argv[]);
    std::string operator[](size_t ii);
};

CmdParser::CmdParser(int m_argc, char* m_argv[])
{
    for(int ii = 0; ii < m_argc; ii++)
    {
        args.push_back(std::string(m_argv[ii]));
    }
}

std::string CmdParser::operator[](size_t ii)
{
    return args[ii];
}

typedef struct StateInfo
{

};
class StateMachine
{
public:
    State state;
    void cropPhase();
private:
    CmdParser parser;
    std::vector<VideoWrapper> vidWrappers;
};

void StateMachine::cropPhase();
{

}

int main(int argc, char* argv[])
{
    CmdParser parser(argc, argv);
    VideoWrapper vw(parser[1]);
    vw.record(parser[2]);
}
