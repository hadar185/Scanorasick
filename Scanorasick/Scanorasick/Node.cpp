#include "Node.h"

int Node::count = 0;

Node::Node(uint8_t value) :
	m_value(value),
	m_fail(NULL),
	m_end(false),
	m_index(count)
{
	count++;
}

Node::Node(NodeStruct node_struct, Node *fail) :
	m_value(node_struct.value),
	m_fail(fail),
	m_end(node_struct.end),
	m_index(node_struct.index)
{
	count++;
}

uint8_t Node::get_value()
{
	return m_value;
}

std::map<uint8_t, Node*> Node::get_nexts()
{
	return m_nexts;
}

Node* Node::get_next(uint8_t value)
{
	std::map<uint8_t, Node*>::iterator it = m_nexts.find(value);
	if (it == m_nexts.end())
	{
		return NULL;
	}
	return it->second;
}

Node* Node::add_next(uint8_t value) {
	Node* next = new Node(value);
	std::vector<uint8_t> full_value = m_full_value;
	full_value.push_back(value);
	next->set_full_value(full_value);
	m_nexts.insert({ value, next });

	return m_nexts.at(value);
}

Node* Node::add_next(NodeStruct node_struct, Node *fail) {
	Node* next = new Node(node_struct, fail);
	std::vector<uint8_t> full_value = m_full_value;
	full_value.push_back(node_struct.value);
	next->set_full_value(full_value);
	m_nexts.insert({ node_struct.value, next });

	return m_nexts.at(node_struct.value);
}

std::vector<uint8_t> Node::get_full_value()
{
	return m_full_value;
}

void Node::set_full_value(std::vector<uint8_t> value)
{
	m_full_value = value;
}

Node* Node::get_fail()
{
	return m_fail;
}

void Node::set_fail(Node* fail)
{
	m_fail = fail;
}

bool Node::is_end()
{
	return m_end;
}

void Node::set_end(bool end)
{
	m_end = end;
}

int Node::get_index()
{
	return m_index;
}

void Node::print(int i)
{
	if (m_fail != NULL)
	{
		std::cout << "value: " << m_value << ", index: " << m_index << ", fail_index: " << m_fail->m_index << ", end: " << m_end << ", full: ";
		print_full_value();
	}

	std::map<uint8_t, Node*>::iterator it;
	for (it = m_nexts.begin(); it != m_nexts.end(); it++)
	{
		it->second->print(i + 1);
	}
}

void Node::print_full_value()
{
	for (std::vector<uint8_t>::const_iterator i = m_full_value.begin(); i != m_full_value.end(); ++i)
	{
		std::cout << *i;
	}
	std::cout << std::endl;
}

NodeStruct Node::serialize()
{
	int length = m_nexts.size();
	int* nexts = (int*)malloc(length * sizeof(int));
	if (!nexts)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}

	std::map<uint8_t, Node*>::iterator it;
	int i = 0;
	for (it = m_nexts.begin(); it != m_nexts.end(); it++)
	{
		nexts[i] = it->second->m_index;
		++i;
	}

	return { m_index, m_fail->m_index, m_value, m_end, length, nexts };

}

/*int Node::Serialize(std::vector<NodeStruct> nodes_array)
{
	//std::vector<NodeStruct> nodes_array;
	std::map<uint8_t, Node*>::iterator it;
	if (m_end)
	{
		int* nexts = NULL;
		nodes_array.push_back({ m_value, 0, m_end, m_index, nexts });
		return nodes_array.size() - 1;
	}
	int* nexts = (int*)malloc(m_nexts.size() * sizeof(int));
	int i = 0;
	for (it = m_nexts.begin(); it != m_nexts.end(); it++)
	{
		nexts[i] = it->second->Serialize(nodes_array);
		++i;
	}
	nodes_array.push_back({ m_value, 0, m_end, m_index, nexts });
	return nodes_array.size() - 1;
}*/

//NodeStruct Node::Deserialize()
//{
//	int* nexts = (int*)malloc(m_nexts.size() * sizeof(int));
//
//	return { m_value, 0, m_end, m_index, nexts };
//}
