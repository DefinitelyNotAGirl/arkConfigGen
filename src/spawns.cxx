/*
 * Created Date: Wednesday June 7th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Wednesday June 7th 2023 8:40:26 pm
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

#include <string>
#include <list>
#include <json.h>

std::string genSpawns(json& j)
{
    std::string res;

    for(json& region : j)
    {
        if(!region.hasEntry("type"))
        {
            regtype_add: ;
            res += "ConfigAddNPCSpawnEntriesContainer=(NPCSpawnEntriesContainerClassString=\""+region.data+"\",NPCSpawnEntries=(";
        }
        else
        {
            if(std::string(region["type"]) == "add")
                goto regtype_add;
            else if(std::string(region["type"]) == "subtract")
                res += "ConfigSubtractNPCSpawnEntriesContainer=(NPCSpawnEntriesContainerClassString=\""+region.data+"\",NPCSpawnEntries=(";
            else if(std::string(region["type"]) == "override")
                res += "ConfigOverrideNPCSpawnEntriesContainer=(NPCSpawnEntriesContainerClassString=\""+region.data+"\",NPCSpawnEntries=(";
            else
                error("invalid region config type id \""+std::string(region["type"])+"\"");
        }
        for(json& spawner : region)
        {
            if(spawner.data == "type")
                continue;
            res += "(AnEntryName=\""+spawner.data+"\",EntryWeight="+std::string(spawner["weight"])+",NPCsToSpawnStrings=(";
            std::cout << "adding " << spawner.data << " to " << region.data << "..." << std::endl;
            for(json& creature : spawner["creatures"])
                res+="\""+std::string(creature)+"\",";
            res.pop_back();
            res += ")),";
        }
        res.pop_back();
        res += "),NPCSpawnLimits=(";
        std::list<std::string> defined;
        for(json& spawner : region)
        {
            if(spawner.data == "type")
                continue;
            for(json& creature : spawner["creatures"])
            {
                for(std::string c : defined)
                    if(c == std::string(creature))
                        goto dontLimitCreature;
                defined.push_back(creature);
                res += "(NPCClassString=\""+std::string(creature)+"\",MaxPercentageOfDesiredNumToAllow="+std::string(spawner["weight"])+"),";
                dontLimitCreature: ;
            }
        }
        res.pop_back();
        res+="))\n"; 
    }
    return res;
}