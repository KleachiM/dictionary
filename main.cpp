#include "Functions.h"
using namespace std;

int main(int argc, char* argv[])
{
	ProgramState programState = ProgramState::SUCCESS;
	auto fileName = ParseArgs(argc, argv, programState);

	if (programState == ProgramState::BAD_ARGS_COUNT)
	{
		std::cout << "Неправильное количество аргументов" << std::endl;
		std::cout << "Пример вызова функции: ./dictionary <путь к файлу со словарем (при наличии)>" << std::endl;
		return EXIT_FAILURE;
	}

	map<mapKeyType , mapValueType> dict = GetDictFromFile(fileName, programState);

	if (programState == ProgramState::FILE_OPENING_ERROR)
	{
		std::cout << "Невозможно прочитать файл " << *fileName << std::endl;
		return EXIT_FAILURE;
	}

	OpenSession(std::cin, dict, programState);

	if (programState == ProgramState::SUCCESS_MAP_CHANGED)
	{
		if (NeedToSaveDict())
		{
			SaveDictToFile(fileName, dict, programState);
			if (programState == ProgramState::FILE_WRITING_ERROR)
			{
				std::cout << "Невозможно записать изменения в файл " << *fileName << std::endl;
				return EXIT_FAILURE;
			}
			std::cout << "Изменения сохранены" << std::endl;
		}
	}

	return EXIT_SUCCESS;
}
