#include <cstdio>
#include <string>

#include "index.h"
const int CHECK_NOT_PASSED = 1;
const int CHECK_PASSED = 0;
const int PROGRAM_FAILED = 2;
const char* ERROR1 = "Out of filename length\n";
const char* ERROR2 = "Bad parameters\n";
const char* PASS = "SUCCESS\n";
const char* NO_PASS = "FAIL\n";

int main(int argc, char *argv[])
{
	if (argc != 3) 
	{
		printf(ERROR2);
		return PROGRAM_FAILED;
	}
	if (argv[1] == NULL || argv[2] == NULL)
	{
		printf(ERROR2);
		return PROGRAM_FAILED;
	}
	

	std::string filename = argv[1];
	std::string pattern = argv[2];

	for (unsigned int i = 0; i < filename.length(); i++)
	{
		if ((int)filename.at(i) < 33 || (int)filename.at(i) > 126)
		{
			printf(ERROR2);
			return PROGRAM_FAILED;
		}
		if (filename.at(i) == '?' || filename.at(i) == '*')
		{
			printf(ERROR2);
			return PROGRAM_FAILED;
		}
		if (filename.at(i) == '\\')
		{
			filename.at(i) = '/';
		}
	}

	for (unsigned int i = 0; i < pattern.length(); i++)
	{
		if ((int)pattern.at(i) < 32 || (int)pattern.at(i) > 126)
		{
			printf(ERROR2);
			return PROGRAM_FAILED;
		}
		if (pattern.at(i) == '\\')
		{
			pattern.at(i) = '/';
		}
	}

	int position = 0;
	int star_position = -1;
	int double_star_position = -1;
	int star_shift = 0;

	for (unsigned int i = 0; i < pattern.length(); i++)
	{
		if (pattern.at(i) == '*')
		{
			star_position = i;
			star_shift++;

			if (i + 1 < pattern.length() && pattern.at(i + 1) == '*')
			{
				star_shift++;
				i++;
				double_star_position = i;
				star_position = i;
			}
			continue;
		}
		if (position + i - star_shift >= filename.length())
		{
			printf(NO_PASS);
			return CHECK_NOT_PASSED;
		}
		if (filename.at(position + i - star_shift) != pattern.at(i)) 
		{
			if (!isSlash(filename, position + i - star_shift))
			{
				if (pattern.at(i) == '?')
				{
					continue;
				}

				if (star_position != -1 && !isSlash(filename, position + star_position - star_shift + 1))
				{
					position++;
					i = star_position;
					continue;
				}
			}

			if (double_star_position == -1)
			{
				printf(NO_PASS);
				return CHECK_NOT_PASSED;
			}
			else
			{
				position++;
				i = double_star_position;
				continue;
			}	
		}
	}

	if (double_star_position == pattern.length() - 1)
	{
		printf(PASS);
		return CHECK_PASSED;
	}
	if (star_position != pattern.length() - 1 && filename.length() > pattern.length() + position - star_shift)
	{
		printf(NO_PASS);
		return CHECK_NOT_PASSED;
	}
	
	for (unsigned int i = pattern.length() + position - star_shift; i < filename.length(); i++)
	{
		if (isSlash(filename, i)) 
		{
			printf(NO_PASS);
			return CHECK_NOT_PASSED;
		}
	}

	printf(PASS);
	return CHECK_PASSED;
}

bool isSlash(std::string filename, int position)
{
	if (position >= filename.length())
	{
		printf("Out of filename length");
		exit(PROGRAM_FAILED);
	}
	return (filename.at(position) == '/');
}