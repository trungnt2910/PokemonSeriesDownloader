//variables.cpp - Variable file processing class implementation for PokemonSeriesDownloader

#include "variables.h"

#include <fstream>
#include <string>

#include "fetcher.h"
#include "helper.h"

using std::ifstream;
using std::string;

Variables::Variables()
{
	//Data:
	
	//data[0]: Index page of the series.
	labels.insert({"INDEX_PAGE", 0});
	//data[1]: Prefix of each episode. 
	// (For the purpose of filtering episode links from ads and trash).
	labels.insert({"PREFIX", 1});
	//data[2]: Default download path.
	labels.insert({"DOWNLOAD_DIR", 2});
	
	data.resize(dataCount);
	
	//Commands:
	labels.insert({"FETCH_LAYERS", 128});
	labels.insert({"FETCH_COMMAND", 129});
	
	ifstream dataStream("Variables.dat");
	
	while (dataStream)
	{
		string current_line;
		getline(dataStream, current_line);
		//Ignore empty lines
		if (!current_line.size()) continue;
		//Like common script files, lines starting with '#' are ignored.
		if (current_line[0] == '#') continue;
		//Get the flag name and data
		std::size_t i = current_line.find("=");
		string flag_name = string(current_line.begin(), current_line.begin() + i);
		string flag_data = string(current_line.begin() + i + 1, current_line.end());
		//And clean them up.
		if (flag_name.back() == '+') flag_name.pop_back();
		while (isspace(flag_name.back() == ' ')) flag_name.pop_back();
		while (isspace(flag_data.back() == ' ')) flag_data.pop_back();
		i = 0;
		while (isspace(flag_name[i])) ++i;
		flag_name = string(flag_name.begin() + i, flag_name.end());
		i = 0;
		while (isspace(flag_data[i])) ++i;
		flag_data = string(flag_data.begin() + i, flag_data.end());
		
		if (labels[flag_name] < dataCount)
		{
			data[labels[flag_name]] = flag_data;
		}
		else
		{
			switch (labels[flag_name])
			{
				case 128:
					fetch.setCommandCount(stoi(flag_data));
				break;
				case 129:
					vector<string> params;
										
					std::size_t i = flag_data.find("(") + 1;
					std::size_t j = flag_data.find(")", i);
										
					for (std::size_t x = i; x < j; ++x)
					{
						if (flag_data[x] == '\"')
						{
							int y = flag_data.find("\"", x + 1);
							while (flag_data[y - 1] == '\\')
							{
								y = flag_data.find("\"", y + 1);
							}
							params.push_back(string(flag_data.begin() + x + 1, flag_data.begin() + y));
							params.back() = findAndReplaceAll(params.back(), "\\\"", "\"");
							params.back() = findAndReplaceAll(params.back(), "\\\\", "\\");
							x = y;
						}
					}
					
					while (params.size() < 5) params.push_back("");
					fetch.addCommand(params[0], params[1], params[2], params[3], params[4]);
				break;
			}
		}
	}
}
Variables environment;

