//fetcher.h -- Link extraction functions for PokemonSeriesDownloader

#ifndef __POKEMON__FETCHER__H__
#define __POKEMON__FETCHER__H__

//Forward declarations
#include <iosfwd>

//Borrow a few tools from the standard library...
using std::string;

void processIndexPage(const string & pagename, const string & tempfolder);

#endif //__POKEMON__FETCHER__H__
