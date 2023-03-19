#include "Functions.h"
using namespace std;

int main(int argc, char* argv[])
{
	auto fileName = ParseArgs(argc, argv);
	ErrorCode errorCode = ErrorCode::SUCCESS;
	map<mapKeyType , mapValueType> dict = GetEngRusDictFromFile(fileName, errorCode);

	OpenSession(std::cin, dict, errorCode);

	std::cout << "Session ended" << std::endl;
	//TODO: записать словарь в файл (создать файл или изменить файл)
	return EXIT_SUCCESS;
}
