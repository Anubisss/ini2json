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

#include "IniFile.h"

#include "Parse/ParsedFile.h"

#include <iostream>

IniFile::IniFile(const std::string fileName) : _fileName(fileName),
                                               _fileStream(), _parsedFile(NULL)
{
}

IniFile::~IniFile()
{
    delete _parsedFile;
}

bool IniFile::Open()
{
    _fileStream.open(_fileName.c_str(), std::ifstream::in);
    if (_fileStream.fail())
    {
        Close();
        return false;
    }
    return true;
}

void IniFile::Close()
{
    _fileStream.close();
}

void IniFile::Parse()
{
    std::cerr << "[INFO] Reading " << _fileName << " ..." << std::endl;

    _parsedFile = new ParsedFile(this);
    size_t lineNumberCounter = 1;
    // reads until EOF
    while (!_fileStream.eof())
    {
        char line[FILE_READ_BUFFER];
        // reads a line into the buffer
        _fileStream.getline(line, FILE_READ_BUFFER);

        // creates a parsed line
        ParsedLine* pl = new ParsedLine(_parsedFile,
                                        line,
                                        lineNumberCounter++);
        // and adds to the file
        _parsedFile->AddLine(pl);
    }
    // reading is ready so let's close the file
    Close();

    std::cerr << "[INFO] " << _parsedFile->GetParsedLinesCount()
              << " rows read." << std::endl;
    std::cerr << "[INFO] Parsing " << _fileName << " ..." << std::endl;

    _parsedFile->ParseLines();

    std::cerr << "[INFO] Parsed " << _parsedFile->GetValidParsedLines()
              << " valid ("
              << _parsedFile->GetParsedLinesCount() -
                 _parsedFile->GetValidParsedLines()
              << " invalid) rows." << std::endl;
};
