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

//For this we will store our program's memory in a map, for each mem line we will apply the mask, we can use bitsets to create a binary version of the value and iterate
//the mask string in reverse to set or reset certain bits. Then return the decimal version of the bitset and store that value in the memory at the correct address, once
//all of the programs have been processed we can sum up all the values in memory to get the answer

//Part 2
//Now instead of the mask being applied to the value its is instead applied to the memory address, also within the mask the bits that are marked with 'X' are now
//floating meaning they can be both 1 and 0, we will need to find all permutations of 'X' bits and store the value at all of those addresses. Then we need to sum all the
//values left in memory to find the answer

//Because the 'X's don't matter, we really only need to find values that have the 

std::vector<std::string> getPuzzleInput()
{
	std::vector<std::string> input;

	std::ifstream file;
	std::string line;
	file.open("puzzle-input.txt"); 
	while (std::getline(file, line))
	{
		input.push_back(line);
	}

	return input;
}

uint64_t processMask(uint64_t value, std::string mask)
{
	std::bitset<36> b = std::bitset<36>(value);

	for (int i = 36; i >= 0; i--) //watch out for off by 1s
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

//part 2-----------------------------------------------------------------------------------------------------------------------------------------

void generateBinaryPermutations(int i, int n, std::vector<std::bitset<36>>& bitsets, std::bitset<36> bits, std::string mask)
{

	if (i == n)
	{
		bitsets.push_back(bits);
		return;
	}

		if (mask[i] == 'X')
		{
			bits[35 - i] = 0;
			generateBinaryPermutations(i + 1, n, bitsets, bits, mask);

			bits[35 - i] = 1;
			generateBinaryPermutations(i + 1, n, bitsets, bits, mask);
		}
		if (mask[i] == '0')
		{
			generateBinaryPermutations(i + 1, n, bitsets, bits, mask);
		}
		if (mask[i] == '1')
		{
			bits[35 - i] = 1;
			generateBinaryPermutations(i + 1, n, bitsets, bits, mask);
		}
}

//this will take a map, and it will first check to see if the address exists in the map, if so it will update the map, if not it will create a new pair and insert into the
//map
void writeToAddress(std::vector<std::bitset<36>> bitsets, uint64_t value, std::unordered_map<uint64_t, uint64_t>& map) 
{
	for (auto b : bitsets)
	{
		uint64_t addr = b.to_ullong();

		std::unordered_map<uint64_t, uint64_t>::iterator it = map.find(addr);
		if (it != map.end())
		{
			it->second = value;
		}
		else
		{
			map.insert(std::make_pair(addr, value));
		}
	}
}

void processInstructionP2(std::string line, std::string mask, std::unordered_map<uint64_t, uint64_t>& map)
{
	//extract mem location with getline
	std::string reg = line.substr(line.find("[") + 1);
	std::stringstream s(reg);
	std::string loc;
	std::getline(s, loc, ']');

	int mem = std::stoi(loc);

	uint64_t value = std::stoull(line.substr(line.find("=") + 2));

	std::vector<std::bitset<36>> bitsets;
	std::bitset bits = std::bitset<36>(mem);

	generateBinaryPermutations(0, 36, bitsets, bits, mask);

	writeToAddress(bitsets, value, map);

}

void processProgramsP2(std::vector<std::string> input, std::unordered_map<uint64_t, uint64_t>& map)
{
	std::string mask = "";

	for (auto i : input)
	{
		std::string start = i.substr(0, 3);

		if (start == "mas")
		{
			mask = i.substr(7);
		}
		else if (start == "mem")
		{
			processInstructionP2(i, mask, map);
		}
	}
}


uint64_t findAnswerPart2(const std::unordered_map<uint64_t, uint64_t>& map)
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

	//std::cout << findAnswerPart1(result) << '\n';

	//std::vector<std::bitset<36>> bitsets;

	//const int n = 36;

	//std::bitset bits = std::bitset<36>(42);
	//std::string mask = "000000000000000000000000000000X1001X";

	//generateBinaryPermutations(0, n, bitsets, bits, mask);

	//for (auto b : bitsets)
	//{
	//	std::cout << b.to_string() << '\n';
	//}

	//writeToAddress(bitsets, 100);
	std::unordered_map<uint64_t, uint64_t> map;

	processProgramsP2(input, map);

	std::cout << findAnswerPart2(map) << '\n';

	return 0;
}