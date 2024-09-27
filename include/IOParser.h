#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>

#include "Scene.h"

template <typename T>
class IOParser {
   public:
	static void parse(const std::filesystem::path &inputPath,
					  bool exportPPM = false);
};

template <typename T>
void IOParser<T>::parse(const std::filesystem::path &inputPath,
						bool exportPPM) {
	Scene<T> scene;
	std::filesystem::path outputPath =
		inputPath.parent_path() /
		(inputPath.stem().string() + "_out" + inputPath.extension().string());

	std::ifstream inputFile(inputPath);
	if (inputFile.is_open()) {
		inputFile >> scene;
	}
	inputFile.close();

	std::vector<std::vector<T>> result = std::move(scene.render());
	std::ofstream outputFile(outputPath);
	if (outputFile.is_open()) {
		for (auto row = 0; row < scene.M; row++) {
			for (auto col = 0; col < scene.M; col++) {
				outputFile << std::fixed << std::setprecision(6)
						   << result[row][col] << " ";
			}
			outputFile << std::endl;
		}
	}

	if (exportPPM) {
		std::ofstream ppm(outputPath.replace_extension("ppm"));
		if (ppm.is_open()) {
			ppm << "P3"
				<< "\n"
				<< scene.M << " " << scene.M << "\n"
				<< 255 << "\n";
			for (auto row = 0; row < scene.M; row++) {
				for (auto col = 0; col < scene.M; col++) {
					unsigned short color = std::clamp(
						static_cast<unsigned int>(result[row][col] / 30 * 255),
						0u, 255u);
					ppm << color << " " << color << " " << color << "\t";
				}
				ppm << std::endl;
			}
		}
	}

	outputFile.close();
}