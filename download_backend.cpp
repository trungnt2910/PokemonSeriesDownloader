//download_backend.cpp -- Implementation of the function that does our downloading work...
#include "download_backend.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <map>

#include <Windows.h>

#include <urlmon.h>
#include <wininet.h>

//Borrow a few tools from the standard library
using std::cout;
using std::endl;
using std::map;

const char * DownloadError(const HRESULT hr)
{
	static const map<HRESULT, const char *> error_names =
	{
		{0x00000000, "The download completed successfully."},
		{0x80004004, "The download has been aborted."},
		{0x80004005, "Unspecified failure."},
		{0x8000FFFF, "Unexpected failure."},
		{0x80070005, "Access Denied. Blissey's husband does not want to share this video :("},
		{0x8007000E, "Out of memory. Please clean up your disk before continuing."},
		{0x800C0004, "Failed to connect to the Internet. Please check your Internet connection and try again."},
		{0x800C0005, "File not found. Please check your Internet connection and try again."},
		{0x800C0008, "The download of this episode has failed."},
		{0x800C000B, "The Internet connection has timed out."}
	};
	
	try
	{
		return error_names.at(hr);
	}
	//Unknown error
	catch (const std::out_of_range & a)
	{
		cout << "Exit code: 0x" << std::hex << hr << endl;
		return "Failure because of an unknown reason.";
	}
}

void LoadBar(unsigned curr_val, unsigned max_val, unsigned bar_width = 20)
{
    if((curr_val != max_val) && (curr_val % (max_val / 100) != 0))
        return;

    double   ratio   =  curr_val / (double)max_val;
    unsigned bar_now =  (unsigned)(ratio * bar_width);

    cout << ("\r") << std::setw(3) << (unsigned)(ratio * 100.0) << ("% [");
    for(unsigned curr_val = 0; curr_val < bar_now; ++curr_val)
        cout << ("=");
    for(unsigned curr_val = bar_now; curr_val < bar_width; ++curr_val)
        cout << (" ");
    cout << ("]") << std::flush;
}

class CallbackHandler : public IBindStatusCallback
{
private:
    int m_percentLast;
    bool v;

public:
    CallbackHandler(const bool verbose = false) : m_percentLast(0), v(0)
    {
    }

    // IUnknown

    HRESULT STDMETHODCALLTYPE
    QueryInterface(REFIID riid, void** ppvObject)
    {

        if(    IsEqualIID(IID_IBindStatusCallback, riid)
            || IsEqualIID(IID_IUnknown, riid) )
        {
            *ppvObject = reinterpret_cast<void*>(this);
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE
    AddRef()
    {
        return 2UL;
    }

    ULONG STDMETHODCALLTYPE
    Release()
    {
        return 1UL;
    }

    // IBindStatusCallback

    HRESULT STDMETHODCALLTYPE
    OnStartBinding(DWORD     /*dwReserved*/,
                   IBinding* /*pib*/)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE
    GetPriority(LONG* /*pnPriority*/)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE
    OnLowResource(DWORD /*reserved*/)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE
    OnProgress(ULONG   ulProgress,
               ULONG   ulProgressMax,
               ULONG   ulStatusCode,
               LPCWSTR /*szStatusText*/)
    {
        switch(ulStatusCode)
        {
            case BINDSTATUS_FINDINGRESOURCE:
            	if (v)
                cout << ("Finding resource...") << endl;
            break;
            case BINDSTATUS_CONNECTING:
            	if (v)
                cout << ("Connecting...") << endl;
            break;
            case BINDSTATUS_SENDINGREQUEST:
            	if (v)
                cout << ("Sending request...") << endl;
            break;
            case BINDSTATUS_MIMETYPEAVAILABLE:
            	if (v)
                cout << ("Mime type available") << endl;
            break;
            case BINDSTATUS_CACHEFILENAMEAVAILABLE:
            	if (v)
                cout << ("Cache filename available") << endl;
            break;
            case BINDSTATUS_BEGINDOWNLOADDATA:
            	if (v)
                cout << ("Begin download") << endl;
            break;
            case BINDSTATUS_DOWNLOADINGDATA:
            case BINDSTATUS_ENDDOWNLOADDATA:
            {
                int percent = (int)(100.0 * static_cast<double>(ulProgress)
                                / static_cast<double>(ulProgressMax));
                if(m_percentLast < percent)
                {
                    LoadBar(percent, 100);
                    m_percentLast = percent;
                }
                if(ulStatusCode == BINDSTATUS_ENDDOWNLOADDATA)
                {
                	if (v)
                    cout << endl << ("End download") << endl;
                }
            }
            break;

            default:
            {
            	if (v)
                cout << ("Status code : ") << ulStatusCode << endl;
            }
        }
        // The download can be cancelled by returning E_ABORT here
        // of from any other of the methods.
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE
    OnStopBinding(HRESULT /*hresult*/,
                  LPCWSTR /*szError*/)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE
    GetBindInfo(DWORD*    /*grfBINDF*/,
                BINDINFO* /*pbindinfo*/)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE
    OnDataAvailable(DWORD      /*grfBSCF*/,
                    DWORD      /*dwSize*/,
                    FORMATETC* /*pformatetc*/,
                    STGMEDIUM* /*pstgmed*/)
    {
        return E_NOTIMPL;
    }

    HRESULT STDMETHODCALLTYPE
    OnObjectAvailable(REFIID    /*riid*/,
                      IUnknown* /*punk*/)
    {
        return E_NOTIMPL;
    }
};

int32_t DownloadtoFile(const string & URL, const string & FilePath, const bool verbose)
{
	const char * url = URL.c_str();
	const char * filePath = FilePath.c_str();
	
	if (verbose)
    cout << "Downloading   : " << url      << endl
		 << "To local file : " << filePath << endl;

    // Invalidate cache, so file is always downloaded from web site
    // (if not called, the file will be retieved from the cache if
    // it's already been downloaded.)
    DeleteUrlCacheEntry(url);

    CallbackHandler callbackHandler(verbose);
    IBindStatusCallback* pBindStatusCallback = NULL;
    callbackHandler.QueryInterface(IID_IBindStatusCallback,
                                   reinterpret_cast<void**>(&pBindStatusCallback));

    HRESULT hr = URLDownloadToFile(
        NULL,   // A pointer to the controlling IUnknown interface
        url,
        filePath,
        0,      // Reserved. Must be set to 0.
        pBindStatusCallback );

	endl(cout); //Flush our buffers.
	
	cout << DownloadError(hr) << "\n" << endl;

    // should usually call Release on a COM object, but this one (callbackHandler)
    // was created on the stack so is going out of scope now and will die anyway.
    
    return hr;
}
