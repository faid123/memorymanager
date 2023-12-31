#pragma once

#include <wtypes.h>

class Hash
{
public:
	Hash() = default;
	~Hash() = default;
	Hash& operator=(const Hash& source) = delete;
	Hash(const Hash& source) = delete;
	Hash& operator=(Hash&&) = delete;
	Hash(const Hash&&) = delete;

	unsigned int hash(const void* key, BYTE length, const unsigned int modifier = 0);

private:
	void mix(unsigned int& a, unsigned int& b, unsigned int& c);
	unsigned int rot(unsigned int x, unsigned int k);
	void final(unsigned int& a, unsigned int& b, unsigned int& c);
};