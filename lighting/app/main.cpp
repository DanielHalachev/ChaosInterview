#include <chrono>
#include <string>

#include "lighting/LightingParser.h"

int main() {
	LightingParser<double> parser;
	for (auto i = 1; i <= 3; i++) {
		auto start = std::chrono::high_resolution_clock::now();
		parser.parse("/home/daniel/test" + std::to_string(i) + ".txt", nullptr, true);
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;
		std::cout << duration.count() << "\n";
	}
}