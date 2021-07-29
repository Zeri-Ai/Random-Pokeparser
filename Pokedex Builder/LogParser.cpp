#include <fstream>
#include <string>
#include "LogParser.h"
#include "Util.h"
const std::regex Parser::pokemon_evolution{ R"((.+?) now evolves into ((?:.+?, )*)(?:(.+?)? and )?(.+?)$)" };
const std::string Parser::type_pokemon_evolution{ "--Randomized Evolutions--" };
const std::string Parser::type_pokemon_moveset_metronome{ "Pokemon Movesets: Metronome Only" };
const std::string Parser::type_pokemon_moveset{ "--Pokemon Movesets--" };
const std::regex Parser::pokemon_moveset{ R"((\d+?) (.+?) *: ((?:(?:.+? at level \d+),)* (?:.+? at level \d+)))" };
const std::string Parser::type_pokemon_tm_move{ "--TM Moves--" };
Parser::Parser(StringType current_string_type): m_current_string_type{current_string_type}
{
}
bool Parser::current_type_parsable(const std::string& line)
{
	std::stringstream str_stream;
	std::smatch sm;
	std::string token{};
	switch (m_current_string_type)
	{
	case StringType::none:
		return false;
	case StringType::randomized_evolution:
		if (!std::regex_match(line, sm, pokemon_evolution))
		{
			str_stream << "line: \"" << line << "\" is not a randomized evolution.";
			Logger::trace(str_stream.str());
			return false;
		}
		for (unsigned i = 1; i < sm.size(); ++i)
		{
			if (i == 2)
			{
				str_stream << utilities::replace(sm.str(i), ',', ' ') << ' ';
			}
			else
			{
				str_stream << sm[i] << ' ';
			}
		}
		while (str_stream >> token)
		{
			//Each token is a pokemon
			//First token is the evolving pokemon
			Logger::trace(token);
		}
		return true;
	case StringType::moveset:
		if (!std::regex_match(line, sm, pokemon_moveset))
		{
			str_stream << "line: " << line << " is not a pokemon moveset.";
			Logger::trace(str_stream.str());
			return false;
		}
		// First match is pokemon number
		// Second match is pokemon name
		// Third match is every moveset, requires further parsing.
		for (unsigned i = 1; i < sm.size(); ++i)
		{
			str_stream << sm[i] << ' ';
		}
		Logger::trace(str_stream.str());
		return true;
	}
	
	return false;
}
bool Parser::parse_type(const std::string& line)
{
	if (line == type_pokemon_evolution)
	{
		Logger::trace("Type parsed as Pokemon Evolution.");
		m_current_string_type = StringType::randomized_evolution;
		return true;
	}
	if (line == type_pokemon_moveset_metronome)
	{
		Logger::trace("Line parsed as Pokemon Movesets: Metronome.");
		m_current_string_type = StringType::none;
		return true;
	}
	if (line == type_pokemon_moveset)
	{
		Logger::trace("Line parsed as Pokemon Movesets.");
		m_current_string_type = StringType::moveset;
		return true;
	}
	Logger::trace("No type could be parsed.");
	return false;
}
void Parser::parse_line(const std::string& line)
{
	if (line.empty())
	{
		Logger::trace("Line is empty, skipping.");
		return;
	}
	if (current_type_parsable(line))
	{
		return;
	}
	if (!parse_type(line))
	{
		if (m_current_string_type != StringType::none)
		{
			Logger::trace("Current type returned to none.");
			m_current_string_type = StringType::none;
		}
		Logger::trace("Line was not parsed.");
	}
}
void Parser::parse_file(const char* filename)
{
	std::ifstream file_stream{ filename };
	file_stream.ignore(constants::byte_order_mark_size);
	std::string line{};
	while (std::getline(file_stream, line))
	{
		std::ostringstream debug{};
		debug << "Parsing line: " << line;
		Logger::debug(debug.str());
		parse_line(line);
	}
}