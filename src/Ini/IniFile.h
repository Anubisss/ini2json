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

#ifndef __INI_FILE_H__
#define __INI_FILE_H__

#include <string>
#include <fstream>

class ParsedFile;

#define FILE_READ_BUFFER 256

/**
 * That class represents the ini file.
 * Has functions to open/close/parse the file.
 */
class IniFile
{
public:
    /**
     * These are all the possible types of each lines of the ini file.
     */
    enum IniLineType
    {
        TYPE_SECTION,       // [section]
        TYPE_PROPERTY,      // key=value
        TYPE_COMMENT,       // ; comment
        TYPE_BLANK_LINE,    //
        TYPE_INVALID,       // if something goes wrong

        MAX_TYPE
    };

    /**
     * This is a section in an ini file.
     * Syntax is: [Section Name]
     */
    struct Section
    {
        std::string Name;

        Section::Section()
        {
            Name = "";
        }
    };

    /**
     * This is an ini property.
     * Syntax is: Key=Value
     */
    struct Property
    {
        std::string Key;
        std::string Value;

        Section const* Section;

        Property::Property()
        {
            Key = "";
            Value = "";
            Section = NULL;
        }
    };

    /**
     * Comment indicators.
     * ; Comment
     * # Comment
     */
    static const bool IsCommentIndicator(const char c)
    {
        switch (c)
        {
            case ';':
            case '#':
                return true;
            default:
                return false;
        }
    }

    /**
     * Opening and closing tags for sections and property delimiter.
     */
    static const char GetSectionStartTag() { return '['; }
    static const char GetSectionEndTag() { return ']'; }
    static const char GetPopertyDelimiter() { return '='; }

    static const bool IsSectionStartTag(const char c)
    {
        return c == GetSectionStartTag();
    }
    static const bool IsSectionEndTag(const char c)
    {
        return c == GetSectionEndTag();
    }

public:
    IniFile(const std::string fileName);
    ~IniFile();

    /**
     * This function tries to open the specific file.
     * Returns true if opening is successful or false if failed.
     */
    bool Open();

    /**
     * Parses then stores the parsed data.
     */
    void Parse();

    std::string GetFileName() const { return _fileName; }

    ParsedFile* GetParsedFile() const { return _parsedFile; }

private:
    /**
     * Simply closes the file stream
     */
    void Close();

private:
    std::string _fileName;
    std::ifstream _fileStream;

    /**
     * This stores itself the parsed file.
     */
    ParsedFile* _parsedFile;
};

#endif /* __INI_FILE_H__ */
