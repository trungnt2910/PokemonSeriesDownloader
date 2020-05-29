//main.cpp -- main() function of PokemonSeriesDownloader. Initializes resources, and call other interfaces.

#include <Windows.h> //For CreateDirectory

#include <iostream>

#include "downloader.h"
#include "fetcher.h"
#include "helper.h"
#include "variables.h"

//Borrow a few tools from the standard library
using std::cin;
using std::cout;
using std::string;
using std::vector;

vector<string> links(0);
vector<string> names(0);
vector<string> direct(0);

int main(int argc, char** argv) 
{
	cout << "Initializing...\n";
	
	string appdata = getenv("APPDATA");
	string tempfolder = appdata + "\\PokemonDownloader\\";
	string filename = "page.html";
	string homepage = environment["INDEX_PAGE"];

	system((string("rd /S /Q ") + tempfolder).c_str()); //Cleans up the old directory.

	CreateDirectory(tempfolder.c_str(), nullptr); //Create a new directory.
	
	cout << "Fetching data from Blissey's husband...\n";
	URLDownloadToFile(nullptr, homepage.c_str(), (tempfolder + filename).c_str(), 0, nullptr);

	if (!checkExistence(tempfolder + filename))
	{
		cout << "Unable to download web page.";
		return 1;
	}

	cout << "Download finished.\n";

	cout << "Attempting to open data file...\n";

	links.push_back("Placeholder."); //Placeholder at 0 to make the index start at 1
	names.push_back("Placeholder."); 
	direct.push_back("Placeholder.");
	
	processIndexPage(filename,tempfolder);
	
	downloadInterface();
	
	cout << "Press any key to exit...\n";
	cin.get();
	
	return 0;
}
