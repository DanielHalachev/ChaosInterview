#include <filesystem>
#include <string>

#include "IOParser.h"

int main() {
	IOParser<float> parser;
	for (auto i = 1; i <= 2; i++) {
		parser.parse("/home/daniel/test" + std::to_string(i) + ".txt", true);
	}
}