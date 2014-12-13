#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include <system_error>

#include "ThrowMacro.h"

static std::string combine_message_elements(const char* file, unsigned int line, const char* user_message, const char* sys_message)
{
	std::ostringstream msg;
	std::string f{ file };
	msg << sys_message << " (" << f.substr(f.rfind('/') + 1) << ":" << line << ")";
	if (user_message) {
		msg << ", " << user_message;
	}
	return msg.str();
}

void __throw_if_min1(int x, const char* file, unsigned int line, const char* message)
{
	if (x == -1) {
		std::error_code ec{ errno, std::system_category() };
		std::ostringstream ec_str;
		ec_str << "system error " << ec.value() << ": " << ec.message();
		std::string msg{ combine_message_elements(file, line, message, ec_str.str().c_str()) };
		throw std::system_error(ec, msg);
	}
}

void __throw_if_null(const void* p, const char* file, unsigned int line, const char* message)
{
	if (p == nullptr) {
		std::string msg{ combine_message_elements(file, line, message, "null pointer exception") };
		throw std::runtime_error(msg);
	}
}
