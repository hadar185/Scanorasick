#pragma once

#include <iostream>
#include <vector>
#include <map>

#include "Node.h"

class Trie
{
private:
	Node* m_root;

	void add_backs(Node*);

public:
	Trie();

	Node* get_root();
	void add_pattern(std::vector<uint8_t>);
	void add_patterns(std::vector<std::vector<uint8_t>>);
	Node* search(std::vector<uint8_t>);
	void add_backs();
	void print();

	NodeStruct *serialize();
	void deserialize(std::vector<NodeStruct>);
};

