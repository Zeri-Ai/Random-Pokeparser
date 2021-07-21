#pragma once
#include <fstream>
#include <map>
#include <mutex>
namespace constants
{
	enum class Log_level {
		trace,
		debug,
		info,
		warn,
		error,
		fatal,
		off
	};
	constexpr char default_log_name[] { "logfile" };
	constexpr char default_extension[]{ ".log" };
	static constexpr int byte_order_mark_size{ 3 };
}
namespace utilities
{
	std::string replace(const std::string& str, const char old_text, const char new_text);
}
class Logger
{
private:
	static constants::Log_level m_log_level;
	static std::mutex m_log_mutex;
	static std::ofstream m_log;
	template <typename T> static void log(const char* header, T message)
	{
		Logger::m_log << '[' << header << ']' << message << std::endl;
	}
public:
	static void change_priority(constants::Log_level level);
	template <typename T = char*> static void trace(T message = "")
	{
		if (m_log_level <= constants::Log_level::trace)
		{
			Logger::log("trace", message);
		}
	}
	template <typename T = char*> static void debug(T message = "")
	{
		if (m_log_level <= constants::Log_level::debug)
		{
			Logger::log("debug", message);
		}
	}
	template <typename T = char*> static void info(T message = "")
	{
		if (m_log_level <= constants::Log_level::info)
		{
			Logger::log("info", message);
		}
	}
	template <typename T = char*> static void warn(T message = "")
	{
		if (m_log_level <= constants::Log_level::warn)
		{
			Logger::log("warn", message);
		}
	}
	template <typename T = char*> static void error(T message = "")
	{
		if (m_log_level <= constants::Log_level::error)
		{
			Logger::log("error", message);
		}
	}
	template <typename T = char*> static void fatal(T message = "")
	{
		if (m_log_level <= constants::Log_level::fatal)
		{
			Logger::log("fatal", message);
		}
	}
};