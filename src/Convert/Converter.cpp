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
#include <sstream>

#define FILE_EXTENSION_DELIMITER    "."
#define JSON_FILE_EXTENSION         "json"

/*static */ std::ofstream  Converter::_fileStream;

/* static */ void Converter::Convert(ParsedFile const* parsedFile)
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

    _fileStream.open(fileName.c_str(), std::ofstream::out);
    if (!_fileStream.good())
    {
        std::cerr << "[ERROR] Can't write to file: " << fileName << std::endl;
        _fileStream.close();
        return;
    }

    // the ugly part...

    std::stringstream ss;
    ss << "{" << std::endl;
    Print(ss.str());

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
                ss.str(std::string()); // resets the stringstream
                ss << "," << std::endl;
                Print(ss.str());
            }
            if (parsedLine->GetProperty()->Section)
                Print("    ");

            ss.str(std::string());
            ss << "    \"" << parsedLine->GetProperty()->Key << "\":\""
               << parsedLine->GetProperty()->Value  << "\"";
            Print(ss.str());

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
                    ss.str(std::string());
                    ss << std::endl;
                    Print(ss.str());
                }
                ss.str(std::string());
                ss << "    }," << std::endl;
                Print(ss.str());
            }
            if (firstSection && !firstProperty)
            {
                ss.str(std::string());
                ss << "," << std::endl;
                Print(ss.str());
            }
            ss.str(std::string());
            ss << "    \"" << parsedLine->GetSection()->Name
                       << "\":" << std::endl;
            ss << "    {" << std::endl;
            Print(ss.str());

            firstSection = false;
            emptySection = true;
        }
    }
    if (!firstSection)
    {
        if (!emptySection)
        {
            ss.str(std::string());
            ss << std::endl;
            Print(ss.str());
        }
        ss.str(std::string());
        ss << "    }" << std::endl;
        Print(ss.str());
    }
    else if (!firstSection || !firstProperty)
    {
        ss.str(std::string());
        ss << std::endl;
        Print(ss.str());
    }

    ss.str(std::string());
    ss << "}" << std::endl;
    Print(ss.str());

    _fileStream.close();
}

/* static */ void Converter::Print(std::string text)
{
    // as simple as that :)
    std::cout << text;
    _fileStream << text;
}
