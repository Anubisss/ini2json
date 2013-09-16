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

#include "ParsedFile.h"

ParsedFile::ParsedFile(IniFile const* ini) : _iniFile(ini), _validParsedLines(0),
                                                     _sections()
{
}

ParsedFile::~ParsedFile()
{
    for (SectionList_ConstItr itr = _sections.begin(); itr != _sections.end(); ++itr)
        delete *itr;
    _sections.clear();

    for (ParsedLinesList_ConstItr itr = _parsedLines.begin(); itr != _parsedLines.end(); ++itr)
        delete *itr;
    _parsedLines.clear();
}

void ParsedFile::ParseLines()
{
    for (ParsedLinesList_ConstItr itr = _parsedLines.begin(); itr != _parsedLines.end(); ++itr)
    {
        ParsedLine* line = *itr;
        line->Parse();
        if (line->GetType() != IniFile::TYPE_INVALID) // just to know valid and invalid parses
            ++_validParsedLines;
    }
}

IniFile::Section const* ParsedFile::GetLastSection() const
{
    if (_sections.empty())
        return NULL;
    return _sections.back();
}
