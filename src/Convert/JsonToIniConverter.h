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

#ifndef __JSON_TO_INI_CONVERTER_H__
#define __JSON_TO_INI_CONVERTER_H__

#include <string>
#include <map>
#include <fstream>

class JsonToIniConverter
{
public:
    static void Convert(const std::string& fileName);
private:
    struct JsonNode {
        std::string value;
        std::map<std::string, JsonNode*> children;
        bool isObject() const { return !children.empty(); }
    };
    static void OutputIni(JsonNode* node, std::ofstream& out, bool isRoot);
    static void Cleanup(JsonNode* node);
};

#endif /* __JSON_TO_INI_CONVERTER_H__ */
