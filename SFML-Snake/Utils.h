#pragma once

#include <string>

class Utils
{
private:
	Utils();

public:
	static std::string GetFormattedNumericString(const std::string& string, int textLength);
};