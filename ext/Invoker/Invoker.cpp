#include "Invoker.h"

#include <Windows.h>
#include <metahost.h>

#pragma comment(lib, "mscoree.lib")

#import "mscorlib.tlb" raw_interfaces_only				\
    high_property_prefixes("_get","_put","_putref")		\
    rename("ReportEvent", "InteropServices_ReportEvent")
using namespace mscorlib;

class CSClass
{
public:
	CSClass();
	~CSClass();

	bool Bind(const wchar_t* pszVersion, const wchar_t* pszAssemblyName, const wchar_t* pszClassName);

	std::string CallMethodSV(const wchar_t* methodName);
	std::string CallMethodSS(const wchar_t* methodName, const std::string& s);

	std::string CallStaticMethodSS(const wchar_t* methodName, const std::string& s);
private:
	ICLRMetaHost *pMetaHost = NULL;
	ICLRRuntimeInfo *pRuntimeInfo = NULL;
	ICorRuntimeHost *pCorRuntimeHost = NULL;
	IUnknownPtr spAppDomainThunk = NULL;
	_AppDomainPtr spDefaultAppDomain = NULL;
	_AssemblyPtr spAssembly = NULL;
	_TypePtr spType = NULL;
	bstr_t bstrClassName;
	variant_t vtObject;
};

CSClass::CSClass()
{
	
}

CSClass::~CSClass()
{
	if (pMetaHost)
	{
		pMetaHost->Release();
		pMetaHost = NULL;
	}
	if (pRuntimeInfo)
	{
		pRuntimeInfo->Release();
		pRuntimeInfo = NULL;
	}
	if (pCorRuntimeHost)
	{
		// Please note that after a call to Stop, the CLR cannot be 
		// reinitialized into the same process. This step is usually not 
		// necessary. You can leave the .NET runtime loaded in your process.
		//wprintf(L"Stop the .NET runtime\n");
		//pCorRuntimeHost->Stop();

		pCorRuntimeHost->Release();
		pCorRuntimeHost = NULL;
	}
}

bool CSClass::Bind(const wchar_t* pszVersion, const wchar_t* pszAssemblyName, const wchar_t* pszClassName)
{
	HRESULT hr;

	bstr_t bstrAssemblyName(pszAssemblyName);
	bstrClassName = pszClassName;

	// 
	// Load and start the .NET runtime.
	// 

	wprintf(L"Load and start the .NET runtime %s \n", pszVersion);

	hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
	if (FAILED(hr))
	{
		wprintf(L"CLRCreateInstance failed w/hr 0x%08lx\n", hr);
		return false;
	}

	// Get the ICLRRuntimeInfo corresponding to a particular CLR version. It 
	// supersedes CorBindToRuntimeEx with STARTUP_LOADER_SAFEMODE.
	hr = pMetaHost->GetRuntime(pszVersion, IID_PPV_ARGS(&pRuntimeInfo));
	if (FAILED(hr))
	{
		wprintf(L"ICLRMetaHost::GetRuntime failed w/hr 0x%08lx\n", hr);
		return false;
	}

	// Check if the specified runtime can be loaded into the process. This 
	// method will take into account other runtimes that may already be 
	// loaded into the process and set pbLoadable to TRUE if this runtime can 
	// be loaded in an in-process side-by-side fashion. 
	BOOL fLoadable;
	hr = pRuntimeInfo->IsLoadable(&fLoadable);
	if (FAILED(hr))
	{
		wprintf(L"ICLRRuntimeInfo::IsLoadable failed w/hr 0x%08lx\n", hr);
		return false;
	}

	if (!fLoadable)
	{
		wprintf(L".NET runtime %s cannot be loaded\n", pszVersion);
		return false;
	}

	// Load the CLR into the current process and return a runtime interface 
	// pointer. ICorRuntimeHost and ICLRRuntimeHost are the two CLR hosting  
	// interfaces supported by CLR 4.0. Here we demo the ICorRuntimeHost 
	// interface that was provided in .NET v1.x, and is compatible with all 
	// .NET Frameworks. 
	hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost,
		IID_PPV_ARGS(&pCorRuntimeHost));
	if (FAILED(hr))
	{
		wprintf(L"ICLRRuntimeInfo::GetInterface failed w/hr 0x%08lx\n", hr);
		return false;
	}

	// Start the CLR.
	hr = pCorRuntimeHost->Start();
	if (FAILED(hr))
	{
		wprintf(L"CLR failed to start w/hr 0x%08lx\n", hr);
		return false;
	}

	// Get a pointer to the default AppDomain in the CLR.
	hr = pCorRuntimeHost->GetDefaultDomain(&spAppDomainThunk);
	if (FAILED(hr))
	{
		wprintf(L"ICorRuntimeHost::GetDefaultDomain failed w/hr 0x%08lx\n", hr);
		return false;
	}

	hr = spAppDomainThunk->QueryInterface(IID_PPV_ARGS(&spDefaultAppDomain));
	if (FAILED(hr))
	{
		wprintf(L"Failed to get default AppDomain w/hr 0x%08lx\n", hr);
		return false;
	}

	// Load the .NET assembly.
	wprintf(L"Load the assembly %s\n", pszAssemblyName);
	hr = spDefaultAppDomain->Load_2(bstrAssemblyName, &spAssembly);
	if (FAILED(hr))
	{
		wprintf(L"Failed to load the assembly w/hr 0x%08lx\n", hr);
		return false;
	}

	// Get the Type of CSSimpleObject.
	hr = spAssembly->GetType_2(bstrClassName, &spType);
	if (FAILED(hr))
	{
		wprintf(L"Failed to get the Type interface w/hr 0x%08lx\n", hr);
		return false;
	}

	// Instantiate the class.
	hr = spAssembly->CreateInstance(bstrClassName, &vtObject);
	if (FAILED(hr))
	{
		wprintf(L"Assembly::CreateInstance failed w/hr 0x%08lx\n", hr);
		return false;
	}
	return true;
}

