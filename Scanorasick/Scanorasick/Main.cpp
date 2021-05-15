#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>

#include "Trie.h"
#include "Node.h"
#include "File.h"
#include "Scanner.h"

enum class Commands : uint8_t
{
	INIT = 0,
	SCAN
};

enum ArgIndex
{
	COMMAND = 1,
	TARGET_FILE,
	DFA_FILE,

	ARGS_COUNT
};

void write_dfa(Trie trie, std::string dfa_path)
{
	NodeStruct* ns = trie.serialize();
	std::ofstream file(dfa_path, std::ios::binary);

	for (size_t i = 0; i < trie.get_size(); i++)
	{
		file.write((const char*)(&ns[i]), sizeof(NodeStruct) - sizeof(int*));
		file.write((const char*)(ns[i].next_indexes), ns[i].next_amount * sizeof(int));
	}
	file.close();
}

std::vector<NodeStruct> read_dfa(std::string dfa_path)
{
	std::vector<NodeStruct> node_structs;
	std::ifstream file(dfa_path, std::ios::binary);
	if (!file.good())
	{
		std::cerr << "File does not exist" << std::endl;
	}

	while (!file.eof())
	{
		NodeStruct node;
		file.read(reinterpret_cast<char*>(&node), sizeof(NodeStruct) - sizeof(int*));

		node.next_indexes = (int*)malloc(node.next_amount * sizeof(int));
		if (!node.next_indexes)
		{
			std::cerr << "Memory Allocation Failed";
			exit(1);
		}
		file.read(reinterpret_cast<char*>(node.next_indexes), node.next_amount * sizeof(int));

		node_structs.push_back(node);
	}
	file.close();

	return node_structs;
}

void init(std::string signatures_path, std::string dfa_path)
{
	File signatures_file(signatures_path);
	std::vector<uint8_t> signatures_buffer;
	std::vector<std::vector<uint8_t>> signatures_array;
	Trie trie;
	signatures_buffer = signatures_file.get_content();
	signatures_array = signatures_file.split_signatures(signatures_buffer);

	trie.add_patterns(signatures_array);
	trie.add_backs();
	trie.print();

	write_dfa(trie, dfa_path);
}

void scan(std::string scan_path, std::string dfa_path)
{
	File file_to_scan(scan_path);
	std::vector<uint8_t> content = file_to_scan.read_file(true);

	Trie trie;
	trie.deserialize(read_dfa(dfa_path));
	trie.print();

	Scanner scanner;
	std::cout << "Found " << scanner.scan(content, trie) << " signatures of " << trie.get_patterns_amount();
}

int main(int argc, char* argv[])
{
	Commands cmd = (Commands)-1;

	if (argc != ArgIndex::ARGS_COUNT)
	{
		std::cerr << "init [signatures_file_path] [dfa_file_path]" << std::endl;
		std::cerr << "scan [file_to_scan_path] [dfa_file_path]" << std::endl;
		return 1;
	}

	if ((std::string)argv[ArgIndex::COMMAND] == "init")
	{
		cmd = Commands::INIT;
	}
	else if ((std::string)argv[ArgIndex::COMMAND] == "scan")
	{
		cmd = Commands::SCAN;
	}

	switch (cmd)
	{
	case Commands::INIT:
		init(argv[ArgIndex::TARGET_FILE], argv[ArgIndex::DFA_FILE]);
		break;

	case Commands::SCAN:
		scan(argv[ArgIndex::TARGET_FILE], argv[ArgIndex::DFA_FILE]);
		break;

	default:
		std::cerr << "init [signatures_file_path] [dfa_file_path]" << std::endl;
		std::cerr << "scan [file_to_scan_path] [dfa_file_path]" << std::endl;
		break;
	}

	return 0;
}
