#include <catch2/catch_test_macros.hpp>
#include "../Functions.h"

TEST_CASE("parse args")
{
	std::string param1 = "progName";
	std::string param2 = "fileName";
	std::string param3 = "bad param";
	ProgramState errorCode = ProgramState::SUCCESS;
	char* tstArgv[] = {param1.data(), param2.data(), NULL};
	char* badArgv[] = {param1.data(), param2.data(), param3.data(), NULL};
	REQUIRE(ParseArgs(2, tstArgv, errorCode) == "fileName");
	REQUIRE(ParseArgs(1, tstArgv, errorCode) == std::nullopt);
	REQUIRE(ParseArgs(3, badArgv, errorCode) == std::nullopt);
	REQUIRE(errorCode == ProgramState::BAD_ARGS_COUNT);
}

// MakePairFromStringLine
TEST_CASE("MakePairFromStringLine, good scenario (one val)")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	MapElem expectedMapElem = {"key", {"value"}};
	MapElem retMapElem = MakePairFromStringLine("key@#value", errorCode);
	REQUIRE(expectedMapElem.key == retMapElem.key);
	REQUIRE(expectedMapElem.value == retMapElem.value);
}

TEST_CASE("MakePairFromStringLine, good scenario (many val)")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	MapElem expectedMapElem = {"key", {"value1", "value2"}};
	MapElem retMapElem = MakePairFromStringLine("key@#value1@#value2", errorCode);
	REQUIRE(expectedMapElem.key == retMapElem.key);
	REQUIRE(expectedMapElem.value == retMapElem.value);
}

TEST_CASE("MakePairFromStringLine, empty line")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	MapElem expectedMapElem = {"key", {"value"}};
	MapElem retMapElem = MakePairFromStringLine("", errorCode);
	REQUIRE(errorCode == ProgramState::READ_EMPTY_LINE_FROM_FILE);
}

TEST_CASE("MakePairFromStringLine, only key")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	MapElem expectedMapElem = {"key", {"value"}};
	MapElem retMapElem = MakePairFromStringLine("key", errorCode);
	REQUIRE(errorCode == ProgramState::READ_MAP_FROM_FILE_ERROR);
}