std::string CSClass::CallMethodSV(const wchar_t* methodName)
{
	HRESULT hr;
	bstr_t bstrMethodName(methodName);
	SAFEARRAY *psaMethodArgs = NULL;
	variant_t vtStringRet;

	// Call the instance method of the class.
	//   public string ToString();

	// Create a safe array to contain the arguments of the method.
	psaMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 0);

	// Invoke the "ToString" method from the Type interface.
	hr = spType->InvokeMember_3(bstrMethodName, static_cast<BindingFlags>(
		BindingFlags_InvokeMethod | BindingFlags_Instance | BindingFlags_Public),
		NULL, vtObject, psaMethodArgs, &vtStringRet);
	if (FAILED(hr))
	{
		wprintf(L"Failed to invoke ToString w/hr 0x%08lx\n", hr);
	}
	else
	{
		// Print the call result of the method.
		/*wprintf(L"Call %s.%s() => %s\n",
			static_cast<PCWSTR>(bstrClassName),
			static_cast<PCWSTR>(bstrMethodName),
			static_cast<PCWSTR>(vtStringRet.bstrVal));*/

		std::wstring wstr(vtStringRet.bstrVal);
		return std::string(wstr.begin(), wstr.end());
	}

	if (psaMethodArgs)
	{
		SafeArrayDestroy(psaMethodArgs);
		psaMethodArgs = NULL;
	}

	return "";
}

std::string CSClass::CallMethodSS(const wchar_t* methodName, const std::string& s)
{
	HRESULT hr;
	bstr_t bstrMethodName(methodName);
	SAFEARRAY *psaMethodArgs = NULL;
	variant_t vtStringRet;

	// Call the instance method of the class.
	//   public string ToString();
	
	// Create a safe array to contain the arguments of the method.
	psaMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	
	variant_t arg0;
	arg0.SetString(s.c_str());

	variant_t HUGEP* pdFreq;
	hr = SafeArrayAccessData(psaMethodArgs, (void HUGEP* FAR*)&pdFreq);
	if (SUCCEEDED(hr))
	{
		*pdFreq++ = arg0;
		SafeArrayUnaccessData(psaMethodArgs);
	}

	// Invoke the "ToString" method from the Type interface.
	hr = spType->InvokeMember_3(bstrMethodName, static_cast<BindingFlags>(
		BindingFlags_InvokeMethod | BindingFlags_Instance | BindingFlags_Public),
		NULL, vtObject, psaMethodArgs, &vtStringRet);
	if (FAILED(hr))
	{
		wprintf(L"Failed to invoke %s w/hr 0x%08lx\n", methodName, hr);
	}
	else
	{
		// Print the call result of the method.
		/*wprintf(L"Call %s.%s() => %s\n",
			static_cast<PCWSTR>(bstrClassName),
			static_cast<PCWSTR>(bstrMethodName),
			static_cast<PCWSTR>(vtStringRet.bstrVal));*/

		std::wstring wstr(vtStringRet.bstrVal);
		return std::string(wstr.begin(), wstr.end());
	}

	if (psaMethodArgs)
	{
		SafeArrayDestroy(psaMethodArgs);
		psaMethodArgs = NULL;
	}

	return "";
}

