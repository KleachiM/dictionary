#include "Functions.h"

std::optional<std::string> ParseArgs(int argc, char** argv, ErrorCode& errorCode)
{
	/*if (argc != 2)
		return std::nullopt;
	return argv[1];*/
	if (argc > 2)
	{
		errorCode = ErrorCode::BAD_ARGS_COUNT;
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
	return move(strm);//TODO: почему что-то с std, а что-то без
}

MapElem GetMapElementFromString(const std::string& line, ErrorCode& errorCode)//TODO: добавить конкретики в именование
{
	errorCode = ErrorCode::SUCCESS;
	MapElem mapElem;
	if (line.empty())
	{
		errorCode = ErrorCode::READ_EMPTY_LINE_FROM_FILE;
		return mapElem;
	}
	std::vector<std::string> vecFromString;
	std::stringstream ss(line);
	copy(std::istream_iterator<std::string>(ss), std::istream_iterator<std::string>(), inserter(vecFromString, vecFromString.end()));
	if (vecFromString.size() < 2)
	{
		errorCode = ErrorCode::READ_MAP_FROM_FILE_ERROR;
		return mapElem;
	}
	for (std::vector<std::string>::iterator iter = vecFromString.begin(); iter != vecFromString.end(); ++iter)
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

std::map<mapKeyType, mapValueType> ReadMapFromFile(const std::string& fileName, ErrorCode& errorCode)//TODO: мб добавить, что читаем именно dictionary
{
	errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> outMap;//TODO: мб это dictionaryMap
	if (auto strm = OpenFileForReading(fileName))//TODO: уменьшить вложенность
	{
		std::string line;
		MapElem mapElem;
		while (std::getline(strm, line))
		{
			mapElem = GetMapElementFromString(line, errorCode);
			if (errorCode == ErrorCode::SUCCESS)
			{
				outMap.insert(std::make_pair(mapElem.key, mapElem.value));
			}
		}
	}
	else
	{
		errorCode = ErrorCode::FILE_OPENING_ERROR;
	}
	return outMap;
}

std::map<mapKeyType, mapValueType> GetEngRusDictFromFile(const std::optional<std::string>& fileName, ErrorCode& errorCode)//TOOD: фи-я названа неправильно, так как с испанскими словами тоже будет работать
{
	std::map<mapKeyType, mapValueType> outMap;
	if (!fileName)
	{
		return outMap;
	}
	outMap = ReadMapFromFile(*fileName, errorCode);
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

void OpenSession(std::istream& cin, std::map<mapKeyType, mapValueType>& dict, ErrorCode& errorCode)
{
	std::cout << "Введите слово(фразу) на английском языке или \"...\" для завершения" << std::endl;
	std::string userInpStr;//TODO: нужно ограничить хону видимости переменной
	while (true)
	{
		userInpStr = "";
		std::getline(cin, userInpStr);
		if (userInpStr == "...") break;
		if (!userInpStr.empty())//TODO: инвертировать условние и если empty, то делать continue, это уменьшит уровень вложенности
		{
			// поиск строки в словаре
			auto keyValue = GetValueFromDictByKey(dict, userInpStr);// TODO: переменную переименовать на "перевод"
			if (keyValue)
			{
				// вывести значение по ключу
				PrintValueFromDictToCout(*keyValue);
				std::cout << std::endl;
			}
			else
			{
				std::cout << "Неизвестное слово \"" << userInpStr << "\". Введите перевод или пустую строку для отказа." << std::endl;
				std::vector<std::string> vecUserAnsw;
				std::string strUserAnsw;
				std::getline(std::cin, strUserAnsw);
				if (!strUserAnsw.empty())//TODO: если строка пустая, уже можно вывести сообщение о проигнорированном слове и выйти из цикла
				{
					vecUserAnsw.push_back(strUserAnsw);
				}
				//TODO: уменьшить вложенность за счет continue/break и декомпозиии функции на более мелкие для читаемости кода
				if (vecUserAnsw.empty())
				{
					std::cout << "Слово \"" << userInpStr << "\" проигнорировано." << std::endl;
				}
				else
				{
					// добавить неизвестное слово в словарь, изменить признак изменения словаря
					InsertValueToDict(dict, userInpStr, vecUserAnsw);
					errorCode = ErrorCode::SUCCESS_MAP_CHANGED;//TODO: почему в errorCode мы кладем успешное состояние программы?
					std::cout << "Слово \"" << userInpStr << "\" сохранено в словаре как \"";
					// вывести значение по ключу
					PrintValueFromDictToCout(vecUserAnsw);
					std::cout << "\"" << std::endl;
				}
			}
		}
	}
}

bool NeedToSaveDict()
{
	std::string userAnswer;//TODO: нужно ограничить хону видимости переменной
	while (true)
	{
		std::cout << "Словарь был изменен. Вы хотите сохранить изменения? (Y/N)" << std::endl;
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

void SaveDictToFile(const std::optional<std::string>& fileName, const std::map<mapKeyType, mapValueType>& dict, ErrorCode& errorCode)
{
	const std::string DEFAULT_FILENAME = "dict.txt"; // TODO: разобраться с сохранением в словарь, чтобы работало
	std::string fileToOpen = fileName ? *fileName : DEFAULT_FILENAME;
	std::ofstream outputFile(fileToOpen);
	if (outputFile.is_open())
	{
		for (const auto& [key, value]: dict)
		{
			outputFile << key;
			std::copy(value.begin(), value.end(), std::ostream_iterator<std::string>(outputFile, " "));
			outputFile << " ";
			outputFile << std::endl;
		}
		outputFile.flush();

		if (!outputFile)
		{
			errorCode = ErrorCode::FILE_WRITING_ERROR;
		}
	}
	else
	{
		errorCode = ErrorCode::FILE_WRITING_ERROR;
	}
}