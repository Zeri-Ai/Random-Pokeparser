#include <chrono>
#include <fstream>
#include <mutex>
#include "Util.h"
constants::Log_level Logger::m_log_level{ constants::Log_level::trace };
std::mutex Logger::m_log_mutex;
std::ofstream Logger::m_log{
	[]() -> std::string
	{
		std::scoped_lock lock(Logger::m_log_mutex);
		std::ostringstream stringstream;
		stringstream << constants::default_log_name;
		stringstream << constants::default_extension;
		return stringstream.str();
	}()
};
void Logger::change_priority(constants::Log_level level)
{
	m_log_level = level;
}