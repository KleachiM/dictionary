#ifndef DICTIONARY_FUNCTIONS_H
#define DICTIONARY_FUNCTIONS_H

#include <iostream>
#include <optional>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>

enum class ErrorCode
{
	SUCCESS,
	SUCCESS_MAP_CHANGED,
	FILE_OPENING_ERROR,
	FILE_WRITING_ERROR,
	READ_MAP_FROM_FILE_ERROR,
	READ_EMPTY_LINE_FROM_FILE
};

using mapKeyType = std::string;
using mapValueType = std::vector<std::string>;

struct MapElem
{
	mapKeyType key;
	mapValueType value;
};

std::optional<std::string> ParseArgs(int argc, char* argv[]);
std::ifstream OpenFileForReading(const std::string & fileName);
MapElem GetMapElementFromString(const std::string& line, ErrorCode& errorCode);
std::map<mapKeyType, mapValueType> ReadMapFromFile(const std::string& fileName, ErrorCode& errorCode);
std::map<mapKeyType, mapValueType> GetEngRusDictFromFile(const std::optional<std::string>& fileName, ErrorCode& errorCode);
void OpenSession(std::istream& cin, std::map<mapKeyType, mapValueType>& dict, ErrorCode& errorCode);
std::optional<mapValueType> GetValueFromDictByKey(const std::map<mapKeyType, mapValueType>& dict, const std::string& key);
void InsertValueToDict(std::map<mapKeyType, mapValueType>& dict, mapKeyType key, const mapValueType& value);
bool NeedToSaveDict();
void SaveDictToFile(const std::optional<std::string>& fileName, const std::map<mapKeyType, mapValueType>& dict, ErrorCode& errorCode);
#endif //DICTIONARY_FUNCTIONS_H
