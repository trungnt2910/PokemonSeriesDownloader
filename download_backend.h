//download_backend.h -- Implementation of the function that does our downloading work...

#ifndef __POKEMON__DOWNLOADER__BACKEND__H__
#define __POEKMON__DOWNLOADER__BACKEND__H__

//Forward declarations
#include <iosfwd>

#include <stdint.h>

//Borrow a few tools from the standard library...
using std::string;

//Our function
int32_t DownloadtoFile(const string & URL, const string & FilePath, const bool verbose = false);

#endif //__POKEMON__DOWNlOADER__BACKEND__H__
