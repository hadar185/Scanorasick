#include <memory>

#include "Trie.h"

// TO DO: Change everywhere m_root to std::unique_ptr

Trie::Trie() : m_patterns_amount(0), m_size(0)
{
	//m_root = std::make_shared<Node>();
	m_root = new Node();

	// Increase amount of nodes in trie
	++m_size;
	m_root->set_fail_link(m_root);
}

Node* Trie::get_root()
{
	return m_root;
}

int Trie::get_size()
{
	return m_size;
}

int Trie::get_patterns_amount()
{
	return m_patterns_amount;
}

void Trie::add_pattern(Buffer pattern)
{
	Node* current = m_root;
	for (int i = 0; i < pattern.size(); ++i)
	{
		Node* next = current->get_next(pattern[i]);
		if (next != NULL)
		{
			current = next;
		}
		else
		{
			current = current->add_next(pattern[i], m_size);
			++m_size;
			current->set_fail_link(m_root);
		}

		if (i == pattern.size()-1 && !current->is_match())
		{
			current->set_match(true);
			++m_patterns_amount;
		}
	}
}

void Trie::add_patterns(std::vector<Buffer> patterns_array)
{
	std::vector<Buffer>::iterator it;

	for (it = patterns_array.begin(); it != patterns_array.end(); it++)
	{
		add_pattern(*it);
	}
}

Node* Trie::search(Buffer pattern)
{
	Node* current = m_root;
	for (int i = 0; i < pattern.size(); ++i)
	{
		current = current->get_next(pattern[i]);
		if (current == NULL)
		{
			break;
		}
	}
	return current;
}

void Trie::add_back_links()
{
	add_back_links(m_root);
}

void Trie::add_back_links(Node* node)
{
	Buffer suffix = node->get_full_value();
	node->set_fail_link(m_root);

	while (suffix.size() > 0)
	{
		// TO DO: Don't erase
		suffix.erase(suffix.begin());
		Node* target_fail = search(suffix);
		if (target_fail != NULL)
		{
			node->set_fail_link(target_fail);
			break;
		}
	}

	//std::map<uint8_t, std::unique_ptr<Node>> nexts = std::move(node->get_nexts());
	std::map<uint8_t, Node *> nexts = node->get_nexts();
	for (const auto& [key, value] : nexts)
	{
		add_back_links(value);
	}
}

void Trie::print()
{
	m_root->print(0);
}

std::unique_ptr<NodeStruct[]> Trie::serialize()
{
	// TO DO: Use managed pointers!
	auto nodes_array = std::make_unique<NodeStruct[]>(m_size);
	//NodeStruct *nodes_array = (NodeStruct *)malloc(m_size * sizeof(NodeStruct));
	if (!nodes_array)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}

	Node* current_node;

	// Create a queue for BFS
	std::list<Node*> queue;
	queue.push_back(m_root);

	while (!queue.empty())
	{
		current_node = queue.front();
		queue.pop_front();

		//std::cout << current_node->get_value();
		nodes_array[current_node->get_index()] = current_node->serialize();

		std::map<uint8_t, Node *> nexts = current_node->get_nexts();
		for (const auto& [key, value] : nexts)
		{
			queue.push_back(value);
		}
	}

	return nodes_array;
}

void Trie::deserialize(std::vector<NodeStruct> node_structs)
{
	// TO DO: Use managed pointers!
	Node **nodes = (Node**)malloc(node_structs.size() * sizeof(Node *));
	if (!nodes)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}

	nodes[0] = m_root;
	for (auto i = 0; i < node_structs.size(); i++)
	{
		for (auto j = 0; j < node_structs[i].next_amount; j++)
		{
			NodeStruct next = node_structs[node_structs[i].next_indexes[j]];
			nodes[next.index] = nodes[i]->add_next(next, nodes[next.fail_index]);
			++m_size;
			if (next.end)
			{
				++m_patterns_amount;
			}
		}
	}
	for (auto i = 0; i < node_structs.size(); i++)
	{
		nodes[node_structs[i].index]->set_fail_link(nodes[node_structs[node_structs[i].index].fail_index]);
	}
}
