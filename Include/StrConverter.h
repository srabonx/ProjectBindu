#ifndef STRCONVERTER_H
#define STRCONVERTER_H

#include <locale>
#include <sstream>
#include <string>

namespace  BINDU
{
	class StrConverter
	{
	private:

	public:

		static std::wstring getWideStr(const char* string)
		{
			std::wostringstream wstm;

			std::string str = string;

			const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());

			for (size_t i = 0; i < str.size(); ++i)
				wstm << ctfacet.widen(str[i]);

			return wstm.str();
		}

		static std::wstring getWideStr(const std::string& string)
		{
			std::wostringstream wstm;

			const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());

			for (size_t i = 0; i < string.size(); ++i)
				wstm << ctfacet.widen(string[i]);

			return wstm.str();
		}

	};
};
#endif // STRCONVERTER_H
