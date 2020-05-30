//helper.cpp -- Helper functions for PokemonSeriesDownloader

#include "helper.h"

#include <cctype>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <Windows.h>

//Borrow a few tools from the standard library
using std::ifstream;

string StandardizePath(string path)
{
	if (path[0] == '\"') path.erase(path.begin(), path.begin() + 1);
	if (path.back() == '\"') path.pop_back();
	if (path.back() == '\\') path.pop_back();
	return path;
}

void PrepareDirectory(const string & fullpath)
{
	size_t i = 0;
	if (fullpath[1] == ':')	i = 3; //skips drive letter;
	size_t j = i;
	while (i < fullpath.size())
	{
		while ((fullpath[j] != '\\')&&(j < fullpath.size()))
		++j;
		CreateDirectory(string(fullpath.begin(), fullpath.begin()+j).c_str(), nullptr);
		i = j + 1;
		j = i;
	}
}

bool checkExistence(const string & filePath)
{
	static ifstream opener;
	opener.open(filePath);
	if (opener.is_open())
	{
		opener.close();
		return true;
	}
	opener.close();
	return false;
}
