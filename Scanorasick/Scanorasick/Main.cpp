#include <iostream>
#include <cstdint>
#include <vector>
#include <fstream>
#include <iterator>

#include "Trie.h"

enum class Commands : uint8_t
{
	INIT = 0,
	SCAN
};

std::vector<uint8_t> read_file(std::string path)
{
	std::ifstream file(path, std::ios::binary);

	if (!file.good())
	{
		std::cerr << "File does not exist" << std::endl;
	}

	// Leave new lines in binary mode
	file.unsetf(std::ios::skipws);

	std::vector<uint8_t> bytes((std::istream_iterator<uint8_t>(file)),
		std::istream_iterator<uint8_t>());

	std::cout << "Read " << bytes.size() << " numbers" << std::endl;
	for (std::vector<uint8_t>::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
		std::cout << *i << ' ';

	return bytes;
}

std::vector<std::vector<uint8_t>> split_signatures(std::vector<uint8_t> bytes)
{
	std::vector<std::vector<uint8_t>> signatures = { {} };
	int index = 0;

	for (std::vector<uint8_t>::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
	{
		if (*i == '\n') {
			++index;
			signatures.push_back({});
			std::cout << "Line break found";
		}
		signatures[index].push_back(*i);
	}
	return signatures;
}

int main(int argc, char* argv[])
{
	std::cout << "Hello" << std::endl;

	// 1. Command from user
	Commands cmd = Commands::INIT;

	// 2. Signature file
	std::vector<uint8_t> signatures_buffer = { 'h', 'e', 'r', 'e', '\n', 't', 'h', 'e' };

	// 3. Parse(signatures_buffer) -> signatures_array
	std::vector<std::vector<uint8_t>> signatures_array = { { 'h', 'e', 'r', 'e' }, { 't', 'h', 'e' }, {'h', 'e', 'y' } };

	/*
		* TODO:
		*	- Scanner class: construct, scan
		*	- Trie class
		*   - Node class
	*/
	signatures_buffer = read_file("C://Users//hadar//Desktop//signaturesFile.txt");
	signatures_array = split_signatures(signatures_buffer);
	Trie trie;
	//for (std::vector<uint8_t>::const_iterator i = signatures_array[0].begin(); i != signatures_array[0].end(); ++i)
	//	std::cout << *i << ' ';
	trie.add_pattern(signatures_array[0]);
	trie.add_pattern(signatures_array[1]);
	trie.add_pattern(signatures_array[2]);
	trie.print();
	

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