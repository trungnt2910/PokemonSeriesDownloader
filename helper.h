//helper.h -- Helper functions for PokemonSeriesDownloader
#ifndef __POKEMON__HELPER__H__
#define __POKEMON__HELPER__H__

//Forward declarations...
#include <iosfwd>

//Cannot forward declare vector :(
#include <vector>

//Borrow a few tools from the standard library...
using std::string;
using std::vector;

string findAndReplaceAll(string data, const string & toSearch, const string & replaceStr);
string StandardizePath(string path);
string purifyLink(string link);
string decodeURL(string link);
string removeIllegalFilenameCharacters(string filename);
void PrepareDirectory(const string & fullpath);
std::vector<string> GET_VARIABLES();
bool checkExistence(const string & filePath);

#endif //__POKEMON__HELPER__H__
