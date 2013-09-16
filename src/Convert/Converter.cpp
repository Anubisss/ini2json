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

#include "Converter.h"

#include <iostream>
#include <fstream>

#define FILE_EXTENSION_DELIMITER    "."
#define JSON_FILE_EXTENSION         "json"

void Converter::Convert(ParsedFile const* parsedFile)
{
    std::cerr << "[INFO] Converting to JSON..." << std::endl;

    // gets the original file name and converts to *.json
    std::string fileName = parsedFile->GetIniFile()->GetFileName();
    // finds where is the file extension delimiter, "."
    size_t fileExtensionDelimiter = 
            fileName.find_last_of(FILE_EXTENSION_DELIMITER, fileName.length());
    // delimiter found
    if (fileExtensionDelimiter != std::string::npos)
        fileName = fileName.substr(0, fileExtensionDelimiter);

    fileName += FILE_EXTENSION_DELIMITER;
    fileName += JSON_FILE_EXTENSION;

    std::ofstream fileStream;
    fileStream.open(fileName, std::ofstream::out);
    if (!fileStream.good())
    {
        std::cerr << "[ERROR] Can't write to file: " << fileName << std::endl;
        fileStream.close();
        return;
    }

    // the ugly part...

    std::cout << "{" << std::endl;
    fileStream << "{" << std::endl;

    bool firstSection = true;
    bool firstProperty = true;
    bool emptySection = true;
    IniFile::Section const* lastSection = NULL;
    for (ParsedFile::ParsedLinesList_ConstItr itr = 
                                          parsedFile->GetParsedLines().begin();
         itr != parsedFile->GetParsedLines().end();
         ++itr)
    {
        ParsedLine const* parsedLine = *itr;
        if (parsedLine->GetType() == IniFile::TYPE_PROPERTY)
        {
            if (!firstProperty &&
                lastSection == parsedLine->GetProperty()->Section)
            {
                std::cout << "," << std::endl;
                fileStream << "," << std::endl;
            }
            if (parsedLine->GetProperty()->Section)
            {
                std::cout << "    ";
                fileStream << "    ";
            }
            std::cout << "    \"" << parsedLine->GetProperty()->Key << "\":\""
                      << parsedLine->GetProperty()->Value  << "\"";
            fileStream << "    \"" << parsedLine->GetProperty()->Key << "\":\""
                       << parsedLine->GetProperty()->Value  << "\"";
            lastSection = parsedLine->GetProperty()->Section;

            firstProperty = false;
            emptySection = false;
        }
        else if (parsedLine->GetType() == IniFile::TYPE_SECTION)
        {
            if (!firstSection)
            {
                if (!emptySection)
                {
                    std::cout << std::endl;
                    fileStream << std::endl;
                }
                std::cout << "    }," << std::endl;
                fileStream << "    }," << std::endl;
            }
            if (firstSection && !firstProperty)
            {
                std::cout << "," << std::endl;
                fileStream << "," << std::endl;
            }
            std::cout << "    \"" << parsedLine->GetSection()->Name
                      << "\":" << std::endl;
            std::cout << "    {" << std::endl;
            fileStream << "    \"" << parsedLine->GetSection()->Name
                       << "\":" << std::endl;
            fileStream << "    {" << std::endl;

            firstSection = false;
            emptySection = true;
        }
    }
    if (!firstSection)
    {
        if (!emptySection)
        {
            std::cout << std::endl;
            fileStream << std::endl;
        }
        std::cout << "    }" << std::endl;
        fileStream << "    }" << std::endl;
    }
    else if (!firstSection || !firstProperty)
    {
        std::cout << std::endl;
        fileStream << std::endl;
    }

    std::cout << "}" << std::endl;
    fileStream << "}" << std::endl;

    fileStream.close();
}
