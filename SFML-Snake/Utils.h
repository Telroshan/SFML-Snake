#pragma once

#include <string>

// A utility class
class Utils
{
private:
	Utils();

public:
	// Converts an integer to a string with fixed text length (3 => 003 for example)
	static std::string GetFormattedNumericString(const std::string& string, int textLength);
};