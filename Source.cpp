#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <bitset>
#include <sstream>
#include <unordered_map>

//Day 14 Part 1
//Given a list of programs, where each program starts with a mask which is a 36 length string containing 1, 0, or x, and memory locations to insert given values at, in 
//the form of mem[location] = value. Apply the mask to the binary equalivent of the value being placed at a given memory location and put the decimal value of that 
//number at that location. Then sum all values left in memory to get the answer.

//For this we will store our memory in a may where the location is the key and the stored value is the value. It might be best to process the mask for each value before
//storing it in the may so that we don't need to store the mask and relate it to the memory locations it affects. For each bit in the mask that bit in the value's binary
//equalivent gets over written, so we can OR the 1s and AND the 0s to copy.

std::vector<std::string> getPuzzleInput()
{
	std::vector<std::string> input;

	std::ifstream file;
	std::string line;
	file.open("puzzle-input.txt"); //change to test-input to run test, current test passes, but actual input fails
	while (std::getline(file, line))
	{
		input.push_back(line);
	}

	return input;
}

uint64_t processMask(uint64_t value, std::string mask)
{
	std::bitset<36> b = std::bitset<36>(value);

	for (int i = 35; i > 0; i--)
	{
		int pos = 35 - i;

		if (mask[i] == '1')
		{
			b.set(pos);
		}

		if (mask[i] == '0')
		{
			b.reset(pos);
		}
	}

	uint64_t result;
	result = b.to_ullong();

	return result;
}

std::pair<int, uint64_t> processInstruction(std::string line, std::string mask)
{
	//extract mem location with getline
	std::string reg = line.substr(line.find("[") + 1);
	std::stringstream s(reg);
	std::string loc;
	std::getline(s, loc, ']');

	int mem = std::stoi(loc);

	uint64_t value = std::stoull(line.substr(line.find("=") + 2));

	//std::cout << mem << ":" << value << '\n';

	uint64_t masked = processMask(value, mask);

	return std::make_pair(mem, masked);
}

std::unordered_map<int, uint64_t> processPrograms(std::vector<std::string> input)
{
	std::string mask = "";
	std::pair<int, uint64_t> pair;
	std::unordered_map<int, uint64_t> map;

	for (auto i : input)
	{
		std::string start = i.substr(0, 3);

		if (start == "mas")
		{
			mask = i.substr(7);
		}
		else if (start == "mem")
		{
			pair = processInstruction(i, mask);
		}

		std::unordered_map<int, uint64_t>::iterator it = map.find(pair.first);
		if (it != map.end())
		{
			it->second = pair.second;
		}
		else
			map.insert(pair);
	}

	return map;
}

uint64_t findAnswerPart1(const std::unordered_map<int, uint64_t>& map)
{
	uint64_t count = 0;

	for (auto& m : map)
	{
		count = count + m.second;
	}

	return count;
}


int main()
{
	std::vector<std::string> input = getPuzzleInput();

	std::unordered_map<int, uint64_t> result = processPrograms(input);

	/*for (auto& r : result)
	{
		std::cout << "mem[" << r.first << "] = " << r.second << '\n';
	}*/

	std::cout << findAnswerPart1(result) << '\n'; //current answer: 5198746293242 is incorrect

	return 0;
}