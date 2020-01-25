#include <bits/stdc++.h>
#include <Windows.h> 
using namespace std;
#define ullong unsigned long long
#define llong long long
#define problemname ""
//ifstream fin(problemname".INP");
//ofstream fout(problemname".OUT");
ifstream opener;
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
	//	cerr << "created dir " << string(fullpath.begin(), fullpath.begin()+j).c_str() << endl;
		i = j + 1;
		j = i;
	}
//	cerr << "finished.\n"; 
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
	return link;
}
string process(string & element)
{
	int i = element.find("https:");
	int j = i;
	while ((element[j] != ';')) ++j;
	string result = string(element.begin() + i, element.begin() + j);
	if (string(result.end() - 4, result.end()) == "#038")
	{
		result = string(result.begin(), result.end() - 4);
		i = j + 1;
		j = i;
		while ((element[j] != ';')) ++j;
		result = result + string(element.begin() + i, element.begin() + j - 4);
		i = j + 1;
		j = i;
		while ((element[j] != ';')) ++j;
		result = result + string(element.begin() + i, element.begin() + j - 4);
	}
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
		}
		string data = process(current_line);
	//	cout << current_line << endl << endl;
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
	//implement later:
	//data[0]: Index page of the series.
	//data[1]: Prefix of each episode.
	return {
		"https://blisseyhusband.in/pokemon-the-series-sun-moon-ultra-legends/",
		"https://blisseyhusband.in/"
	};
}
int main()
{
	
	cout << "Initializing...\n";
	
	//Gets our varibles in Variables.dat file.
	vector<string> data = GET_VARIABLES();
	
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
			cout << "Link:\n";
			links.push_back(string(current.begin() + i, current.begin() + j));
			cout << links.back() << endl;
//			getline(html, current);
			i = current.find("Countdown");
			if (i != string::npos)
			{
				cout << "Episode coming soon!";
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
			cout << "Name:\n";
			j = i;
			if (count > 47) cout << "WTF?0;";
			while (current[j] != '<') ++j;
			string temp = string(current.begin() + i, current.begin() + j);
			static int find_colon;
			find_colon = temp.find(":");
			static string replace;
			replace = "";
			if (!isspace(temp[find_colon - 1])) replace += ' ';
			replace += '-';
			if (!isspace(temp[find_colon + 1])) replace += ' ';
			temp.replace(temp.begin() + find_colon, temp.begin() + find_colon + 1, replace);
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
		cout << "Done.\n";
		direct.push_back(directURL);
		++count;
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
	string dlpath;
	getline(cin, dlpath);
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
