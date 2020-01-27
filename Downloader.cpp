#include <bits/stdc++.h>
#include <Windows.h> 
using namespace std;
#define ullong unsigned long long
#define llong long long
ifstream opener;
void findAndReplaceAll(string & data, const string & toSearch, const string & replaceStr)
{
	// Get the first occurrence
	size_t pos = data.find(toSearch);
	// Repeat till end is reached
	while(pos != string::npos)
	{
		// Replace this occurrence of Sub String
		data.replace(pos, toSearch.size(), replaceStr);
		// Get the next occurrence from the current position
		pos =data.find(toSearch, pos + replaceStr.size());
	}
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
	int i = 0;
	if (fullpath[1] == ':')	i = 3; //skips drive letter;
	int j = i;
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
	for (int i = 0; i<link.size(); ++i)
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

//This function currently supports the old elite-video-player
//Blissey's husband is using. However, he is known to have been
//testing with cloudfront streaming (Episode 48). This involves more complicated
//link fetching as well as using ffmpeg to get valid data from m3u8 files.

//Fetching from cloudfront will be implemented later if Blissey's husband
//adopt this for his site.
string process(string & element)
{
	findAndReplaceAll(element, "&#8221;", "\"");
	int i = element.find("mp4HD");
	i = element.find(":", i);
	i += 2;
	int j = i;
	while ((element[j] != ',')) ++j;
	string result = string(element.begin() + i, element.begin() + j);
	return result; 
}

string processfile(const string & path)
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

int Download(const vector<string> & URL, const vector<string> & name, const int begin, const int end, string & dlpath)
{
	int SuccessCount = 0;
	dlpath = StandardizePath(dlpath);
	PrepareDirectory(dlpath);
	cout << "Downloading to: " << dlpath << endl;
	for (int i = begin; i <= end; ++i)
	{
		cout << "Downloading " << name[i] << endl;
		URLDownloadToFile(nullptr, URL[i].c_str(), (dlpath + "\\" + name[i]).c_str(), 0, nullptr);
		opener.open((dlpath + "\\" + name[i]).c_str());
		if (!opener.is_open())
		{
			cout << "Failed.\n";
			opener.close();
		}
		else 
		{
			cout << "Done.\n";
			opener.close();
			++SuccessCount;
		}
	}
	return SuccessCount;
}

int Download(const vector<string> & URL, const vector<string> & name, const vector<int> & queue, const string dlpath)
{
	int SuccessCount = 0;
	PrepareDirectory(dlpath);
	for (const auto & i : queue)
	{
		cout << "Downloading " << name[i] << endl;
		URLDownloadToFile(nullptr, URL[i].c_str(), (dlpath + "\\" + name[i]).c_str(), 0, nullptr);
		opener.open((dlpath + "\\" + name[i]).c_str());
		if (!opener.is_open())
		{
			cout << "Failed.\n";
			opener.close();
		}
		else
		{
			cout << "Done.\n";
			opener.close();
			++SuccessCount;
		}
	}
	return SuccessCount;
}

vector<string> GET_VARIABLES()
{
	//Coming soon: configure variables to support downloading on other Pokemon sites.
	map<string, int> flags;
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
		int i = current_line.find("=");
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

int main()
{
	
	cout << "Initializing...\n";
	
	//Gets our varibles in Variables.dat file.
	vector<string> data = GET_VARIABLES();
	
	cout << data[0] << endl <<data[1] << endl << data[2] << endl;
	
	string appdata = getenv("APPDATA");
	string tempfolder = appdata + "\\PokemonDownloader\\";
	string filename = "page.html";
	string homepage = data[0];
	
	system((string("rd /S /Q ") + tempfolder).c_str()); //Cleans up the old directory.
	
	CreateDirectory(tempfolder.c_str(), nullptr); //Create a new directory.
	
	cout << "Fetching data from Blissey's husband...\n";
	URLDownloadToFile(nullptr, homepage.c_str(), (tempfolder + filename).c_str(), 0, nullptr);
	
	opener.open((tempfolder + filename).c_str(), ios_base::binary);
	if (!opener.is_open())
	{
		cout << "Unable to download Web page.\n";
		return 1;
	}
	opener.close();
	
	cout << "Download finished.\n";
	cout << "Attempting to open data file...\n";
	
	ifstream html((tempfolder + filename).c_str()); //Stream to process .html file.
	
	cout << "Processing data file...\n";
	uint32_t count = 0;
	string current;
	vector<string> links(0);
	vector<string> names(0);
	vector<string> direct(0);

	links.push_back("Placeholder."); //Placeholder at 0 to make the index start at 1
	names.push_back("Placeholder."); 
	direct.push_back("Placeholder.");

	while (!html.eof())
	{
		getline(html, current);
		static int i;
		i = current.find(data[1]);
		if (i!=string::npos)
		{
			++count;
			static int j;
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
				cout << "Invalid link.\n";
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
			static int find_colon;
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
	for (int i = 1; i<links.size(); ++i)
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
		string directURL = processfile(tempfolder + episodename);
		cout << "Done.\n\n";
		direct.push_back(directURL);
		if (directURL != "INVALID FILE.") ++count;
	}
	cout << "\n--------------------------------------------\n";
	cout << count << " episodes ready to download.\n";
	cout << "Choose an option:\n";
	cout << "1. Download all.\n";
	cout << "2. Download a range of episodes.\n";
	cout << "3. Download specified episodes.\n";
	cout << "Your choice: ";
	int choice = 0;
	cin >> choice;
	if (!cin) cin.clear();
	while (cin.get() != '\n') continue;
	cout << "Enter the path of the downloaded files:\n";
	cout << "(Blank line to use default download path).\n";
	string dlpath;
	getline(cin, dlpath);
	if (!dlpath.size()) dlpath = data[2];
	count = 0;
	switch (choice)
	{
		case 1:
			count = Download(direct, names, 1, count, dlpath);
			break;
		case 2:
			cout << "Enter the first episode in your range: ";
			int start,stop;
			start = 0;
			stop = 0;
			cin >> start;
			cout << "Enter the last episode in your range: ";
			cin >> stop;
			if (!cin) cin.clear();
			while (cin.get() != '\n') continue;
			count = Download(direct, names, start, stop, dlpath);
			break;
		case 3:
			cout << "Enter the index of the episodes you want to download (q to quit):\n";
			vector<int> queue(0);
			int current;
			while (cin)
			{
				cin >> current;
				if (cin) queue.push_back(current);
			}
			cin.clear();
			while (cin.get() != '\n') continue;
			count = Download(direct, names, queue, dlpath);
			break;
	}
	cout << "\n--------------------------------------------\n";	
	cout << count << " files successfully downloaded.\n";
	return 0;
}
