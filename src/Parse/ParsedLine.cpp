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

#include "ParsedLine.h"

#include "ParsedFile.h"

#include <iostream>

#define ASCII_SPACE 0x20
#define ASCII_TAB   0x09

ParsedLine::ParsedLine(ParsedFile* parsedFile,
                       const std::string line,
                       size_t lineNumber) : _parsedFile(parsedFile),
                                            _rawLine(line),
                                            _lineNumber(lineNumber),
                                            _lineType(IniFile::MAX_TYPE),
                                            _property(NULL), _section(NULL)
{
}

ParsedLine::~ParsedLine()
{
    // to delete a NULL pointer is safe, but that's more pretty :)
    if (GetType() == IniFile::TYPE_PROPERTY)
        delete _property;
}

void ParsedLine::Parse()
{
    // first of all: should ignore white spaces before any useful character
    // so let's find the first non white space character's position
    size_t firstPos = std::string::npos;
    for (size_t i = 0; i < _rawLine.length(); ++i)
        // space and tab characters
        if (_rawLine[i] != char(ASCII_SPACE) && _rawLine[i] != char(ASCII_TAB))
        {
            firstPos = i;
            break;
        }

    // stores that's this an invalid or valid parsed line
    bool invalid = false;

    // blank line
    // every lines which contains _only_ white spaces or nothing (just \r\n)
    if (firstPos == std::string::npos)
        _lineType = IniFile::TYPE_BLANK_LINE;
    // comment
    else if (IniFile::IsCommentIndicator(_rawLine[firstPos]))
        _lineType = IniFile::TYPE_COMMENT;
    // section
    else if (IniFile::IsSectionStartTag(_rawLine[firstPos]))
    {
        // first, tries to find invalid "things"

        size_t sectionEndPos =
            _rawLine.find_first_of(IniFile::GetSectionEndTag(), firstPos + 1);
        // no section close tag
        if (sectionEndPos == std::string::npos)
            invalid = true;
        // no section name
        else if (sectionEndPos == firstPos + 1)
            invalid = true;
        else
        {
            // white space section name
            bool haveSectionName = false;
            for (size_t i = firstPos + 1; i < sectionEndPos; ++i)
                if (_rawLine[i] != char(ASCII_SPACE)
                    && _rawLine[i] != char(ASCII_TAB))
                {
                    haveSectionName = true;
                    break;
                }
            if (!haveSectionName)
                invalid = true;
            else
            {
                // multiple section open tags 
                if (_rawLine.find_last_of(IniFile::GetSectionStartTag(),
                                          sectionEndPos) 
                                         != firstPos)
                    invalid = true;
                // multiple section end tags
                else if (_rawLine.find_last_of(IniFile::GetSectionEndTag(),
                                               sectionEndPos + 1)
                                              != sectionEndPos)
                    invalid = true;
                else
                {
                    // text after section end tag
                    bool textAfterEnd = false;
                    for (size_t i = sectionEndPos + 1;
                         i < _rawLine.length();
                         ++i)
                        if (_rawLine[i] != char(ASCII_SPACE) &&
                            _rawLine[i] != char(ASCII_TAB))
                        {
                            textAfterEnd = true;
                            break;
                        }

                    if (textAfterEnd)
                        invalid = true;
                    // should be a valid section
                    else
                    {
                        IniFile::Section* section = new IniFile::Section();
                        _section = section;
                        _parsedFile->AddSection(section);
                        _lineType = IniFile::TYPE_SECTION;

                        section->Name = _rawLine.substr(firstPos + 1,
                                                 sectionEndPos - firstPos - 1);
                    }
                }
            }
        }
    }
    // property
    else if (_rawLine.find(IniFile::GetPopertyDelimiter(),
                           firstPos + 1)
                          != std::string::npos)
    {
        _property = new IniFile::Property();
        _lineType = IniFile::TYPE_PROPERTY;

        size_t delimPos = _rawLine.find(IniFile::GetPopertyDelimiter(),
                                        firstPos + 1);
        // if there is a space: ignores it
        if (_rawLine[delimPos - 1] == ' ')
            _property->Key = _rawLine.substr(firstPos,  
                                             delimPos - firstPos - 1);
        else
            _property->Key = _rawLine.substr(firstPos, delimPos - firstPos);
        if (_rawLine.length() >= (delimPos + 2) &&
            _rawLine[delimPos + 1] == ' ')
            _property->Value = _rawLine.substr(delimPos + 2);
        else
            _property->Value = _rawLine.substr(delimPos + 1);
        _property->Section = _parsedFile->GetLastSection();
    }
    // invalid
    // should be just some texts
    else
        invalid = true;

    if (invalid)
    {
        // be kind :)
        std::cerr << "[WARNING] Invalid '" << _rawLine << "' in line "
                  << _lineNumber << ", ignored." << std::endl;
        _lineType = IniFile::TYPE_INVALID;
    }
}
