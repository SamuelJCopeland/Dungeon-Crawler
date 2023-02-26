/*
* Copyright: Samuel Copeland
* Date: 12/05/2022
*
* File: parser.h
*/

#ifndef PARSER_H
#define PARSER_H

#include<string>
#include<vector>

class  UnhandledEscapeCharacter : public std::exception
{
public:
	UnhandledEscapeCharacter(int aPosition)
	{
		mError = "Unhandled \\ at " + std::to_string(aPosition);
	}

	std::string what()
	{
		return mError;
	}

private:
	std::string mError;
};

class  Parser
{
public:

	// Prepends a given prefix to lines in a string
	static std::string prependLines(const std::string& aInput, const std::string& aPrefix);

	// This function will not handle \a, \b, or \f
	static std::string handleEscapeChars(const std::string& aInput);

	// Implementation of a function similar to the Python string.split() function
	static std::vector<std::string> split(const std::string& aS, const std::string& aDelimiter);

	// Looks for a substring in a string, if the substring is found, return the index of the start of that
	// substring in the string otherwise, return -1
	static int locateSubstr(const std::string& aBaseString, const std::string& aSubString, int aStart = 0);

	// Implements the functionality of Python subrange (e.g. s[:-1]) for strings
	static std::string getSubstringRange(const std::string& aS, int aStart, int aEnd);

	static std::vector<std::string> parseInput(const std::string aInput);
private:
	static void handleBackslash(int aI, const std::string& aInput, std::string& aOutput);
};

#endif