#include "date_time.h"

#include <ctime>
#include <sstream>
#include <iomanip>

DateTime GetCurrentDateTime()
{
	std::time_t t = std::time(nullptr);
	std::tm tm = *std::localtime(&t);
	std::stringstream ss;
	ss << std::put_time(&tm, "%FT%T");
	return ss.str();
}