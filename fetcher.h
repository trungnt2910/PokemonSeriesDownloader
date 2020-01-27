#ifndef __POKEMON__FETCHER__H__
#define __POKEMON__FETCHER__H__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//Borrow a few tools from the standard library
using std::cout;
using std::endl;
using std::ifstream;
using std::string;
using std::stringstream;
using std::vector;

//string process(string & element);
//string processfile(const string & path);
void processIndexPage(const string & pagename, const string & tempfolder);

#endif
