#include "cs_bridge.h"
#include "../Invoker/Invoker.h"

Invoker::Object gDLL;

bool CS::Initialize(const std::string& dllName)
{
	gDLL = Invoker::Create(L"v4.0.30319", std::wstring(dllName.begin(), dllName.end()).c_str(), L"SecurityUtility");
	return gDLL != nullptr;;
}

void CS::Shutdown()
{
	Invoker::Destroy(gDLL);
}

std::string EncryptString(const std::string& text)
{
	return Invoker::CallStaticMethodSS(gDLL, L"EncryptString", text);
}

std::string DecryptString(const std::string& text)
{
	return Invoker::CallStaticMethodSS(gDLL, L"DecryptString", text);
}