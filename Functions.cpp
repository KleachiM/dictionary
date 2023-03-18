#include "Functions.h"

std::optional<std::string> ParseArgs(int argc, char** argv)
{
	if (argc != 2)
		return std::nullopt;
	return argv[1];
}
