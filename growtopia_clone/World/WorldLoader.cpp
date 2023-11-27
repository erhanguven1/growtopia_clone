//
// Created by erhan güven on 27.11.2023.
//

#include "WorldLoader.h"
#include <tinyxml2/tinyxml2.h>
#include <fstream>

namespace Game
{
    FILE* WorldLoader::fp;

    void WorldLoader::loadWorld(const char* worldName, World* worldRef)
    {

    }

    void WorldLoader::saveWorld(const char *worldName, const World *worldRef)
    {
        using namespace tinyxml2;

        const std::string fullPath = std::string("/Users/erhanguven/CLionProjects/growtopia_clone/growtopia_clone/Resources/Worlds/")+worldName+".xml";

        fp = fopen(fullPath.c_str(), "w+");

        auto* doc = new XMLDocument();
        XMLElement* mapData =  doc->NewElement("WorldData");

        XMLNode* gridElement = doc->InsertEndChild(doc->NewElement("Grid"));

        for (int i = 0; i < worldRef->getGrid().size(); ++i)
        {
            for (int j = 0; j < worldRef->getGrid()[i].size(); ++j)
            {
                auto tile = worldRef->getGrid()[i][j];
                XMLElement* tileTypeDataElement;
                tileTypeDataElement = doc->NewElement("TileType");

                tileTypeDataElement->SetAttribute("Type", (int)tile->getBlockType());

                tileTypeDataElement->InsertNewChildElement("X")->SetText(i);
                tileTypeDataElement->InsertNewChildElement("Y")->SetText(j);

                gridElement->InsertEndChild(tileTypeDataElement);
            }
        }

        mapData->InsertEndChild(gridElement);
        doc->InsertEndChild(mapData);

        XMLPrinter printer(fp);
        doc->Print(&printer);

        fclose(fp);
    }
} // Game