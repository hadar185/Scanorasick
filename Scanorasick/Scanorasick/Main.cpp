#include <iostream>
#include <cstdint>
#include <vector>

enum class Commands : uint8_t
{
	INIT = 0,
	SCAN
};

int main(int argc, char* argv[])
{
	std::cout << "Hello" << std::endl;

	// 1. Command from user
	Commands cmd = Commands::INIT;

	// 2. Signature file
	std::vector<uint8_t> signatures_buffer = { 'h', 'e', 'r', 'e', '\n', 't', 'h', 'e' };

	// 3. Parse(signatures_buffer) -> signatures_array
	std::vector<uint8_t*> signatures_array = { "here", "the" };

	/*
		* TODO:
		*	- Scanner class: construct, scan
		*	- Trie class
		*   - Node class
	*/

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