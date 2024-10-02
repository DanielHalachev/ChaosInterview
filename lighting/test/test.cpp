#include <algorithm>
#include <catch2/benchmark/catch_benchmark_all.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <random>
#include <thread>
#include <vector>

#include "../include/lighting/LightingParser.h"

const unsigned int NUM_LIGHTS = 250'000;
const unsigned int RESOLUTION = 128;

class DefaultProgressNotifier : public ProgressNotifier {
   public:
	unsigned short numReports{0};
	void reportProgress(unsigned short  /*done*/, unsigned short  /*total*/) final {
		this->numReports++;
	}
};

TEST_CASE("Test computing 250 000 lights") {
	const double DEG = 15.0;
	std::default_random_engine engine;
	std::uniform_real_distribution<double> genX(0.0, 1.0);
	std::uniform_real_distribution<double> genY(0.0, 1.0);
	std::uniform_int_distribution<short> genR(0, 24);
	std::uniform_int_distribution<short> genS(1, 10);
	std::uniform_real_distribution<double> genI(0.2, 0.4);

	std::vector<Light<double>> lights;
	lights.reserve(NUM_LIGHTS);

	std::filesystem::path testInputPath("./test.txt");
	std::filesystem::path testOutputPath =
		testInputPath.parent_path() / (testInputPath.stem().string() + "_out" + testInputPath.extension().string());

	std::ofstream testFile(testInputPath);
	if (testFile.is_open()) {
		testFile << RESOLUTION << "\n";
		testFile << NUM_LIGHTS << "\n";
		for (auto i = 0; i < NUM_LIGHTS; i++) {
			testFile << genX(engine) << " "								//
					 << genY(engine) << " "								//
					 << DEG * static_cast<double>(genR(engine)) << " "	//
					 << DEG * static_cast<double>(genS(engine)) << " "	//
					 << genI(engine) << "\n";							//
		}
	}
	testFile.close();

	DefaultProgressNotifier notifier;
	LightingParser<double> parser;
	unsigned short numThreads = std::thread::hardware_concurrency();
	numThreads = std::min(numThreads * numThreads, 32);

	parser.parse(testInputPath, &notifier);
	CHECK_FALSE(testOutputPath.empty());
	CHECK(notifier.numReports == numThreads);
	std::filesystem::remove(testInputPath);
	std::filesystem::remove(testOutputPath);
}