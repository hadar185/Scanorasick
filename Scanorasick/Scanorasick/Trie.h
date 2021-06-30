#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <map>

#include "Node.h"

class Trie
{
private:
	//std::shared_ptr<Node> m_root;
	//Node *m_root;
	NodeUPtr m_root;
	int m_size;
	int m_patterns_amount;


private:
	void add_back_links(Node*);


// CR: Change some functions to private
public:
	Trie();
	virtual ~Trie() = default;
	// Trie(const Trie&) = default;

	Node* get_root();

	int get_size();

	int get_patterns_amount();

	void add_pattern(Buffer);
	void add_patterns(std::vector<Buffer>);
	Node* search(Buffer);
	void add_back_links();
	void print();

	std::unique_ptr<NodeStruct[]> serialize();
	void deserialize(std::vector<NodeStruct>);
};

