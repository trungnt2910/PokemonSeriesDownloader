#ifndef __POKEMON__DOWNLOADER__BACKEND__H__
#define __POEKMON__DOWNLOADER__BACKEND__H__

#define WIN32_LEAN_AND_MEAN
// some of these are defined in MinGw's specstrings.h, but not all...
#define QUERYCONTEXT void*

#include <Windows.h>

#include <iostream> // must be included before hack.h as SAL annotations
#include <iomanip>  // collide with GCC's standard library implmentation

#define __in
#define __in_opt
#define __out
#define __out_opt
#define __inout_opt
#define __inout
#define __deref_inout
#define __in_bcount_opt(c)
#define __reserved
#define __deref_out
#define __deref_out_opt

#define __out_bcount_part_opt(c, p)
#define __in_range(a, b)

#define __RPC__in
#define __RPC__in_opt
#define __RPC__out
#define __RPC__out_opt
#define __RPC__inout
#define __RPC__inout_opt
#define __RPC_unique_pointer
#define __RPC__deref_out_opt

#include <urlmon.h>

#include <wininet.h>

#include <string>

//Borrow a few tools from the standard library
using std::cout;
using std::endl;
using std::string;

//Our function
int32_t DownloadtoFile(const string & URL, const string & FilePath, const bool verbose = false);

#endif //__POKEMON__DOWNlOADER__BACKEND__H__
