#pragma once

#include <windows.h>

// http://www.unknowncheats.me/forum/c-and-c/77419-findpattern.html#post650040
// Original code by learn_more
// Fix based on suggestion from stevemk14ebr : http://www.unknowncheats.me/forum/1056782-post13.html

#define INRANGE(x,a,b)		(x >= a && x <= b)
#define getBits( x )		(INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x )		(getBits(x[0]) << 4 | getBits(x[1]))

PBYTE FindPattern(const PBYTE rangeStart, const PBYTE rangeEnd, const char* pattern)
{
	const unsigned char* pat = reinterpret_cast<const unsigned char*>(pattern);
	PBYTE firstMatch = 0;
	for (PBYTE pCur = rangeStart; pCur < rangeEnd; ++pCur) {
		if (*(PBYTE)pat == (BYTE)'\?' || *pCur == getByte(pat)) {
			if (!firstMatch) {
				firstMatch = pCur;
			}
			pat += (*(PWORD)pat == (WORD)'\?\?' || *(PBYTE)pat != (BYTE)'\?') ? 2 : 1;
			if (!*pat) {
				return firstMatch;
			}
			pat++;
			if (!*pat) {
				return firstMatch;
			}
		} else if (firstMatch) {
			pCur = firstMatch;
			pat = reinterpret_cast<const unsigned char*>(pattern);
			firstMatch = 0;
		}
	}
	return NULL;
}