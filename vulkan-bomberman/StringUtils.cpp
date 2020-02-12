#include "StringUtils.h"

bool cmp_str::operator()(char const *a, char const *b) const
{
	return std::strcmp(a, b) < 0;
}