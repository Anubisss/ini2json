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

#ifndef __PARSED_FILE_H__
#define __PARSED_FILE_H__

#include "ParsedLine.h"

#include <list>

/**
 * That class stores the parsed file.
 */
class ParsedFile
{
public:
    /**
     * List type for storing the parsed lines.
     */
    typedef std::list<ParsedLine*> ParsedLinesList;
    typedef ParsedLinesList::const_iterator ParsedLinesList_ConstItr;

public:
    ParsedFile(IniFile const* ini);
    ~ParsedFile();

    /**
     * Adds a line.
     */
    void AddLine(ParsedLine* line)
    {
        _parsedLines.push_back(line);
    }

    /**
     * Parses all the lines from the IniFile.
     */
    void ParseLines();

    size_t GetParsedLinesCount() const { return _parsedLines.size(); }
    size_t GetValidParsedLines() const { return _validParsedLines; }

    /**
     * Adds a section.
     */
    void AddSection(IniFile::Section* section)
    {
        _sections.push_back(section);
    }

    /**
     * Gives the section which parsed last.
     * Returns NULL if there is no sections.
     */
    IniFile::Section const* GetLastSection() const;

    /**
     * Returns the parsed lines list.
     */
    ParsedLinesList const& GetParsedLines() const { return _parsedLines; }

    IniFile const* GetIniFile() const { return _iniFile; }

private:
    /**
     * Type for storing the sections which are parsed.
     */
    typedef std::list<IniFile::Section*> SectionList;
    typedef std::list<IniFile::Section*>::const_iterator SectionList_ConstItr;

    IniFile const* _iniFile;

    ParsedLinesList _parsedLines;

    /**
     * Number of successfully parsed lines.
     */
    size_t _validParsedLines;

    SectionList _sections;
};

#endif /* __PARSED_FILE_H__ */
