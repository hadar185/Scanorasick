#include "Node.h"

Node::Node(uint8_t value) :
	m_value(value),
	m_fail(NULL)
{}

uint8_t Node::get_value()
{
	return m_value;
}

Node *Node::get_next(uint8_t value)
{
	std::map<uint8_t, Node*>::iterator it = m_nexts.find(value);
	if (it == m_nexts.end())
	{
		return NULL;
	}
	return it->second;
}

Node *Node::add_next(uint8_t value) {
	Node *next = new Node(value);
	m_nexts.insert({ value, next });
	return m_nexts.at(value);
}

void Node::print(int i)
{
	std::cout << m_value;
	std::map<uint8_t, Node*>::iterator it;
	for (it = m_nexts.begin(); it != m_nexts.end(); it++)
	{
		it->second->print(i + 1);
	}
}
