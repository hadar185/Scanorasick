#pragma once

#include <iostream>
#include <queue>
#include <map>

#include "Node.h"

class Trie
{
private:
	NodeUPtr m_root;
	int m_size;
	int m_patterns_amount;


private:
	void add_back_links(Node*);
	void add_pattern(Buffer);
	Node* search(Buffer);


public:
	Trie();
	virtual ~Trie() = default;
	// Trie(const Trie&) = default;

	Node* get_root();

	int get_size();

	int get_patterns_amount();

	void add_patterns(std::vector<Buffer>);
	void add_back_links();
	void print();

	std::unique_ptr<NodeStruct[]> serialize();
	void deserialize(std::vector<NodeStruct>);
};

