//fetcher.h -- Link extraction functions for PokemonSeriesDownloader

#ifndef __POKEMON__FETCHER__H__
#define __POKEMON__FETCHER__H__

//Forward declarations
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

//Borrow a few tools from the standard library...
using std::string;
using std::vector;

void processIndexPage(const string & pagename, const string & tempfolder);

class FetcherCommand
{
	private:
		struct FetchRecord
		{
			string fileNameSuffix;
			string elementToken;
			string linkToken;
			string linkPrefix;
			string linkSuffix;
			FetchRecord(const string & fN, const string & eT, const string & lT, const string & lP, const string & lS)
				: fileNameSuffix(fN), elementToken(eT), linkToken(lT), linkPrefix(lP), linkSuffix(lS)
			{};
			FetchRecord() = default;
		};
		vector<FetchRecord> commands;
		std::size_t commandCount;
		static string processFile(const string & filePath, const FetchRecord & record);
		static string processElement(string & element, const FetchRecord & record);
	public:
		FetcherCommand() = default;
		FetcherCommand & setCommandCount(const std::size_t count);
		FetcherCommand & addCommand(const string & fileNameSuffix, const string & elementToken, 
									const string & linkToken, const string & linkPrefix,
									const string & linkSuffix);
		string operator()(const string & directory, const string & episodeName, string link);
};

inline FetcherCommand & FetcherCommand::setCommandCount(const std::size_t count)
{
	commands.reserve(count);
	commandCount = count;
	return (*this);
}

inline FetcherCommand & FetcherCommand::addCommand(const string & fileNameSuffix, const string & elementToken, 
													const string & linkToken, const string & linkPrefix,
													const string & linkSuffix)
{
	if (commands.size() < commandCount)
	commands.emplace_back(fileNameSuffix, elementToken, linkToken, linkPrefix, linkSuffix);
	return (*this);
}

#endif //__POKEMON__FETCHER__H__
