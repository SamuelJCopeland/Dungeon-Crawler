#include <vector>
#include <string>

using namespace std;

#ifndef PARSER_H
#define PARSER_H

class Parser {
public:
	//Implementation of a function similar to the Python string.split() function
	static vector<string> split(string s, string delimiter) {
		vector<string> result;
		int i = 0;
		while (i < s.size()) {
			int splitIndex = locateSubstr(s, delimiter, i);
			if (splitIndex > -1) {
				result.push_back(getSubstringRange(s, i, splitIndex));
				i = splitIndex + delimiter.size();
			}
			else {
				result.push_back(getSubstringRange(s, i, -1));
				break;
			}
		}

		return result;

	}
	
	//Looks for a substring in a string, if the substring is found, return the endex of the start of that substring in the string
	//otherwise, return -1
	static int locateSubstr(string baseString, string subString, int start = 0) {
		for (int i = start; i < baseString.size() - subString.size()+1; i++) {
			bool match = true;
			for (int j = 0; j < subString.size(); j++) {
				if (baseString[i+j] != subString[j]) {
					match = false;
					break;
				}
			}
			if (match) {
				return i;
			}
		}
		return -1;
	}

	//Implements the functionality of Python subrange (e.g. s[:-1]) for strings
	static string getSubstringRange(string s, int start, int end) {
		string result = "";			

		if (end < 0) {
			end = s.size() + end + 1;
		}
		else if (end > s.size()) {
			return "";
		}

		for (int i = start; i < end; i++) {
			result += s[i];
		}
		return result;
	}
};


#endif