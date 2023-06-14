/**
 * Created Date: Wednesday June 7th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Wednesday June 7th 2023 8:01:03 pm
 * Modified By: DefinitelyNotAGirl@github (definitelynotagirl115169@gmail.com)
 * -----
 * Copyright (c) 2023 DefinitelyNotAGirl@github
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <cargparse.hxx>

#include <iostream>

#include <main.hxx>

#include <file.h>

void info(std::string s)
{
    std::cout << s << std::endl;
}
void error(std::string e)
{
    std::cerr << e << std::endl;
}

cargparse::handlertype handler_seperate(CARGPARSE_HANDLER_ARGS)
{
    seperateFiles = true;
    std::cout << "using seperate config files..." << std::endl;
}
cargparse::handlertype handler_spawns(CARGPARSE_HANDLER_ARGS)
{
    json_spawns = args.front();
    std::cout << "spawn config file: " << args.front() << std::endl;
}
cargparse::handlertype handler_stacks(CARGPARSE_HANDLER_ARGS)
{
    json_stacks = args.front();
    std::cout << "stack config file: " << args.front() << std::endl;
}
cargparse::handlertype handler_autospawn(CARGPARSE_HANDLER_ARGS)
{
    json_autospawn = args.front();
    std::cout << "autospawn config file: " << args.front() << std::endl;
}
cargparse::handlertype handler_output(CARGPARSE_HANDLER_ARGS)
{
    output = args.front();
    std::cout << "outputting to: " << args.front() << std::endl;
}
cargparse::handlertype handler_input(CARGPARSE_HANDLER_ARGS)
{
    json_all = args.front();
    std::cout << "input file: " << args.front() << std::endl;
}

void helpout()
{
    std::cout << "arkConfigGen is a small command line application that takes in json config files and outputs configuration code for ark Survival Evolved." << std::endl;
    std::cout << "The generated configuration code has to be put into \"game.ini\", it works on both servers and singleplayer and should work on all platforms" << std::endl;
    std::cout << "usage: " << std::endl;
    std::cout << "--help (same as -help & -h): outputs this help message" << std::endl << std::endl;
    std::cout << "--seperate (same as -seperate & -s): tells the program to use seperate config files for spawns and stacks" << std::endl << std::endl;
    std::cout << "--stacks (same as -stacks) specifies the stack config file (ie: stacks.json)" << std::endl << std::endl;
    std::cout << "--spawns (same as -spawns) specifies the spawn config file (ie: spawns.json)" << std::endl << std::endl;
    std::cout << "--autospawn (same as -autospawn) specifies the autospawn config file (ie: autospawn.json)" << std::endl << std::endl;
    std::cout << "--input (same as -input & -i) specifies the input file if --seperate is not specified" << std::endl << std::endl;
    std::cout << "--output (same as -output & -o) specifies the output file" << std::endl << std::endl;
    std::cout << "usage example: " << std::endl;
    std::cout << "\t./configGen --seperate --stacks stacks.json --spawns spawns.json -o game.ini" << std::endl;
    exit(0);
}

cargparse::handlertype handler_help(CARGPARSE_HANDLER_ARGS)
{
    helpout();
}

cargparse::handlertype unkownArgHandler(CARGPARSE_UNKOWNHANDLER_ARGS)
{std::cout << "unkown argument: \"" << arg << "\"" << std::endl << std::endl;helpout();}

int main(int argc, char** argv)
{
    cargparse::instance ins;

    ins.addParameter(false,0,"--seperate",&handler_seperate);
    ins.addParameter(false,0,"-seperate",&handler_seperate);
    ins.addParameter(false,0,"-s",&handler_seperate);

    ins.addParameter(false,0,"--help",&handler_help);
    ins.addParameter(false,0,"-help",&handler_help);
    ins.addParameter(false,0,"-h",&handler_help);

    ins.addParameter(false,1,"--output",&handler_output);
    ins.addParameter(false,1,"-output",&handler_output);
    ins.addParameter(false,1,"-o",&handler_output);

    ins.addParameter(false,1,"--input",&handler_input);
    ins.addParameter(false,1,"-input",&handler_input);
    ins.addParameter(false,1,"-i",&handler_input);

    ins.addParameter(false,1,"--stacks",&handler_stacks);
    ins.addParameter(false,1,"--spawns",&handler_spawns);
    ins.addParameter(false,1,"--autospawn",&handler_autospawn);

    ins.unknownHandler = &unkownArgHandler;

    ins.parse(argc,argv);

    if(seperateFiles)
    {
        if(json_spawns == "")
        {
            std::cerr << "Error: no spawn input file specified" << std::endl;
            exit(-1);
        }
        if(json_stacks == "")
        {
            std::cerr << "Error: no stack input file specified" << std::endl;
            exit(-1);
        }
        if(json_autospawn == "")
        {
            std::cerr << "Error: no autospawn input file specified" << std::endl;
            exit(-1);
        }
    }
    else
    {
        if(json_all == "")
        {
            std::cerr << "Error: no input file specified" << std::endl;
            exit(-1);
        }
    }
    if(output == "")
    {
        std::cerr << "Error: no output file specified" << std::endl;
        exit(-1);
    }

    std::string content;
    content += "[/script/shootergame.shootergamemode]\n";

    if(seperateFiles)
    {
        json stacks = jsonLoad(json_stacks);
        json spawns = jsonLoad(json_spawns);
        json autospawn = jsonLoad(json_autospawn);

        content += genSpawns(spawns);
        content += genAutospawn(autospawn);
        content += genStacks(stacks);
    }
    else
    {
        json all = jsonLoad(json_all);

        content += genSpawns(all["spawns"]);
        content += genStacks(all["stacks"]);
        content += genAutospawn(all["autospawn"]);
    }

    file outfile(output);

    outfile.clear();
    outfile << content;

    return 0;
}