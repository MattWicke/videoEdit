#include "cmdParser.h"
CmdParser::CmdParser(int m_argc, char* m_argv[])
{
    for(int ii = 0; ii < m_argc; ii++)
    {
        args.push_back(std::string(m_argv[ii]));
    }
    assignParams();
}

void CmdParser::assignParams()
{
    int ii = 0;
    while(ii < args.size())
    {
        std::cout << ii << std::endl;
        if(args[ii] == "-m")
        {
            ii++;
            assignMode(args[ii]);
        }
        ii++;
    }
}

void CmdParser::assignMode(std::string inParam)
{
    if(inParam == "rgb")
    {
        eMode = RGB_DELAY;
        std::cout << "Mode set to RGB Delay" << std::endl;
    }

    if(inParam == "srt")
    {
        eMode = PIXEL_SORT;
        sortDirFromUser();
        std::cout << "Mode set to Pixel Sort" << std::endl;
    }

    if(inParam == "sta")
    {
        eMode = STACK;
        std::cout << "Mode set to Stack" << std::endl;
    }

    if(inParam == "tra")
    {
        eMode = TRAILS;
        std::cout << "Mode set to Trails" << std::endl;
    }

    if(inParam == "rol")
    {
        eMode = ROLL;
        rollDirFromUser();
        std::cout << "Mode set to Roll" << std::endl;
    }

    if(inParam == "kal")
    {
        eMode = KALE;
        std::cout << "Mode set to kaleido" << std::endl;
    }

    if(inParam == "kal")
    {
        eMode = KALE;
        std::cout << "Mode set to kaleido" << std::endl;
    }

    if(inParam == "erc")
    {
        eMode = ERC;
        std::cout << "Mode set to equirectangular" << std::endl;
    }

    if(inParam == "non")
    {
        eMode = NO;
        std::cout << "Mode set to none" << std::endl;
    }

    std::cout << "Loop video? 0 for no loops" << std::endl;
    std::cin >> loops;
}

void CmdParser::printHelp()
{
    std::cout << "vidEdit [fileName.mp4] -m [filter] " << std::endl;
    std::cout << " [rgb] RGB delay filter" << std::endl;
    std::cout << " [srt] pixel sorting filter" << std::endl;
    std::cout << " [sta] stack frames filter" << std::endl;
    std::cout << " [tra] trails filter" << std::endl;
    std::cout << " [rol] rolling filter" << std::endl;
    std::cout << " [kal] kaleidoscope filter" << std::endl;
    std::cout << " [erc] equirectangluar to cube faces" << std::endl;
}

void CmdParser::sortDirFromUser()
{
    char input = 0;

    while((input  != 'h')&&(input  != 'v'))
    {
        std::cout << "Enter a direction" << std::endl;
        std::cout << " horizontal h" << std::endl;
        std::cout << " vertical v" << std::endl;
        std::cin >> input;
    }

    switch(input)
    {
        case 'h':
        psMode = HORZ;
        break;

        case 'v':
        psMode = VERT;
        break;
    }
}

void CmdParser::rollDirFromUser()
{
    char input = 0;

    while(   (input  != 'u')
           &&(input  != 'd')
           &&(input  != 'l')
           &&(input  != 'r'))
    {
        std::cout << "Enter a direction" << std::endl;
        std::cout << " u up" << std::endl;
        std::cout << " d down" << std::endl;
        std::cout << " l left" << std::endl;
        std::cout << " r right" << std::endl;
        std::cin >> input;
    }

    switch(input)
    {
        case 'u':
            rollMode = RM_UP;
        break;
        case 'd':
            rollMode = RM_DOWN;
        break;
        case 'l':
            rollMode = RM_LEFT;
        break;
        case 'r':
            rollMode = RM_RIGHT;
        break;
    }
}


std::string CmdParser::operator[](size_t ii)
{
    return args[ii];
}
