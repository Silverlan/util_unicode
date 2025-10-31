// SPDX-FileCopyrightText: (c) 2024 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;


module pragma.string.unicode;

import :shift_jis;

static std::vector<uint8_t> g_convTable;
static bool g_convTableInitialized = false;
void pragma::string::unicode::init_shift_jis(const std::string &dataFilePath)
{
	if(g_convTableInitialized)
		return;
	std::ifstream file(dataFilePath, std::ios::binary | std::ios::ate);
	if(!file) {
		throw std::runtime_error {"Failed to open file '" + dataFilePath + "'!"};
		return;
	}

	std::streamsize fileSize = file.tellg();
	if(fileSize <= 0) {
		throw std::runtime_error {"File '" + dataFilePath + "' is empty!"};
		return;
	}
	file.seekg(0, std::ios::beg);

	g_convTable.resize(fileSize);
	if(!file.read(reinterpret_cast<char *>(g_convTable.data()), fileSize)) {
		throw std::runtime_error {"Failed to read file '" + dataFilePath + "'!"};
		return;
	}

	file.close();
	g_convTableInitialized = true;
}

static std::string sj2utf8(const std::vector<uint8_t> &convTable, const std::string &input)
{
	// See https://stackoverflow.com/a/33170901/2482983
	std::string output(3 * input.length(), ' '); //ShiftJis won't give 4byte UTF8, so max. 3 byte per input char are needed
	size_t indexInput = 0, indexOutput = 0;

	while(indexInput < input.length()) {
		char arraySection = ((uint8_t)input[indexInput]) >> 4;

		size_t arrayOffset;
		if(arraySection == 0x8)
			arrayOffset = 0x100; //these are two-byte shiftjis
		else if(arraySection == 0x9)
			arrayOffset = 0x1100;
		else if(arraySection == 0xE)
			arrayOffset = 0x2100;
		else
			arrayOffset = 0; //this is one byte shiftjis

		//determining real array offset
		if(arrayOffset) {
			arrayOffset += (((uint8_t)input[indexInput]) & 0xf) << 8;
			indexInput++;
			if(indexInput >= input.length())
				break;
		}
		arrayOffset += (uint8_t)input[indexInput++];
		arrayOffset <<= 1;

		//unicode number is...
		uint16_t unicodeValue = (convTable[arrayOffset] << 8) | convTable[arrayOffset + 1];

		//converting to UTF8
		if(unicodeValue < 0x80) {
			output[indexOutput++] = unicodeValue;
		}
		else if(unicodeValue < 0x800) {
			output[indexOutput++] = 0xC0 | (unicodeValue >> 6);
			output[indexOutput++] = 0x80 | (unicodeValue & 0x3f);
		}
		else {
			output[indexOutput++] = 0xE0 | (unicodeValue >> 12);
			output[indexOutput++] = 0x80 | ((unicodeValue & 0xfff) >> 6);
			output[indexOutput++] = 0x80 | (unicodeValue & 0x3f);
		}
	}

	output.resize(indexOutput); //remove the unnecessary bytes
	return output;
}

std::string pragma::string::unicode::shift_jis_to_utf8(const std::string &input, bool validate)
{
	if(!g_convTableInitialized)
		throw std::runtime_error {"Shift_JIS has not been initialized! Make sure to run init_shift_jis first!"};
	if(validate) {
		if(validate_shift_jis(input.data()) == 0)
			return input;
	}
	return ::sj2utf8(g_convTable, input);
}
