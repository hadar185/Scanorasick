#pragma once

#include <iostream>
#include <vector>

#include "Trie.h"

class Scanner
{
public:
	int scan(std::vector<uint8_t> buffer, Trie trie);
};
