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

	for (size_t i = 0; i < Node::count; i++)
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
	scanner.scan(content, trie);
}

int main(int argc, char* argv[])
{
	// 1. Command from user
	Commands cmd = Commands::INIT;

	if (argc != ArgIndex::ARGS_COUNT)
	{
		std::cerr << "init [signatures_file_path] [dfa_file_path]" << std::endl;
		std::cerr << "scan [file_to_scan_path] [dfa_file_path]" << std::endl;
		return 1;
	}

	std::cout << argv[ArgIndex::COMMAND];

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

//// write vector v to text.txt
//void write(std::string path, NodeStruct ns) {
//	std::ofstream file;
//	file.open(path, std::ios::binary);
//	/*std::vector<std::string> v{ "one", "two", "three" };
//	for (const auto& e : v) file << e << "\n";
//	file.write(reinterpret_cast<const char*>(ns.next_indexes.data()), ns.next_indexes.size());*/
//	file << ns.value << std::endl;
//	file << ns.fail_index << std::endl;
//	file << ns.end << std::endl;
//	file << ns.index << std::endl;
//	file << 2 << " ";
//	for (int i = 0; i < 2; i++) {
//		file << ns.next_indexes[i] << ", ";
//	}
//	/*file << std::endl << 3;
//	for (int i = 0; i < 3; i++) {
//		file << ns.full_value[i] << " ";
//	}*/
//	//for (const auto& e : ns.next_indexes) file << e << ", ";
//	//file << std::endl;
//	//for (const auto& e : ns.full_value) file << e << ", ";
//	file.close();
//}
//
//// read data from from text.txt and store it in vector v
//void read(std::string path) {
//	std::ifstream file;
//	file.open(path);
//	std::string line;
//	//NodeStruct ns{};
//	while (getline(file, line)) {
//		//ns.next_indexes.push_back(line);
//	}
//	file.close();
//}

/*File signatures_file("SignaturesFile.txt");

	std::vector<uint8_t> signatures_buffer = signatures_file.get_content();
	std::vector<std::vector<uint8_t>> signatures_array = signatures_file.split_signatures(signatures_buffer);

	Trie trie;

	trie.add_patterns(signatures_array);
	trie.add_backs();
	trie.print();

	File file_to_scan("FileToScan.txt");
	std::vector<uint8_t> content = file_to_scan.read_file(true);

	Scanner scanner;
	scanner.scan(content, trie);*/

	/*int* nexts = (int *)malloc(2 * sizeof(int));
	if (!nexts)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}
	std::cout << "Initializing values..." << std::endl;

	for (int i = 0; i < 2; i++)
	{
		nexts[i] = i;
	}

	uint8_t* full = (uint8_t*)malloc(3 * sizeof(uint8_t));
	if (!full)
	{
		std::cout << "Memory Allocation Failed";
		exit(1);
	}
	std::cout << "Initializing values..." << std::endl;

	for (int i = 0; i < 3; i++)
	{
		full[i] = 'a' + i;
	}

	struct NodeStruct ns = { 'a', 0, false, 1, nexts};

	write("node.txt", ns);*/
	/*std::cout << sizeof(NodeStruct) - sizeof(int*);

	NodeStruct *ns = trie.serialize();

	std::ofstream file;
	file.open("node2.txt", std::ios::binary);
	for (size_t i = 0; i < Node::count; i++)
	{
		file.write(reinterpret_cast<const char*>(&ns[i]), sizeof(NodeStruct) - sizeof(int*));

		file.write((const char *)(ns[i].next_indexes), ns[i].next_amount * sizeof(int));
	}
	file.close();

	std::ifstream in_file;
	in_file.open("node2.txt", std::ios::binary);
	std::vector<NodeStruct> node_structs;
	while (!in_file.eof())
	{
		NodeStruct node;
		in_file.read(reinterpret_cast<char*>(&node), sizeof(NodeStruct) - sizeof(int*));
		std::cout << std::endl << node.next_amount << std::endl;
		node.next_indexes = (int*)malloc(node.next_amount * sizeof(int));
		if (!node.next_indexes)
		{
			std::cout << "Memory Allocation Failed";
			exit(1);
		}
		std::cout << "Initializing values..." << std::endl;
		//for (size_t i = 0; i < node.next_amount; i++)
		//{
		in_file.read(reinterpret_cast<char*>(node.next_indexes), node.next_amount * sizeof(int));
		//}

		node_structs.push_back(node);
	}
	in_file.close();

	Trie trie2;
	trie2.deserialize(node_structs);
	trie2.print();*/


	/*std::ofstream outFile("node.dat", std::ios::binary);
	outFile.write((char*)& ns, sizeof(ns));
	outFile.close();
	std::cout << sizeof(ns.next_indexes) << " " << sizeof(struct NodeStruct);

	File data("node.dat");
	std::ifstream inFile("node.dat", std::ios::binary);
	struct NodeStruct ns2;

	struct NodeStruct ns2 = (char*)data.read_file(true);
	inFile.read((char*)& ns2, sizeof(ns));
	std::cout << ns2.value;

	inFile.close();*/
