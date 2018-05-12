#pragma once

#include <string>

namespace Invoker
{
	typedef void* Object;

	Object Create(const wchar_t* pszVersion, const wchar_t* pszAssemblyName, const wchar_t* pszClassName);
	void Destroy(Object& obj);

	// Calling codes
	// Call [Static] Method [Return][Args...]
	//
	// V = void
	// S = string
	//

	std::string CallMethodSV(Object obj, const wchar_t* methodName);
	std::string CallMethodSS(Object obj, const wchar_t* methodName, const std::string& s);

	std::string CallStaticMethodSS(Object obj, const wchar_t* methodName, const std::string& s);
}