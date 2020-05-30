#include "stringtransformer.h"

#include <cctype>
#include <sstream>

using std::stringstream;

//Private StringProcessorFunction Functor class implementation:
string StringTransformer::StringProcessorFunction::apply(const argument_t & argv) const
{
	argument_t tempArgv = argv;
						
	string resultingString = funct(tempArgv);
		
	//First in, first out;
	for (const auto & toProcess : nextFuncts)
	{
		tempArgv[0] = resultingString;
		resultingString = toProcess->funct(tempArgv);
	}
		
	return resultingString;
}

//Private functions implementation:
string StringTransformer::escape(const argument_t & argv)
{
	const string & s = argv.at(0);	
	
	string result;
	result.reserve(s.size());
	
	for (std::size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] == '\\')
		{
			std::size_t escape_size = 1;
			switch (s[i + 1])
			{
				case 'n':
					result.push_back('\n');
				break;
				case 't':
					result.push_back('\t');
				break;
				case 'v':
					result.push_back('\v');
				break;
				case 'b':
					result.push_back('\b');
				break;
				case 'r':
					result.push_back('\r');
				break;
				case 'a':
					result.push_back('\a');
				break;
				case '?':
					result.push_back('\?');
				break;
				case '\'':
					result.push_back('\'');
				break;
				case '\"':
					result.push_back('\"');
				break;
				case '\\':
					result.push_back('\\');
				break;
				default:
					std::size_t begin = i + 1;
					std::size_t end = begin;
					if (s[begin] == 'x')
					{
						++begin; end = begin;
						while ((end < s.size()) && (ishexadecimal(s[end])))
						{
							++end;
						}
						stringstream ss(string(s.begin() + begin, s.begin() + end));
						char temp;
						ss >> std::hex >> (int &)temp;
						result.push_back(temp);
						escape_size = end - begin + 1;
						break;
					}
					else if (std::isdigit(s[begin]))
					{
						while ((end < s.size()) && (isoctal(s[end])))
						{
							++end;
						}
						stringstream ss(string(s.begin() + begin, s.begin() + end));
						char temp;
						ss >> std::oct >> (int &)temp;
						result.push_back(temp);
						escape_size = end - begin;
						break;
					}
					else escape_size = 0;
				break;
			}
			i += escape_size;
		}
		else result.push_back(s[i]);
	}
	return result;
}

string StringTransformer::tolower(const argument_t & argv)
{
	const string & s = argv.at(0);
	string result;
	result.reserve(s.size());
	for (const auto & x : s)
	{
		result.push_back(std::tolower(x));
	}
	return result;
}

string StringTransformer::escapeURL(const argument_t & argv)
{
	const string & s = argv.at(0);
	string link;
	link.reserve(s.size());
	for (std::size_t i = 0; i < s.size(); ++i)
	{
		//Detects '%' character
		if (s[i] == '%')
		{
			//Put encoded value in a stringstream
			stringstream toEscape(string(s.begin() + i + 1, s.begin() + i + 3));
			char temp;
			//Convert it to an int, then put it in temp;
			toEscape >> std::hex >> (int &)temp;
			link.push_back(temp);
			i += 2;
		}
		else link.push_back(s[i]);
	}
	return link;
}

string StringTransformer::purifyURL(const argument_t & argv)
{
	const string & s = argv.at(0);
	string link;
	link.reserve(s.size());
	for (std::size_t i = 0; i < s.size(); ++i)
	{
		if (s[i] == '\\') continue;
		link.push_back(s[i]);	
	}
	//Removes the stupid escape sequence
	if (link.back() == ';' && std::isdigit(*(link.end() - 2)))
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

string StringTransformer::removeIllegalFilenameCharacters(const argument_t & argv)
{
	const string & s = argv.at(0);
	string filename;
	filename.reserve(s.size());
	static const string illegalCharacters = "<>:\"/\\|?*";
	for (const auto & x : s)
	{
		if (illegalCharacters.find(x) != string::npos) continue;
		filename.push_back(x);
	}
	return filename;
}

string StringTransformer::findAndReplaceAll(const argument_t & argv)
{
	const string & s = argv.at(0);
	const string & toFind = argv.at(1);
	const string & replaceStr = argv.at(2);
	
	string result = "";
	result.reserve(s.size());
	// Get the first occurrence
	std::size_t lastPos = 0;
	std::size_t pos = s.find(toFind);
	
	// Repeat till end is reached
	while (pos != string::npos)
	{
		//Add any part of the string that is not the substring:
		result += string(s.begin() + lastPos, s.begin() + pos);
		//Add the replaced part:
		result += replaceStr;
		//Save the last position
		lastPos = pos + toFind.size();
		// Get the next occurrence from the current position
		pos = s.find(toFind, pos + toFind.size());
	}
	result += string(s.begin() + lastPos, s.end());
		
	return result;
}

const std::map<string, StringTransformer::StringProcessorFunction> StringTransformer::FunctionMap =
{
	{"escape", (escape)},
	{"escapeurl", (escapeURL)},
	{"purifyurl", (purifyURL)},
	{"removeillegalfilenamecharacters", (removeIllegalFilenameCharacters)},
	{"tolower", (tolower)},
	{"findandreplaceall", (findAndReplaceAll)}
};

//Static instance of the class that all other files uses.
StringTransformer Transform = StringTransformer();

