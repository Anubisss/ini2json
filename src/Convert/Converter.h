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

#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include "Parse/ParsedFile.h"

/**
 * This static class/function does all the work.
 */
class Converter
{
public:
    static void Convert(ParsedFile const* parsedFile);
};

#endif /* __CONVERTER_H__ */
