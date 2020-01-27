#include "helper.h"

string findAndReplaceAll(string data, const string & toSearch, const string & replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
	// Repeat till end is reached
	while(pos != string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos = data.find(toSearch, pos + replaceStr.size());
	}
	return data;
}

string StandardizePath(string path)
{
	if (path[0] == '\"') path.erase(path.begin(), path.begin() + 1);
	if (path.back() == '\"') path.pop_back();
	if (path.back() == '\\') path.pop_back();
	return path;
}

void PrepareDirectory(const string & fullpath)
{
	int16_t i = 0;
	if (fullpath[1] == ':')	i = 3; //skips drive letter;
	int16_t j = i;
	while (i < fullpath.size())
	{
		while (fullpath[j] != '\\')
		++j;
		CreateDirectory(string(fullpath.begin(), fullpath.begin()+j).c_str(), nullptr);
		i = j + 1;
		j = i;
	}
}

string purifyLink(string link)
{
	for (int16_t i = 0; i<link.size(); ++i)
	{
		if (link[i] == '\\')
		{
			link.erase(link.begin() + i);
			--i;
		}		
	}
	//Removes the stupid escape sequence
	if (link.back() == ';' && isdigit(*(link.end() - 2)))
	{
		while (link.back() != '&') link.pop_back();
		link.pop_back();
	}
	//Removes stupid '"' character
	if (link.back() == '"')
	{
		link.pop_back();
	}
	return link;
}

vector<string> GET_VARIABLES()
{
	//Coming soon: configure variables to support downloading on other Pokemon sites.
	map<string, int16_t> flags;
	//data[0]: Index page of the series.
	flags.insert({"INDEX_PAGE", 0});
	//data[1]: Prefix of each episode. 
	// (For the purpose of filtering episode links from ads and trash).
	flags.insert({"PREFIX", 1});
	//data[2]: Default download path.
	flags.insert({"DOWNLOAD_DIR", 2});
	
	ifstream data("Variables.dat");
	string current_line;
	
	vector<string> return_data(flags.size());
	
	while (data)
	{
		getline(data, current_line);
		//Ignore empty lines
		if (!current_line.size()) continue;
		//Like common script files, lines starting with '#' are ignored.
		if (current_line[0] == '#') continue;
		//Get the flag name and data
		int16_t i = current_line.find("=");
		string flag_name = string(current_line.begin(), current_line.begin() + i);
		string flag_data = string(current_line.begin() + i + 1, current_line.end());
		//And clean them up.
		while (isspace(flag_name.back() == ' ')) flag_name.pop_back();
		while (isspace(flag_data.back() == ' ')) flag_data.pop_back();
		i = 0;
		while (isspace(flag_name[i])) ++i;
		flag_name = string(flag_name.begin() + i, flag_name.end());
		i = 0;
		while (isspace(flag_data[i])) ++i;
		flag_data = string(flag_data.begin() + i, flag_data.end());
		
		return_data[flags[flag_name]] = flag_data;
	}

	return return_data;
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