std::string CSClass::CallStaticMethodSS(const wchar_t* methodName, const std::string& s)
{
	HRESULT hr;
	bstr_t bstrStaticMethodName(methodName);
	SAFEARRAY *psaStaticMethodArgs = NULL;
	variant_t vtStringRet;
	variant_t vtStringArg(s.c_str());
	variant_t vtEmpty;

	// Call the static method of the class: 
	//   public static int GetStringLength(string str);

	// Create a safe array to contain the arguments of the method. The safe 
	// array must be created with vt = VT_VARIANT because .NET reflection 
	// expects an array of Object - VT_VARIANT. There is only one argument, 
	// so cElements = 1.
	psaStaticMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	LONG index = 0;
	hr = SafeArrayPutElement(psaStaticMethodArgs, &index, &vtStringArg);
	if (FAILED(hr))
	{
		wprintf(L"SafeArrayPutElement failed w/hr 0x%08lx\n", hr);
	}
	else
	{
		// Invoke the "GetStringLength" method from the Type interface.
		hr = spType->InvokeMember_3(bstrStaticMethodName, static_cast<BindingFlags>(
			BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public),
			NULL, vtEmpty, psaStaticMethodArgs, &vtStringRet);
		if (FAILED(hr))
		{
			wprintf(L"Failed to invoke %s w/hr 0x%08lx\n", methodName, hr);
		}
		else
		{
			// Print the call result of the static method.
			/*wprintf(L"Call %s.%s(\"%s\") => %s\n",
				static_cast<PCWSTR>(bstrClassName),
				static_cast<PCWSTR>(bstrStaticMethodName),
				static_cast<PCWSTR>(vtStringArg.bstrVal),
				static_cast<PCWSTR>(vtStringRet.bstrVal));*/

			std::wstring wstr(vtStringRet.bstrVal);
			return std::string(wstr.begin(), wstr.end());
		}
	}
	return "";
}

CSClass* ObjectToCSClass(Invoker::Object obj)
{
	return reinterpret_cast<CSClass*>(obj);
}

Invoker::Object Invoker::Create(const wchar_t* pszVersion, const wchar_t* pszAssemblyName, const wchar_t* pszClassName)
{
	CSClass* pclass = new CSClass();
	if (!pclass->Bind(pszVersion, pszAssemblyName, pszClassName))
	{
		delete pclass;
	}
	else
	{
		return static_cast<Invoker::Object>(pclass);
	}
	return nullptr;
}

void Invoker::Destroy(Invoker::Object& obj)
{
	CSClass* pclass = ObjectToCSClass(obj);
	delete pclass;
	obj = nullptr;
}

std::string Invoker::CallMethodSV(Invoker::Object obj, const wchar_t* methodName)
{
	CSClass* pclass = ObjectToCSClass(obj);
	return pclass->CallMethodSV(methodName);
}

std::string Invoker::CallMethodSS(Invoker::Object obj, const wchar_t* methodName, const std::string& s)
{
	CSClass* pclass = ObjectToCSClass(obj);
	return pclass->CallMethodSS(methodName, s);
}

std::string Invoker::CallStaticMethodSS(Object obj, const wchar_t* methodName, const std::string& s)
{
	CSClass* pclass = ObjectToCSClass(obj);
	return pclass->CallStaticMethodSS(methodName, s);
}