// ReadDictionaryFromFile
TEST_CASE("ReadDictionaryFromFile, not existing file")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(ReadDictionaryFromFile("./files/notExist.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::FILE_OPENING_ERROR);
}

TEST_CASE("ReadDictionaryFromFile, empty file")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(ReadDictionaryFromFile("../../tests/files/empty.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

TEST_CASE("ReadDictionaryFromFile, file with empty line")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(ReadDictionaryFromFile("../../tests/files/fileWithEmptyLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

TEST_CASE("ReadDictionaryFromFile, file with not valid line")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(ReadDictionaryFromFile("../../tests/files/fileWithNotValidLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

TEST_CASE("ReadDictionaryFromFile, valid file")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};
	REQUIRE(ReadDictionaryFromFile("../../tests/files/validFile.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

// GetDictFromFile
TEST_CASE("GetDictFromFile, not specified file")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(GetDictFromFile(std::nullopt, errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

TEST_CASE("GetDictFromFile, not existing file")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(GetDictFromFile("notExist.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::FILE_OPENING_ERROR);
}

TEST_CASE("GetDictFromFile, empty file")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(GetDictFromFile("../../tests/files/empty.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

TEST_CASE("GetDictFromFile, file with empty line")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(GetDictFromFile("../../tests/files/fileWithEmptyLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

TEST_CASE("GetDictFromFile, file with not valid line")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(GetDictFromFile("../../tests/files/fileWithNotValidLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

TEST_CASE("GetDictFromFile, valid file")
{
	ProgramState errorCode = ProgramState::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};
	REQUIRE(GetDictFromFile("../../tests/files/validFile.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ProgramState::SUCCESS);
}

// GetValueFromDictByKey
TEST_CASE("GetValueFromDictByKey")
{
	std::map<mapKeyType, mapValueType> tstMap =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};
	std::string notExistKey = "hello";
	REQUIRE(GetValueFromDictByKey(tstMap, notExistKey) == std::nullopt);
	std::string existKey = "dog";
	REQUIRE(*GetValueFromDictByKey(tstMap, existKey) == (std::vector<std::string>){"собака"});
	REQUIRE(*GetValueFromDictByKey(tstMap, "cat") == (std::vector<std::string>){"кошка", "кот"});
}

TEST_CASE("GetValueFromDictByKey (key in uppercase)")
{
	std::map<mapKeyType, mapValueType> tstMap =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};
	std::string notExistKey = "Hello";
	REQUIRE(GetValueFromDictByKey(tstMap, notExistKey) == std::nullopt);
	std::string existKey = "Dog";
	REQUIRE(*GetValueFromDictByKey(tstMap, existKey) == (std::vector<std::string>){"собака"});
	REQUIRE(*GetValueFromDictByKey(tstMap, "Cat") == (std::vector<std::string>){"кошка", "кот"});
}

// InsertValueToDict
TEST_CASE("InsertValueToDict (single key value")
{
	std::map<mapKeyType, mapValueType> mapBeforeInsert =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};

	mapKeyType insertedKey = "hello";
	mapValueType insertedValue = (std::vector<std::string>){"привет"};

	std::map<mapKeyType, mapValueType> mapAfterInsert =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}},
			{"hello", {"привет"}}
		};

	InsertValueToDict(mapBeforeInsert, insertedKey, insertedValue);
	REQUIRE(mapBeforeInsert == mapAfterInsert);
}

TEST_CASE("InsertValueToDict (multiple key value")
{
	std::map<mapKeyType, mapValueType> mapBeforeInsert =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};

	mapKeyType insertedKey = "hello";
	mapValueType insertedValue = (std::vector<std::string>){"привет", "здравствуйте"};

	std::map<mapKeyType, mapValueType> mapAfterInsert =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}},
			{"hello", {"привет", "здравствуйте"}}
		};

	InsertValueToDict(mapBeforeInsert, insertedKey, insertedValue);
	REQUIRE(mapBeforeInsert == mapAfterInsert);
}

TEST_CASE("InsertValueToDict (value with space)")
{
	std::map<mapKeyType, mapValueType> mapBeforeInsert =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};

	mapKeyType insertedKey = "The Red Square";
	mapValueType insertedValue = (std::vector<std::string>){"Красная площадь"};

	std::map<mapKeyType, mapValueType> mapAfterInsert =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}},
			{"the red square", {"Красная площадь"}}
		};

	InsertValueToDict(mapBeforeInsert, insertedKey, insertedValue);
	REQUIRE(mapBeforeInsert == mapAfterInsert);
}

TEST_CASE("parse string with delimiter")
{
	std::string strWithDelim = "cat@#кошка";
	std::vector<std::string> parsedString = {"cat", "кошка"};
	std::vector<std::string> tstV = ParseStringByDelimiter(strWithDelim, DELIMITER);
	REQUIRE(tstV == parsedString);
}

TEST_CASE("parse phrase with delimiter")
{
	std::string strWithDelim = "the red square@#красная площадь@#площадь красная";
	std::vector<std::string> parsedString = {"the red square", "красная площадь", "площадь красная"};
	std::vector<std::string> tstV = ParseStringByDelimiter(strWithDelim, DELIMITER);
	REQUIRE(tstV == parsedString);
}

TEST_CASE("parse not valid string")
{
	std::string badString = "@#";
	std::vector<std::string> tstV = ParseStringByDelimiter(badString, DELIMITER);
	REQUIRE(tstV.size() < 2);
}

SCENARIO("phrase can be written in file and read")
{
	GIVEN("phrase and its translation")
	{
		std::string phrase = "the red square";
		std::vector<std::string> translate = { "красная площадь" };
		std::map<mapKeyType, mapValueType> dict;
		ProgramState errorCode = ProgramState::SUCCESS;
		WHEN("phrase write in dict and dict write in file")
		{
			// записать фразу и ее перевод в словарь
			InsertValueToDict(dict, phrase, translate);
			// записать словарь в файл
			SaveDictToFile("tstDict.txt", dict, errorCode);
			THEN("readed frase and its translation are equal")
			{
				std::map<mapKeyType, mapValueType> readedDict = GetDictFromFile("tstDict.txt", errorCode);
				auto findedValue = GetValueFromDictByKey(readedDict, phrase);
				REQUIRE(*findedValue == translate);
			}
		}
	}
}