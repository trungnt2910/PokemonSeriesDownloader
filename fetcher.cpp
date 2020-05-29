//fetcher.cpp -- Link extraction functions for PokemonSeriesDownloader

//Current fetcher supports both the old elite-video-player AND the new
//Dailymotion videos embedded here, depending on the code written in 
//variables.dat 

#include "fetcher.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Windows.h> //For URLDownloadtoFile

#include "helper.h"
#include "variables.h"

//Borrow a few tools from the standard library
using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;
using std::vector;

extern vector<string> links;
extern vector<string> names;
extern vector<string> direct;
extern vector<string> data;

string FetcherCommand::operator()(const string & directory, const string & episodeName, string link)
{
	for (const auto & rc : commands)
	{
		URLDownloadToFile(nullptr, link.c_str(), (directory + episodeName + rc.fileNameSuffix).c_str(), 0, nullptr);
		link = processFile(directory + episodeName + rc.fileNameSuffix, rc);
	}
	return link;
}

string FetcherCommand::processFile(const string & filePath, const FetchRecord & rc)
{
	ifstream fin;
		fin.open(filePath.c_str());
		string current_line;
		getline(fin, current_line);
		while (current_line.find(rc.elementToken) == std::string::npos)
		{
			getline(fin, current_line);
			if (fin.eof()) //TODO: implement exception throwing.
			{
				cout << "Failed to get link.\n";
				return "INVALID FILE.";
			}
		}
		string data = processElement(current_line, rc);
		data = purifyLink(data);
	fin.close();
	return data;
}

string FetcherCommand::processElement(string & element, const FetchRecord & rc)
{
	std::size_t i = element.find(rc.linkToken);
	i = element.find(rc.linkPrefix, i);
	i += rc.linkPrefix.size();
	std::size_t j = element.find(rc.linkSuffix, i+1);
	string result = string(element.begin() + i, element.begin() + j);
	return decodeURL(result); 
}

void processIndexPage(const string & pagename, const string & tempfolder)
{
	
	ifstream html((tempfolder + pagename).c_str()); //Stream to process .html file.

	cout << "Processing data file...\n";
	uint32_t count = 0;
	string current;

	while (!html.eof())
	{
		getline(html, current);
		static int16_t i;
		i = current.find(environment["PREFIX"]);
		if (i!=string::npos)
		{
			++count;
			static int16_t j;
			j = i;
			while (current[j] != '\"') ++j;
			links.push_back(string(current.begin() + i, current.begin() + j));
			i = current.find("Countdown");
			if (i != string::npos) //Coming soon!
			{
				cout << "Link:\n";
				cout << links.back() << endl;
				cout << "Episode coming soon!\n";
				links.pop_back();
				--count;
				break;
			}
			i = current.find("Episode");
			if (i == string::npos) // NOT a link to an episode
			{
			//	cout << "Invalid link."; //Shuts the app up.
				links.pop_back();
				--count;
				continue;
			}
			cout << "Link:\n";
			cout << links.back() << endl;
			cout << "Name:\n";
			j = i;
			while (current[j] != '<') ++j;
			string temp = string(current.begin() + i, current.begin() + j);
			temp = removeIllegalFilenameCharacters(temp);
			temp = findAndReplaceAll(temp, "  ", " ");
			while (temp.back() == ' ') temp.pop_back();
			temp += ".mp4";
			names.push_back(temp);
			cout << names.back() << endl;
		}
	}
	cout << "\n--------------------------------------------\n";
	cout << "Found " << count << " episodes.\n";
	cout << "Fetching data for each episode:\n";
	count = 0;
	for (int16_t i = 1; i<links.size(); ++i)
	{
		stringstream ss;
		ss << "Episode " << i;
		string episodename = ss.str();
		static const string extension = ".html";
		cout << episodename << ":\n";
		cout << "Fetching data....\n";
		cout << "Getting direct URL...\n";
		string directURL = environment.getFetcher()(tempfolder, episodename, links[i]);
		cout << "Extracted link:\n";
		cout << directURL << endl;
		cout << "Done.\n\n";
		direct.push_back(directURL);
		if (directURL != "INVALID FILE.") ++count;
	}
	cout << "\n--------------------------------------------\n";
	cout << count << " episodes ready to download.\n\n";
}
