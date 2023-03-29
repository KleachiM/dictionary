#ifndef DICTIONARY_FUNCTIONS_H
#define DICTIONARY_FUNCTIONS_H

#include <iostream>
#include <optional>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstring>
#include <regex>

const std::string DELIMITER = "@#";

enum class ProgramState
{
	SUCCESS,
	SUCCESS_MAP_CHANGED,
	BAD_ARGS_COUNT,
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

std::optional<std::string> ParseArgs(int argc, char* argv[], ProgramState& errorCode);
std::ifstream OpenFileForReading(const std::string & fileName);
MapElem MakePairFromStringLine(const std::string& line, ProgramState& errorCode);
std::map<mapKeyType, mapValueType> ReadDictionaryFromFile(const std::string& fileName, ProgramState& errorCode);
std::map<mapKeyType, mapValueType> GetDictFromFile(const std::optional<std::string>& fileName, ProgramState& errorCode);
void OpenSession(std::istream& cin, std::map<mapKeyType, mapValueType>& dict, ProgramState& errorCode);
std::optional<mapValueType> GetValueFromDictByKey(const std::map<mapKeyType, mapValueType>& dict, const std::string& key);
void InsertValueToDict(std::map<mapKeyType, mapValueType>& dict, mapKeyType key, const mapValueType& value);
bool NeedToSaveDict();
void SaveDictToFile(const std::optional<std::string>& fileName, const std::map<mapKeyType, mapValueType>& dict, ProgramState& errorCode);
std::optional<std::vector<std::string>> GetNewTranslateFromUserInput(std::istream& istream);
std::vector<std::string> ParseStringByDelimiter(const std::string& string, const std::string& delimiter);
#endif //DICTIONARY_FUNCTIONS_H
