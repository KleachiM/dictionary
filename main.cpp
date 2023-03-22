#include "Functions.h"
using namespace std;

int main(int argc, char* argv[])
{
	auto fileName = ParseArgs(argc, argv);

	ErrorCode errorCode = ErrorCode::SUCCESS;

	map<mapKeyType , mapValueType> dict = GetEngRusDictFromFile(fileName, errorCode);

	if (errorCode == ErrorCode::FILE_OPENING_ERROR)
	{
		std::cout << "Невозможно прочитать файл " << *fileName << std::endl;
		return EXIT_FAILURE;
	}

	OpenSession(std::cin, dict, errorCode);

	if (errorCode == ErrorCode::SUCCESS_MAP_CHANGED)
	{
		if (NeedToSaveDict())
		{
			SaveDictToFile(fileName, dict, errorCode);
			if (errorCode == ErrorCode::FILE_WRITING_ERROR)
			{
				std::cout << "Невозможно записать изменения в файл " << *fileName << std::endl;
				return EXIT_FAILURE;
			}
			std::cout << "Изменения сохранены" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
