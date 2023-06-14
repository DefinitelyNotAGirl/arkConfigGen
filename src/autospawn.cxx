/*
 * Created Date: Wednesday June 14th 2023
 * Author: DefinitelyNotAGirl@github
 * -----
 * Last Modified: Wednesday June 14th 2023 4:41:04 pm
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

#include <json.h>
#include <string>

std::string genAutospawn(json& j)
{
    uint64_t entryCounter = 0;
    std::string res;

    for(json& region : j["regions"])
    {
        if(!j["zones"].hasEntry(region.data))
                error("error: unknown region \""+region.data+"\"");

        uint64_t spawncounter = 0;

        for(json& subregion : j["zones"][region.data])
        {
            res += "ConfigOverrideNPCSpawnEntriesContainer=(NPCSpawnEntriesContainerClassString=\""+subregion.data+"\",NPCSpawnEntries=(";
            for(json& creatureSet : region)
            {
                if(!j["creatureSets"].hasEntry(creatureSet.data))
                    goto notASet;
                for(json& creature : j["creatureSets"][creatureSet.data])
                {
                    res += "(AnEntryName=\""+region.data+"_spawner_"+std::to_string(spawncounter++)+"\",EntryWeight="+std::string(j["rarities"][std::string(creatureSet)]["weight"])+",NPCsToSpawnStrings=("+std::string(creature.data)+")),";
                    std::cout << "(entry "+std::to_string(entryCounter++)+" ) added \""+std::string(creature.data)+"\" to "+std::string(subregion.data)+"..." << std::endl;
                }
                continue;
                notASet: ;
                res += "(AnEntryName=\""+region.data+"_spawner_"+std::to_string(spawncounter++)+"\",EntryWeight="+std::string(j["rarities"][std::string(creatureSet)]["weight"])+",NPCsToSpawnStrings=("+std::string(creatureSet.data)+")),";
                std::cout << "(entry "+std::to_string(entryCounter++)+" ) added \""+std::string(creatureSet.data)+"\" to "+std::string(subregion.data)+"..." << std::endl;
            }
            res.pop_back();
            res += "),NPCSpawnLimits=(";
            for(json& creatureSet : region)
            {
                if(!j["creatureSets"].hasEntry(creatureSet.data))
                    goto notASet2;
                for(json& creature : j["creatureSets"][creatureSet.data])
                    res += "(NPCClassString=\""+std::string(creature)+"\",MaxPercentageOfDesiredNumToAllow="+std::string(j["rarities"][std::string(creatureSet)]["max"])+"),";
                continue;
                notASet2: ;
                res += "(NPCClassString=\""+std::string(creatureSet.data)+"\",MaxPercentageOfDesiredNumToAllow="+std::string(j["rarities"][std::string(creatureSet)]["max"])+"),";
            }
            res.pop_back();
            res+="))\n"; 
        }
    }

    return res;
}