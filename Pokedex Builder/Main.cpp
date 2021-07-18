#include "LogParser.h"

int main()
{
	Parser* parser = new Parser{};
	parser->parse_file("file2.log");
	delete parser;
	return 0;
}