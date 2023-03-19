#include <catch2/catch_test_macros.hpp>
#include "../Functions.h"

TEST_CASE("parse args")
{
	std::string param1 = "progName";
	std::string param2 = "fileName";
	char* tstArgv[] = {param1.data(), param2.data(), NULL};
	REQUIRE(ParseArgs(2, tstArgv) == "fileName");
	REQUIRE(ParseArgs(1, tstArgv) == std::nullopt);
}

// GetMapElementFromString
TEST_CASE("GetMapElementFromString, good scenario (one val)")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	MapElem expectedMapElem = {"key", {"value"}};
	MapElem retMapElem = GetMapElementFromString("key value", errorCode);
	REQUIRE(expectedMapElem.key == retMapElem.key);
	REQUIRE(expectedMapElem.value == retMapElem.value);
}

TEST_CASE("GetMapElementFromString, good scenario (many val)")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	MapElem expectedMapElem = {"key", {"value1", "value2"}};
	MapElem retMapElem = GetMapElementFromString("key value1 value2", errorCode);
	REQUIRE(expectedMapElem.key == retMapElem.key);
	REQUIRE(expectedMapElem.value == retMapElem.value);
}

TEST_CASE("GetMapElementFromString, empty line")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	MapElem expectedMapElem = {"key", {"value"}};
	MapElem retMapElem = GetMapElementFromString("", errorCode);
	REQUIRE(errorCode == ErrorCode::READ_EMPTY_LINE_FROM_FILE);
}

TEST_CASE("GetMapElementFromString, only key")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	MapElem expectedMapElem = {"key", {"value"}};
	MapElem retMapElem = GetMapElementFromString("key", errorCode);
	REQUIRE(errorCode == ErrorCode::READ_MAP_FROM_FILE_ERROR);
}

// ReadMapFromFile
TEST_CASE("ReadMapFromFile, not existing file")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(ReadMapFromFile("./files/notExist.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::FILE_OPENING_ERROR);
}

TEST_CASE("ReadMapFromFile, empty file")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(ReadMapFromFile("../../tests/files/empty.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

TEST_CASE("ReadMapFromFile, file with empty line")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(ReadMapFromFile("../../tests/files/fileWithEmptyLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

TEST_CASE("ReadMapFromFile, file with not valid line")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(ReadMapFromFile("../../tests/files/fileWithNotValidLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

TEST_CASE("ReadMapFromFile, valid file")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};
	REQUIRE(ReadMapFromFile("../../tests/files/validFile.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

// GetEngRusDictFromFile
//TODO: доделать тесты отсюда
TEST_CASE("GetEngRusDictFromFile, not specified file")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(GetEngRusDictFromFile(std::nullopt, errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

TEST_CASE("GetEngRusDictFromFile, not existing file")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(GetEngRusDictFromFile("notExist.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::FILE_OPENING_ERROR);
}

TEST_CASE("GetEngRusDictFromFile, empty file")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap;
	REQUIRE(GetEngRusDictFromFile("../../tests/files/empty.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

TEST_CASE("GetEngRusDictFromFile, file with empty line")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(GetEngRusDictFromFile("../../tests/files/fileWithEmptyLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

TEST_CASE("GetEngRusDictFromFile, file with not valid line")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" } },
			{"dog", {"собака"}},
			{"lion", {"лев"}}
		};
	REQUIRE(GetEngRusDictFromFile("../../tests/files/fileWithNotValidLine.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
}

TEST_CASE("GetEngRusDictFromFile, valid file")
{
	ErrorCode errorCode = ErrorCode::SUCCESS;
	std::map<mapKeyType, mapValueType> expectedMap =
		{
			{ "cat", { "кошка" , "кот"} },
			{"dog", {"собака"}},
			{"bird", {"птица"}},
			{"lion", {"лев"}}
		};
	REQUIRE(GetEngRusDictFromFile("../../tests/files/validFile.txt", errorCode) == expectedMap);
	REQUIRE(errorCode == ErrorCode::SUCCESS);
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