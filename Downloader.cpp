#include "downloader.h"
#include "helper.h"

extern vector<string> links;
extern vector<string> names;
extern vector<string> direct;
extern vector<string> data;

int16_t Download(const vector<string> & URL, const vector<string> & name, const int16_t begin, const int16_t end, string & dlpath)
{
	int16_t SuccessCount = 0;
	dlpath = StandardizePath(dlpath);
	PrepareDirectory(dlpath);
	cout << "Downloading to: " << dlpath << endl;
	for (int16_t i = begin; i <= end; ++i)
	{
		cout << "Downloading " << name[i] << endl;
		URLDownloadToFile(nullptr, URL[i].c_str(), (dlpath + "\\" + name[i]).c_str(), 0, nullptr);
		static bool status;
		status = checkExistence((dlpath + "\\" + name[i]).c_str());
		if (!status)
		{
			cout << "Failed.\n";
		}
		else 
		{
			cout << "Done.\n";
			++SuccessCount;
		}
	}
	return SuccessCount;
}

int16_t Download(const vector<string> & URL, const vector<string> & name, const vector<int16_t> & queue, const string dlpath)
{
	int16_t SuccessCount = 0;
	PrepareDirectory(dlpath);
	for (const auto & i : queue)
	{
		cout << "Downloading " << name[i] << endl;
		URLDownloadToFile(nullptr, URL[i].c_str(), (dlpath + "\\" + name[i]).c_str(), 0, nullptr);
		static bool status;
		status = checkExistence((dlpath + "\\" + name[i]).c_str());
		if (!status)
		{
			cout << "Failed.\n";
		}
		else
		{
			cout << "Done.\n";
			++SuccessCount;
		}
	}
	return SuccessCount;
}

void downloadInterface()
{
	cout << "Choose an option:\n";
	cout << "1. Download all.\n";
	cout << "2. Download a range of episodes.\n";
	cout << "3. Download specified episodes.\n";
	cout << "Your choice: ";
	int16_t choice = 0;
	cin >> choice;
	if (!cin) cin.clear();
	while (cin.get() != '\n') continue;
	cout << "Enter the path of the downloaded files:\n";
	cout << "(Blank line to use default download path).\n";
	string dlpath;
	getline(cin, dlpath);
	if (!dlpath.size()) dlpath = data[2];
	int16_t count = 0;
	switch (choice)
	{
		case 1:
			count = Download(direct, names, 1, count, dlpath);
			break;
		case 2:
			cout << "Enter the first episode in your range: ";
			int16_t start, stop;
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
			vector<int16_t> queue(0);
			int16_t current;
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
}
