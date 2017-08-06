#pragma once

#include <string>

#define ENV_VALUE_MAX_CHARS		256

// NOTE: For hardcoded values remember paths must contain double backslash directory separators
char*	GetEnv(const char* const szEnvVarName, char* const buf, const size_t buflen);
void	PutEnv(const char* const szEnvVarName, const char* const szEnvVarNameValue);
bool	IsExisting(const char* const szEnvVarName);
