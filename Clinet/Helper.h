#pragma once
class Helper
{

public:
	static std::string Tostring(wstring value);
	static std::wstring ToWstring(string value);
	static void Replace(OUT string& str, string comp, string rep);
	static void Replace(OUT wstring& str, wstring comp, wstring rep);

};

