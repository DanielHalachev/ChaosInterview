// #define CATCH_CONFIG_MAIN
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
const unsigned int RESOLUTION = 256;

class DefaultProgressNotifier : public ProgressNotifier {
   public:
	unsigned short numReports{0};
	void reportProgress(unsigned short done, unsigned short total) final {
		this->numReports++;
		std::cout << 100.0 * static_cast<float>(done) / static_cast<float>(total) << "%\n";
	}
};

TEST_CASE("Test Bounding Box") {
	Light<double> l1{{0.75, 0.5}, {-1.0, 0.0}, toRadians(180.0), toRadians(30.0), std::cos(toRadians(30.0)), 0.3};
	Light<double> l2{{0.5, 1.2}, {0.0, -1.0}, toRadians(270.0), toRadians(15.0), std::cos(toRadians(15.0)), 0.2};
	Light<double> l3{{0.5, 0.5}, {1.0, 0.0}, toRadians(0.0), toRadians(180.0), std::cos(toRadians(180.0)), 0.2};
	BoundingBox<double> b1(l1);
	BoundingBox<double> b2(l2);
	BoundingBox<double> b3(l3);
	auto [a1, a2] = b1.split(0);
	auto c1 = a1.merge(a2);
	std::cout << a1 << a2 << c1;
}

TEST_CASE("Test time to compute 250 000 lights") {
	const double DEG = 15.0;
	std::default_random_engine engine;
	std::uniform_real_distribution<double> genX(0.0, 1.0);
	std::uniform_real_distribution<double> genY(0.0, 1.0);
	std::uniform_int_distribution<short> genR(0, 24);
	std::uniform_int_distribution<short> genS(1, 10);
	std::uniform_real_distribution<double> genI(0.2, 0.4);

	std::vector<Light<double>> lights;
	lights.reserve(NUM_LIGHTS);

	std::filesystem::path testFilePath("./test.txt");
	std::filesystem::path testOutputPath =
		testFilePath.parent_path() / (testFilePath.stem().string() + "_out" + testFilePath.extension().string());

	std::ofstream testFile(testFilePath);
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

	BENCHMARK("250 000 Lights") { return parser.parse(testFilePath, &notifier); };
	std::cout << notifier.numReports;
	CHECK(notifier.numReports == numThreads);
	// std::filesystem::remove(testFilePath);
	// std::filesystem::remove(testOutputPath);
}