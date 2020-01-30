//fetcher.cpp -- Link extraction functions for PokemonSeriesDownloader

//These functions currently support the old elite-video-player
//Blissey's husband is using. However, he is known to have been
//testing with cloudfront streaming (Episode 48). This involves more complicated
//link fetching as well as using ffmpeg to get valid data from m3u8 files.

//Fetching from cloudfront will be implemented later if Blissey's husband
//adopt this for his site.

#include "fetcher.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <Windows.h> //For URLDownloadtoFile

#include "helper.h"

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

string process(string & element)
{
	//Site-specific variables, in future releases will be read from
	//Variables.dat file.
	static const string link_token = "mp4HD";
	static const string link_prefix = ":";
	static const size_t prefix_to_content = 2;
	static const string link_suffix = "\",";
	static const size_t suffix_to_content = 0;
	
	//Replace stupid fancy HTML escape codes for fancy quotation marks with
	//normal ones.
	element = findAndReplaceAll(element, "&#8221;", "\"");
	element = findAndReplaceAll(element, "&#8243;", "\"");
	int16_t i = element.find(link_token);
	i = element.find(link_prefix, i);
	i += prefix_to_content;
	int16_t j = element.find(link_suffix, i+1);
	j -= suffix_to_content;
	string result = string(element.begin() + i, element.begin() + j);
	return result; 
}

//Finds the HTML element that contains the video.
string processFile(const string & path)
{
	ifstream fin;
		fin.open(path.c_str());
		string current_line;
		getline(fin, current_line);
		while (current_line.find("<div class=\"Elite_video_player\"") == -1)
		{
			getline(fin, current_line);
			if (fin.eof()) //TODO: implement exception throwing.
			{
				cout << "Failed to get link.\n";
				return "INVALID FILE.";
			}
		}
		string data = process(current_line);
		cout << "Extracted link:\n\n";
		data = purifyLink(data);
		cout << data << endl << endl;
		fin.close();
	return data;
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
		i = current.find(data[1]);
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
			static int16_t find_colon;
			find_colon = temp.find(":");
			if (find_colon != string::npos)
			{
				static string replace;
				replace = "";
				if (!isspace(temp[find_colon - 1])) replace += ' ';
				replace += '-';
				if (!isspace(temp[find_colon + 1])) replace += ' ';
				temp.replace(temp.begin() + find_colon, temp.begin() + find_colon + 1, replace);
			}
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
		episodename += extension;
		cout << "Fetching data....\n";
		URLDownloadToFile(nullptr, links[i].c_str(), (tempfolder + episodename).c_str(), 0, nullptr);
		cout << "Getting direct URL...\n";
		string directURL = processFile(tempfolder + episodename);
		cout << "Done.\n\n";
		direct.push_back(directURL);
		if (directURL != "INVALID FILE.") ++count;
	}
	cout << "\n--------------------------------------------\n";
	cout << count << " episodes ready to download.\n\n";
}
