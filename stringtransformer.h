//stringtransformer.h -- Class for processing URLs and much much more
#ifndef __POKEMON__STRINGTRANSFORMER__H__
#define __POKEMON__STRINGTRANSFORMER__H__

//Forward declarations...
#include <iosfwd>

#include <string>
#include <functional>
#include <map>

using std::string;

class StringTransformer
{
	private:
		
		using argument_t = std::vector<std::reference_wrapper<const string>>;
		using function_t = string(const argument_t &);
		
		class StringProcessorFunction;
		
		//Map to allow access to our private functions:
		static const std::map<string, StringProcessorFunction> FunctionMap;
		
		class StringProcessorFunction
		{
			private:
				function_t * funct;
				//ToDo: Function chains
				std::vector<const StringProcessorFunction *> nextFuncts;
				string apply(const argument_t & argv) const;
			public:
				StringProcessorFunction(const function_t & f) : funct(f) {};
				template <typename ... Args>
				string operator()(const Args &... args)
				{
					return apply({{(const string &)args...}});
				}
				template <typename ... Args>
				string operator()(const Args &... args) const
				{
					return funct({{(const string &)args...}});
				}
				StringProcessorFunction & operator[](const string & command)
				{
					nextFuncts.push_back(&FunctionMap.at(tolower({command})));
					return (*this);
				}
		};
		
		//Functions:
		static string escape(const argument_t & argv);
		static string escapeURL(const argument_t & argv);
		static string purifyURL(const argument_t & argv);
		static string removeIllegalFilenameCharacters(const argument_t & argv);
		static string tolower(const argument_t & argv);
		static string findAndReplaceAll(const argument_t & argv);
		static int ishexadecimal(int c)
		{
			if (('0' <= c) && (c <= '9'))
			{
				return c - '0';
			}
			if (('A' <= c) && (c <= 'F'))
			{
				return '9' + 1 + (c - 'A');
			}
			if (('a' <= c) && (c <= 'f'))
			{
				return '9' + ('F' - 'A') + 1 + (c - 'a');
			}
			return 0;
		}
		static int isoctal(int c)
		{
			if (('0' <= c) && (c <= '7'))
			{
				return c - '0';
			}
			else return 0;
		}
	public:
		StringProcessorFunction operator[](const string & command) const
		{
			return FunctionMap.at(tolower({command}));
		}
};

extern StringTransformer Transform;

#endif
