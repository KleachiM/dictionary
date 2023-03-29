#include "Functions.h"


std::optional<std::string> ParseArgs(int argc, char** argv, ProgramState& errorCode)
{
	if (argc > 2)
	{
		errorCode = ProgramState::BAD_ARGS_COUNT;
		return std::nullopt;
	}
	else if (argc == 2)
	{
		return argv[1];
	}
	return std::nullopt;
}

std::ifstream OpenFileForReading(const std::string & fileName)
{
	std::ifstream strm(fileName);
	return std::move(strm);//TODO: ok почему что-то с std, а что-то без
}

std::vector<std::string> ParseStringByDelimiter(const std::string& string, const std::string& delimiter)
{
	std::vector<std::string> parsedString;
	std::regex pattern(delimiter);
	std::copy( std::sregex_token_iterator(string.begin(), string.end(), pattern, -1),
		std::sregex_token_iterator(),
		std::back_inserter(parsedString));
	return parsedString;
}

MapElem MakePairFromStringLine(const std::string& line, ProgramState& errorCode)//TODO: ok добавить конкретики в именование (было GetMapElemFromString)
{
	errorCode = ProgramState::SUCCESS;
	MapElem mapElem;
	if (line.empty())
	{
		errorCode = ProgramState::READ_EMPTY_LINE_FROM_FILE;
		return mapElem;
	}
	std::vector<std::string> vecFromString = ParseStringByDelimiter(line, DELIMITER);
	if (vecFromString.size() < 2)
	{
		errorCode = ProgramState::READ_MAP_FROM_FILE_ERROR;
		return mapElem;
	}
	for (auto iter = vecFromString.begin(); iter != vecFromString.end(); ++iter)
	{
		// read key
		if (iter == vecFromString.begin())
		{
			mapElem.key = *iter;
			continue;
		}
		// read values
		mapElem.value.push_back(*iter);
	}
	return mapElem;
}

std::map<mapKeyType, mapValueType> ReadDictionaryFromFile(const std::string& fileName, ProgramState& errorCode)//TODO: ok мб добавить, что читаем именно dictionary
{
	errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> dictionaryMap;//TODO: ok мб это dictionaryMap
	//TODO: ok уменьшить вложенность
	auto strm = OpenFileForReading(fileName);
	if (!strm)
	{
		errorCode = ProgramState::FILE_OPENING_ERROR;
		return dictionaryMap;
	}
	std::string line;
	while (std::getline(strm, line))
	{
		MapElem mapElem = MakePairFromStringLine(line, errorCode);
		if (errorCode == ProgramState::SUCCESS)
		{
			dictionaryMap.insert(std::make_pair(mapElem.key, mapElem.value));
		}
	}
	return dictionaryMap;
}

std::map<mapKeyType, mapValueType> GetDictFromFile(const std::optional<std::string>& fileName, ProgramState& errorCode)//TOOD: фи-я названа неправильно, так как с испанскими словами тоже будет работать
{
	std::map<mapKeyType, mapValueType> outMap;
	if (!fileName)
	{
		return outMap;
	}
	outMap = ReadDictionaryFromFile(*fileName, errorCode);
	return outMap;
}

std::optional<mapValueType> GetValueFromDictByKey(const std::map<mapKeyType, mapValueType>& dict, const std::string& key)
{
	std::string token = key;
	std::transform(token.begin(), token.end(), token.begin(), tolower);
	if (auto search = dict.find(token); search != dict.end())
	{
		return search->second;
	}
	return std::nullopt;
}

void PrintValueFromDictToCout(const mapValueType& value)
{
	std::copy(value.begin(), (value.begin() + value.size() - 1), std::ostream_iterator<std::string>(std::cout, ", "));
	std::copy((value.begin() + value.size() - 1), (value.begin() + value.size()), std::ostream_iterator<std::string>(std::cout, ""));
}

void InsertValueToDict(std::map<mapKeyType, mapValueType>& dict, mapKeyType key, const mapValueType& value)
{
	std::transform(key.begin(), key.end(), key.begin(), tolower);
	dict.insert(std::make_pair(key, value));
}

void OpenSession(std::istream& cin, std::map<mapKeyType, mapValueType>& dict, ProgramState& errorCode)
{
	std::cout << "Введите слово(фразу) на английском языке или \"...\" для завершения" << std::endl;
	while (true)
	{
		std::string userInpStr = "";//TODO: ok нужно ограничить хону видимости переменной
		std::getline(cin, userInpStr);
		if (userInpStr == "...")
		{
			break;
		}
		if (userInpStr.empty()) //TODO: ok инвертировать условие чтобы уменьшить вложенность
		{
			continue;
		}
		//TODO: ok декомпозировать на маленькие функции
		auto translate = GetValueFromDictByKey(dict, userInpStr);// TODO: ok переменную переименовать на "перевод"
		if (!translate)
		{
			std::cout << "Неизвестное слово \"" << userInpStr << "\". Введите перевод или пустую строку для отказа." << std::endl;
			auto newTranslate = GetNewTranslateFromUserInput(std::cin);
			if (!newTranslate)
			{
				std::cout << "Слово \"" << userInpStr << "\" проигнорировано." << std::endl;
				continue;
			}
			InsertValueToDict(dict, userInpStr, *newTranslate);
			errorCode = ProgramState::SUCCESS_MAP_CHANGED;//TODO: ok почему в errorCode мы кладем успешное состояние программы?
			std::cout << "Слово \"" << userInpStr << "\" сохранено в словаре как \"";
			PrintValueFromDictToCout(*newTranslate);
			std::cout << "\"" << std::endl;
			continue;
		}
		PrintValueFromDictToCout(*translate);
		std::cout << std::endl;
	}
}

std::optional<std::vector<std::string>> GetNewTranslateFromUserInput(std::istream& istream)
{
	std::string userAnsw;
	std::getline(istream, userAnsw);
	if (userAnsw.empty())
	{
		return std::nullopt;
	}
	std::vector<std::string> vecUserAnsw;
	std::stringstream ss(userAnsw);
	std::copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(),
	    inserter(vecUserAnsw, vecUserAnsw.end()));
	return vecUserAnsw;
}

bool NeedToSaveDict()
{
	while (true)
	{
		std::cout << "Словарь был изменен. Вы хотите сохранить изменения? (Y/N)" << std::endl;
		std::string userAnswer;//TODO: ok нужно ограничить хону видимости переменной
		getline(std::cin, userAnswer);
		if ((userAnswer == "Y") || (userAnswer == "y"))
		{
			return true;
		}
		else if ((userAnswer == "N") || (userAnswer == "n"))
		{
			return false;
		}
	}
}

void SaveDictToFile(const std::optional<std::string>& fileName, const std::map<mapKeyType, mapValueType>& dict, ProgramState& errorCode)
{
	const std::string DEFAULT_FILENAME = "dict.txt"; // TODO: ok разобраться с сохранением в словарь, чтобы работало
	std::string fileToOpen = fileName ? *fileName : DEFAULT_FILENAME;
	std::ofstream outputFile(fileToOpen);
	if (outputFile.is_open())
	{
		for (const auto& [key, value]: dict)
		{
			outputFile << key;
			outputFile << DELIMITER;
			std::copy(value.begin(), value.end(),
				std::ostream_iterator<std::string>(outputFile, DELIMITER.c_str()));
			outputFile << std::endl;
		}
		outputFile.flush();

		if (!outputFile)
		{
			errorCode = ProgramState::FILE_WRITING_ERROR;
		}
	}
	else
	{
		errorCode = ProgramState::FILE_WRITING_ERROR;
	}
}