#include <fstream>
#include <string>
#include "LogParser.h"
#include "Util.h"
const std::regex Parser::pokemon_evolution{ R"((.+?) now evolves into ((?:.+?, )*)(?:(.+?)? and )?(.+?)$)" };
const std::string Parser::type_pokemon_evolution{ "--Randomized Evolutions--" };
const std::string Parser::type_pokemon_movesets_metronome{ "Pokemon Movesets: Metronome Only" };
Parser::Parser(StringType current_string_type): m_current_string_type{current_string_type}
{
}
bool Parser::current_type_parsable(const std::string& line)
{
	switch (m_current_string_type)
	{
	case StringType::none:
		return false;
	case StringType::randomized_evolutions:
		std::stringstream str_stream;
		std::smatch sm;
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
		std::string token{};
		while (str_stream >> token)
		{
			//For each pokemon do:
			//First pokemon is the prevolution
			Logger::trace(token);
		}
		return true;
	}
	
	return false;
}
bool Parser::parse_type(const std::string& line)
{
	if (line == type_pokemon_evolution)
	{
		Logger::trace("Type parsed as Pokemon Evolution.");
		m_current_string_type = StringType::randomized_evolutions;
		return true;
	}
	if (line == type_pokemon_movesets_metronome)
	{
		Logger::trace("Line parsed as Pokemon Movesets: Metronome.");
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