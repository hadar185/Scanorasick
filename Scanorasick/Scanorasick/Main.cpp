#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>

#include "Trie.h"
#include "File.h"
#include "Scanner.h"

enum class Commands : uint8_t
{
	INIT = 0,
	SCAN
};

enum ArgIndex
{
	COMMAND = 0,
	TARGET_FILE,
	DFA_FILE,

	ARGS_COUNT
};

int main(int argc, char* argv[])
{
	// 1. Command from user
	Commands cmd = Commands::INIT;

	// 2. Signature file
	//std::vector<uint8_t> signatures_buffer = { 'h', 'e', 'r', 'e', '\n', 't', 'h', 'e' };

	// 3. Parse(signatures_buffer) -> signatures_array
	//std::vector<std::vector<uint8_t>> signatures_array = { { 'h', 'e', 'r', 'e' }, { 't', 'h', 'e' }, {'h', 'e', 'y' } };

	/*
		* TODO:
		*	- Scanner class: construct, scan
		*	- Trie class
		*   - Node class
	*/

	File signatures_file("C://Users//hadar//Desktop//signaturesFile.txt");

	std::vector<uint8_t> signatures_buffer = signatures_file.get_content();
	std::vector<std::vector<uint8_t>> signatures_array = signatures_file.split_signatures(signatures_buffer);
	Trie trie;
	//for (std::vector<uint8_t>::const_iterator i = signatures_array[0].begin(); i != signatures_array[0].end(); ++i)
	//	std::cout << *i << ' ';

	std::vector<std::vector<uint8_t>>::iterator it;
	for (it = signatures_array.begin(); it != signatures_array.end(); it++)
	{
		trie.add_pattern(*it);
	}
	
	trie.add_backs();
	trie.print();

	File file_to_scan("C://Users//hadar//Desktop//FileToScan.txt");
	std::vector<uint8_t> content = file_to_scan.read_file(true);
	Scanner scanner;
	scanner.scan(content, trie);

	switch (cmd)
	{
	case Commands::INIT:

		break;

	case Commands::SCAN:
		break;

	default:
		std::cerr << "Usage..." << std::endl;
		break;
	}

	return 0;
}