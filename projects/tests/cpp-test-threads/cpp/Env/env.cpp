#include <iostream>
#include <string>
#include "env.h"

bool	IsExisting(const char* const szEnvVarName)
{
	size_t requiredSize;
	getenv_s(&requiredSize, NULL, 0, szEnvVarName);
	return (requiredSize != 0);
}

size_t	GetEnvVarLen(const char* const szEnvVarName)
{
	size_t requiredSize;
	getenv_s(&requiredSize, NULL, 0, szEnvVarName);
	return requiredSize;
}

char*	GetEnv(const char* const szEnvVarName, char* const buf, const size_t buflen)
{
	size_t requiredSize = GetEnvVarLen(szEnvVarName);
	if (0 == requiredSize)	{
		std::cerr << szEnvVarName << " doesn't exist!\n";
		return NULL;
	}
	if (buflen < requiredSize) {
		std::cerr << szEnvVarName << " value of " << requiredSize << " chars exceeds max characters limit of " << buflen << ". Truncating value!\n";
		requiredSize = buflen;
	}

	if (ERANGE == getenv_s(&requiredSize, buf, requiredSize, szEnvVarName))
	{
		char* foo = (char*) malloc(requiredSize*sizeof(char));
		if (NULL == foo) {
			return NULL;
		}
		else {
			getenv_s(&requiredSize, foo, requiredSize, szEnvVarName);
			strncpy_s(buf, buflen-1, foo, _TRUNCATE);
			buf[buflen - 1] = '\0';
			free (foo);
		}
	}
	return buf;
}

// NOTE: Returns error only if either of the args are zero!
// NOTE: Not threadsafe! Wrap in mutex on multithreaded systems!
void	PutEnv(const char* const szEnvVarName, const char* const szEnvVarNameValue)
{
	// Attempt to change path. Note that this only affects
	// the environment variable of the current process. The command
	// processor's environment is not changed.
	_putenv_s(szEnvVarName, szEnvVarNameValue);
}
