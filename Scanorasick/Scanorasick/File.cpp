#include "File.h"

#include <iterator>

File::File(std::string path) :
	m_path(path)
{}

std::vector<uint8_t> File::read_file(bool readSpaces=true)
{
	std::ifstream file(m_path, std::ios::binary);

	if (!file.good())
	{
		std::cerr << "File does not exist" << std::endl;
	}

	// Leave new lines in binary mode
	if (readSpaces)
	{
		file.unsetf(std::ios::skipws);
	}

	std::vector<uint8_t> bytes((std::istream_iterator<uint8_t>(file)),
		std::istream_iterator<uint8_t>());

	std::cout << "Read " << bytes.size() << " bytes from file" << std::endl;

	return bytes;
}

std::vector<std::vector<uint8_t>> File::split_signatures(std::vector<uint8_t> bytes)
{
	std::vector<std::vector<uint8_t>> signatures = { {} };
	int index = 0;

	for (std::vector<uint8_t>::const_iterator i = bytes.begin(); i != bytes.end(); ++i)
	{
		if (*i == '\n') {
			++index;
			signatures.push_back({});
			std::cout << "Line break found" << std::endl;
		}
		if (*i != '\r' && *i != '\n')
		{
			signatures[index].push_back(*i);
		}
	}
	return signatures;
}

std::vector<uint8_t> File::get_content() {
	if (m_content.size() == 0)
	{
		m_content = read_file();
	}

	return m_content;
}
