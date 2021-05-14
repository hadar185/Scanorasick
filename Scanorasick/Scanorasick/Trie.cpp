#include "Trie.h"

Trie::Trie()
{
	Node::count = 0;
	m_root = new Node(5);
	m_root->set_fail(m_root);
}

Node* Trie::get_root()
{
	return m_root;
}

void Trie::add_pattern(std::vector<uint8_t> pattern)
{
	Node* current = m_root;
	while (pattern.size() > 0)
	{
		Node* next = current->get_next(pattern[0]);
		if (next != NULL)
		{
			current = next;
		}
		else
		{
			current = current->add_next(pattern[0]);
			current->set_fail(m_root);
		}

		pattern.erase(pattern.begin());
		if (pattern.size() == 0)
		{
			current->set_end(true);
		}
	}
}

void Trie::add_patterns(std::vector<std::vector<uint8_t>> patterns_array)
{
	std::vector<std::vector<uint8_t>>::iterator it;

	for (it = patterns_array.begin(); it != patterns_array.end(); it++)
	{
		add_pattern(*it);
	}
}

Node* Trie::search(std::vector<uint8_t> pattern)
{
	Node* current = m_root;
	while (current != NULL && pattern.size() > 0)
	{
		current = current->get_next(pattern[0]);
		pattern.erase(pattern.begin());
	}
	return current;
}

void Trie::add_backs()
{
	add_backs(m_root);
}

void Trie::add_backs(Node* node)
{
	std::vector<uint8_t> suffix = node->get_full_value();
	while (suffix.size() > 0)
	{
		suffix.erase(suffix.begin());
		Node* target_fail = search(suffix);
		if (target_fail != NULL)
		{
			node->set_fail(target_fail);
			break;
		}
		node->set_fail(m_root);
	}

	std::map<uint8_t, Node*> nexts = node->get_nexts();
	std::map<uint8_t, Node*>::iterator it;
	for (it = nexts.begin(); it != nexts.end(); it++)
	{
		add_backs(it->second);
	}
}

void Trie::print()
{
	m_root->print(0);
}

NodeStruct *Trie::serialize()
{
	NodeStruct *nodes_array = (NodeStruct *)malloc(Node::count * sizeof(NodeStruct));
	if (!nodes_array)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}

	// Create a queue for BFS
	std::list<Node*> queue;
	Node* temp;
	queue.push_back(m_root);
	while (!queue.empty())
	{
		temp = queue.back();
		queue.pop_back();

		std::cout << temp->get_value();
		nodes_array[temp->get_index()] = temp->serialize();

		std::map<uint8_t, Node*>::iterator it;
		std::map<uint8_t, Node*> nexts = temp->get_nexts();
		for (it = nexts.begin(); it != nexts.end(); it++)
		{
			queue.push_back(it->second);
		}
	}

	return nodes_array;
}

void Trie::deserialize(std::vector<NodeStruct> node_structs)
{
	Node **nodes = (Node**)malloc(node_structs.size() * sizeof(Node *));
	if (!nodes)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}

	nodes[0] = m_root;
	for (size_t i = 0; i < node_structs.size(); i++)
	{
		for (size_t j = 0; j < node_structs[i].next_amount; j++)
		{
			NodeStruct next = node_structs[node_structs[i].next_indexes[j]];
			nodes[next.index] = nodes[i]->add_next(next, nodes[next.fail_index]);
		}
	}
}
