#pragma once

#include <iostream>
#include <map>
#include <vector>
#include <list>

struct NodeStruct
{
	int index;
	int fail_index;
	uint8_t value;
	bool end;
	int next_amount;
	int* next_indexes;
};

class Node
{
private:
	uint8_t m_value;
	std::map<uint8_t, Node*> m_nexts;
	std::vector<uint8_t> m_full_value = {};
	Node* m_fail;
	bool m_end;
	int m_index;


public:
	Node(uint8_t, int index);
	Node(NodeStruct, Node *fail);

	uint8_t get_value();

	std::map<uint8_t, Node*> get_nexts();
	Node* get_next(uint8_t);
	Node* add_next(uint8_t, int);
	Node* add_next(NodeStruct, Node *);

	std::vector<uint8_t> get_full_value();
	void set_full_value(std::vector<uint8_t>);

	Node* get_fail();
	void set_fail(Node*);

	bool is_end();
	void set_end(bool);

	int get_index();

	void print(int);
	void print_full_value();

	NodeStruct serialize();
};

