#pragma once

#include <string>

struct CMSData;

namespace Server
{
	const std::string& GetVersion();
	const CMSData& GetCMSData();
	void Run();
}