#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>

#include "Trie.h"
#include "Node.h"
#include "File.h"
#include "Scanner.h"

enum class Commands
{
	INVALID_COMMAND = -1,
	INIT = 0,
	SCAN,
	HELP
};

enum ArgIndex
{
	COMMAND = 1,

	ARGS_COUNT
};

enum InitArgIndex
{
	SIGNATURES_FILE = 2,
	DFA_PATH,

	INIT_ARGS_COUNT
};

enum ScanArgIndex
{
	FILE_TO_SCAN = 2,
	DFA_FILE,
	OUTPUT_FILE,

	SCAN_ARGS_COUNT
};

void write_dfa(Trie& trie, std::string& dfa_path)
{
	auto ns = trie.serialize();
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
// -init SignaturesFile2.txt dfa2.txt
void init(std::string signatures_path, std::string dfa_path)
{
	File signatures_file(signatures_path);
	Buffer signatures_buffer = signatures_file.read_file(true);
	std::vector<Buffer> signatures_array = signatures_file.split_signatures(signatures_buffer);

	Trie trie;
	trie.add_patterns(signatures_array);
	trie.add_back_links();
	//trie.print();

	write_dfa(trie, dfa_path);
}

void scan(std::string scan_path, std::string dfa_path, std::string output_path = "")
{
	File file_to_scan(scan_path);
	Buffer content = file_to_scan.read_file(true);

	Trie trie;
	trie.deserialize(read_dfa(dfa_path));
	//trie.print();

	Scanner scanner;
	std::cout << "Found " << scanner.scan(content, trie, output_path) << " occurrences of " << trie.get_patterns_amount() << " signatures.";
}

void help()
{
	std::cout << "--help" << std::endl;
	std::cout << "--init [signatures_file_path] [dfa_file_path]" << std::endl;
	std::cout << "--scan [file_to_scan_path] [dfa_file_path]" << std::endl;
}

Commands parse_args(std::string arg)
{
	//(std::string)argv[ArgIndex::COMMAND]
	Commands cmd = Commands::INVALID_COMMAND;

	if (arg == "--init")
	{
		cmd = Commands::INIT;
	}
	else if (arg == "--scan")
	{
		cmd = Commands::SCAN;
	}
	else if (arg == "--help") {
		cmd = Commands::HELP;
	}

	return cmd;
}

void do_cmd(Commands cmd, int argc, char *argv[])
{
	// CR: No magic numbers

	switch (cmd)
	{
	case Commands::INIT:
		if (argc != 3)
		{
			init(argv[InitArgIndex::SIGNATURES_FILE], argv[InitArgIndex::DFA_PATH]);
			break;
		}

	case Commands::SCAN:
		if (argc >= 4 && argc <= 5)
		{
			std::string output_path = argc == ScanArgIndex::SCAN_ARGS_COUNT ? argv[ScanArgIndex::OUTPUT_FILE] : "";
			scan(argv[ScanArgIndex::FILE_TO_SCAN], argv[ScanArgIndex::DFA_FILE], output_path);
			break;
		}

	case Commands::HELP:
		help();
		break;

	default:
		std::cerr << "--help to get commands list" << std::endl;
		break;
	}
}

int main(int argc, char* argv[])
{
	if (argc <= 1)
	{
		std::cerr << "--help to get commands list" << std::endl;
		return 1;
	}
	Commands cmd = parse_args(argv[ArgIndex::COMMAND]);

	do_cmd(cmd, argc, argv);

	return 0;
}
