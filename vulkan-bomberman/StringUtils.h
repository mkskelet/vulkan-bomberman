#pragma once

#include <string>

struct cmp_str
{
	bool operator()(char const *a, char const *b) const;
};