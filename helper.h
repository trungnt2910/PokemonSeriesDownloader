//helper.h -- Helper functions for PokemonSeriesDownloader
#ifndef __POKEMON__HELPER__H__
#define __POKEMON__HELPER__H__

#include <cctype>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include <Windows.h>

//Borrow a few tools from the standard library
using std::ifstream;
using std::isdigit;
using std::isspace;
using std::map;
using std::string;
using std::vector;

string findAndReplaceAll(string data, const string & toSearch, const string & replaceStr);
string StandardizePath(string path);
string purifyLink(string link);
void PrepareDirectory(const string & fullpath);
std::vector<string> GET_VARIABLES();
bool checkExistence(const string & filePath);

#endif
