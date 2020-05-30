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

string StandardizePath(string path);
void PrepareDirectory(const string & fullpath);
bool checkExistence(const string & filePath);

#endif //__POKEMON__HELPER__H__
