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

#ifndef __PARSED_LINE_H__
#define __PARSED_LINE_H__

#include "Ini/IniFile.h"

#include <string>

/**
 * This class represents each of the parsed lines.
 */
class ParsedLine
{
public:
    ParsedLine(ParsedFile* parsedFile,
               const std::string line,
               size_t lineNumber);
    ~ParsedLine();

    /**
     * Returns the raw line.
     */
    std::string GetRaw() const { return _rawLine; }

    /**
     * Gives the type of the line.
     * @see IniFile::IniLineType
     */
    IniFile::IniLineType GetType() const { return _lineType; }

    /**
     * Parses this line.
     * This does the real job...
     */
    void Parse();

    /**
     * Returns the property.
     * @return Returns NULL if this is not a property type line.
     */
    IniFile::Property* GetProperty() const { return _property; }
    /**
     * Returns with the section.
     * @return Returns with NULL if there is no section for this line.
     * @note Just property type line can have non NULL value.
     */
    IniFile::Section* GetSection() const { return _section; }

private:
    ParsedFile* _parsedFile;

    std::string _rawLine;
    /**
     * Should know the line number for debug purpose.
     */
    size_t _lineNumber;

    IniFile::IniLineType _lineType;

    IniFile::Property* _property;
    IniFile::Section* _section;
};

#endif /* __PARSED_LINE_H__ */
