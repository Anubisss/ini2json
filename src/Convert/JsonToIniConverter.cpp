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

#include "JsonToIniConverter.h"

#include <iostream>
#include <sstream>
#include <fstream>

class JsonParser
{
public:
    JsonParser(const std::string& in) : input(in), pos(0) {}
    JsonToIniConverter::JsonNode* Parse() {
        skipWhitespace();
        if (pos < input.size() && input[pos] == '{') {
            return ParseObject();
        }
        return NULL;
    }

private:
    std::string input;
    size_t pos;

    void skipWhitespace() {
        while (pos < input.size() && (input[pos] == ' ' || input[pos] == '\t' || input[pos] == '\n' || input[pos] == '\r'))
            pos++;
    }

    void consume(char c) {
        if (pos < input.size() && input[pos] == c) pos++;
    }

    std::string ParseString() {
        consume('"');
        std::string res;
        while (pos < input.size() && input[pos] != '"') {
            if (input[pos] == '\\') {
                pos++;
                if (pos < input.size()) {
                    if (input[pos] == '"') res += '"';
                    else if (input[pos] == '\\') res += '\\';
                    else if (input[pos] == 'n') res += '\n';
                    else if (input[pos] == 't') res += '\t';
                    else if (input[pos] == 'r') res += '\r';
                    else res += input[pos];
                }
            } else {
                res += input[pos];
            }
            pos++;
        }
        consume('"');
        return res;
    }

    JsonToIniConverter::JsonNode* ParseObject() {
        JsonToIniConverter::JsonNode* node = new JsonToIniConverter::JsonNode();
        skipWhitespace();
        consume('{');
        skipWhitespace();
        if (pos < input.size() && input[pos] == '}') {
            consume('}');
            return node;
        }
        while (true) {
            skipWhitespace();
            if (pos >= input.size()) break;
            std::string key = ParseString();
            skipWhitespace();
            consume(':');
            skipWhitespace();
            JsonToIniConverter::JsonNode* val = NULL;
            if (pos < input.size() && input[pos] == '"') {
                std::string v = ParseString();
                val = new JsonToIniConverter::JsonNode();
                val->value = v;
            } else if (pos < input.size() && input[pos] == '{') {
                val = ParseObject();
            }
            if (val) node->children[key] = val;
            skipWhitespace();
            if (pos < input.size() && input[pos] == ',') {
                consume(',');
            } else {
                break;
            }
        }
        skipWhitespace();
        consume('}');
        return node;
    }
};

void JsonToIniConverter::Convert(const std::string& fileName)
{
    std::cerr << "[INFO] Converting to INI..." << std::endl;

    std::ifstream in(fileName);
    if (!in.is_open())
    {
        std::cerr << "[ERROR] Can't open file: " << fileName << std::endl;
        return;
    }
    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();
    std::string content = buffer.str();

    JsonParser parser(content);
    JsonNode* root = parser.Parse();
    if (!root)
    {
        std::cerr << "[ERROR] Failed to parse JSON." << std::endl;
        return;
    }

    std::string outFileName = fileName;
    size_t dot = outFileName.rfind('.');
    if (dot != std::string::npos) outFileName.erase(dot);
    outFileName += ".ini";

    std::ofstream out(outFileName.c_str());
    if (!out.is_open())
    {
        std::cerr << "[ERROR] Can't write to file: " << outFileName << std::endl;
        Cleanup(root);
        return;
    }

    OutputIni(root, out, true);
    out.close();
    Cleanup(root);
}

void JsonToIniConverter::OutputIni(JsonNode* node, std::ofstream& out, bool isRoot)
{
    for (std::map<std::string, JsonNode*>::iterator itr = node->children.begin();
         itr != node->children.end(); ++itr)
    {
        if (itr->second->isObject())
        {
            out << "[" << itr->first << "]" << std::endl;
            OutputIni(itr->second, out, false);
            out << std::endl;
        }
        else
        {
            if (!isRoot)
                out << "    ";
            out << itr->first << "=" << itr->second->value << std::endl;
        }
    }
}

void JsonToIniConverter::Cleanup(JsonNode* node)
{
    if (!node) return;
    for (std::map<std::string, JsonNode*>::iterator itr = node->children.begin();
         itr != node->children.end(); ++itr)
    {
        Cleanup(itr->second);
    }
    delete node;
}
