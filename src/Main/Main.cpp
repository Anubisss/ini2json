/*
 * This file is part of ini2json.
 *
 * ini2json is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * ini2json is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ini2json.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Ini/IniFile.h"
#include "Convert/Converter.h"
#include "Convert/JsonToIniConverter.h"

#include <iostream>
#include <string>

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "[WARNING] You have to give a file via input argument." << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    size_t dot = fileName.rfind('.');
    std::string ext = (dot != std::string::npos) ? fileName.substr(dot + 1) : "";

    if (ext == "json")
    {
        JsonToIniConverter::Convert(fileName);
    }
    else
    {
        IniFile* i = new IniFile(fileName);
        if (i->Open())
        {
            i->Parse();
            Converter::Convert(i->GetParsedFile());
            delete i;
        }
        else
        {
            std::cerr << "[ERROR] Can't open the file: " << fileName << std::endl;
            delete i;
            return 1;
        }
    }

    return 0;
}
