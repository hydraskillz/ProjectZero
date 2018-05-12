#pragma once

#include <string>

namespace CS
{
	bool Initialize(const std::string& dllName);
	void Shutdown();
}

std::string EncryptString(const std::string& text);
std::string DecryptString(const std::string& text);
