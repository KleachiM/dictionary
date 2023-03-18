#include "Functions.h"
using namespace std;

int main(int argc, char* argv[])
{
	map<string, string> dict;
	auto fileName = ParseArgs(argc, argv);
	// TODO: может быть тоже убрать в функцию
	if (!fileName)
	{
		// пустой словарь, сохранить в конце программы
	}
	else
	{
		// читать словарь из файла
	}

	// TODO: убрать в функцию void OpenSession(cin, dict);
	string token;
	while (true)
	{
		// чтение входного потока
		getline(cin, token);
		// (если == ...) break;
		if (token == "...")
			break;
		// если token существует вернуть значение
		// если token не существует, создать новый токен
	}

	// если размер словаря изменился, переписать файл по запросу

	return EXIT_SUCCESS;
}
