#include "Utils.h"

std::string Utils::GetFormattedNumericString(const std::string& string, int textLength)
{
	std::string text = string;
	if (text.length() > textLength)
	{
		text = text.substr(0, textLength);
	}
	while (text.length() < textLength)
	{
		text = "0" + text;
	}
	return text;
}
