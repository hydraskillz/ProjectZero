#include "util.h"
#include <Windows.h>
#include <direct.h>

// half assed copy pasta from stackoverflow
std::vector<std::string> GetAllFileNamesInDirectory(const std::string& path)
{
	std::vector<std::string> names;
	std::string search_path = path + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

std::string GetCurrentWorkingDir()
{
	char workingDir[256];
	_getcwd(workingDir, sizeof(workingDir));
	return workingDir;
}