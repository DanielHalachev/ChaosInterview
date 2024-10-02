#pragma once

#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

#include "../include/lighting/ProgressNotifier.h"
// #include "BVH.h"
#include "Light.h"
#include "ThreadManager.h"
// #include "Utils.h"
template <typename T>
class Scene {
   private:
	// BVH<T> bvh;
	unsigned int M = 0;
	std::vector<Light<T>> lights;
	ProgressNotifier *progressNotifier = nullptr;
	unsigned short totalRectangles = 0;
	std::atomic<unsigned short> rectanglesDone = 0;

   private:
	void renderRectangle(std::vector<std::vector<T>> &result, unsigned int rowIndex, unsigned int columnIndex,
						 unsigned int width, unsigned int height);

   public:
	template <typename S>
	friend std::istream &operator>>(std::istream &is, Scene<S> &scene);
	std::vector<std::vector<T>> render(ProgressNotifier *notifier = nullptr);
	[[nodiscard]] unsigned int resolution() const { return M; }
};

template <typename T>
std::istream &operator>>(std::istream &is, Scene<T> &scene) {
	is >> scene.M;
	unsigned int numberOfLights = 0;
	is >> numberOfLights;
	scene.lights.resize(numberOfLights);
	for (auto i = 0; i < numberOfLights; i++) {
		is >> scene.lights[i];
	}

	// scene.bvh = std::move(BVH(scene.lights));

	return is;
}

template <typename T>
void Scene<T>::renderRectangle(std::vector<std::vector<T>> &result, unsigned int rowIndex, unsigned int columnIndex,
							   unsigned int width, unsigned int height) {
	unsigned int rowLimit = std::min(this->M, rowIndex + height);
	unsigned int columnLimit = std::min(this->M, columnIndex + width);
	for (auto row = rowIndex; row < rowLimit; row++) {
		for (auto col = columnIndex; col < columnLimit; col++) {
			Vector<T> pixel{static_cast<T>(col) / static_cast<T>(this->M),
							static_cast<T>(row) / static_cast<T>(this->M)};

			// flip direction of y axis
			// to use identical coordinate systems
			pixel[1] = T(1) - (pixel[1]);

			// experimented with changing the range of the coordinates
			// pixel[0] *= MULTIPLIER;
			// pixel[1] *= MULTIPLIER;

			// experimented with Kahan summation
			// std::vector<T> values;
			// values.reserve(this->lights.size());
			// for (auto &light : this->lights) {
			// 	values.push_back(light.contributionToPixel(pixel));
			// }
			// result[row][col] = kahan(values);

			T contribution = 0;
			for (auto &light : this->lights) {
				contribution += light.contributionToPixel(pixel);
			}
			result[row][col] = contribution;
		}
	}

	if (this->progressNotifier) {
		this->rectanglesDone++;
		this->progressNotifier->reportProgress(this->rectanglesDone.load(), this->totalRectangles);
	}
}

template <typename T>
std::vector<std::vector<T>> Scene<T>::render(ProgressNotifier *notifier) {
	this->progressNotifier = notifier;
	std::vector<std::vector<T>> result(this->M, std::vector<T>(this->M, 0));
	unsigned int threadCount = std::thread::hardware_concurrency();
	this->totalRectangles = std::min(32u, threadCount * threadCount);
	ThreadManager manager(threadCount);
	unsigned int threadNumY = std::sqrt(this->totalRectangles);
	if (threadNumY == 0) {
		threadNumY = 1;
	}
	unsigned int threadNumX = this->totalRectangles / threadNumY;
	unsigned int regionWidth = this->M / threadNumX;
	unsigned int regionHeight = this->M / threadNumY;
	for (auto i = 0; i < this->totalRectangles; i++) {
		unsigned column = (i * regionWidth) % this->M;
		unsigned row = (i / threadNumX) * regionHeight;
		manager.doJob([this, &result, row, column, regionWidth, regionHeight]() {
			this->renderRectangle(result, row, column, regionWidth, regionHeight);
		});
	}
	manager.waitForAll();
	return result;
}
