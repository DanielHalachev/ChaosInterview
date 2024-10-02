#pragma once

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <ios>
#include <vector>

#include "../../src/Scene.h"
#include "ProgressNotifier.h"

template <typename T>
class LightingParser {
   public:
	static std::vector<std::vector<T>> parse(const std::filesystem::path &inputPath,
											 ProgressNotifier *notifier = nullptr, bool exportPPM = false);
};

template <typename T>
std::vector<std::vector<T>> LightingParser<T>::parse(const std::filesystem::path &inputPath, ProgressNotifier *notifier,
													 bool exportPPM) {
	Scene<T> scene;
	std::filesystem::path outputPath =
		inputPath.parent_path() / (inputPath.stem().string() + "_out" + inputPath.extension().string());

	std::ifstream inputFile(inputPath);
	if (inputFile.is_open()) {
		inputFile >> scene;
	}
	inputFile.close();

	std::vector<std::vector<T>> result = std::move(scene.render(notifier));
	std::ofstream outputFile(outputPath);
	if (outputFile.is_open()) {
		outputFile<<scene.resolution()<<"\n";
		for (auto row = 0; row < scene.resolution(); row++) {
			for (auto col = 0; col < scene.resolution(); col++) {
				outputFile << std::fixed << std::setprecision(6) << result[row][col] << " ";
			}
			outputFile << std::endl;
		}
	}

	if (exportPPM) {
		std::ofstream ppm(outputPath.replace_extension("ppm"));
		if (ppm.is_open()) {
			ppm << "P3"
				<< "\n"
				<< scene.resolution() << " " << scene.resolution() << "\n"
				<< 255 << "\n";
			for (auto row = 0; row < scene.resolution(); row++) {
				for (auto col = 0; col < scene.resolution(); col++) {
					unsigned short color = std::clamp(static_cast<unsigned int>(result[row][col] / 30 * 255), 0u, 255u);
					ppm << color << " " << color << " " << color << "\t";
				}
				ppm << std::endl;
			}
		}
	}

	outputFile.close();
	return result;
}