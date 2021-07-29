#pragma once
#include <regex>
#include <string>
#include "Util.h"
class Parser
{
public:
	enum class StringType
	{
		none,
		randomized_evolution,
		moveset
	};
	static const std::regex pokemon_evolution;
	static const std::string type_pokemon_evolution;
	static const std::string type_pokemon_moveset_metronome;
	static const std::string type_pokemon_moveset;
	static const std::regex pokemon_moveset;
	static const std::string type_pokemon_tm_move;
private:
	StringType m_current_string_type;
	bool current_type_parsable(const std::string& line);
	bool parse_type(const std::string& line);
	void parse_line(const std::string& line);
public:
	Parser(StringType current_string_type = StringType::none);
	void parse_file(const char* filename);
};