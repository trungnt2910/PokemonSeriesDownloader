//variables.h - Variable file processing class for PokemonSeriesDownloader

#ifndef __POKEMON__VARIABLES__H__
#define __POKEMON__VARIABLES__H__

//Forward declarations
#include <cstdint>
#include <iosfwd>
#include <map>
#include <vector>

#include "fetcher.h"

//Borrow a few tools from the standard library...
using std::string;
using std::map;
using std::vector;

class Variables
{
	private:
		static const int dataCount = 3;
		map<string, int32_t> labels;
		vector<string> data;
		FetcherCommand fetch;
	public:
		Variables();
		const string & operator[](const string & label) const;
		FetcherCommand & getFetcher();
};

inline const string & Variables::operator[](const string & label) const
{
	return data[labels.at(label)];
}

inline FetcherCommand & Variables::getFetcher()
{
	return fetch;
}


extern Variables environment;

#endif //__POKEMON__VARIABLES__H__
