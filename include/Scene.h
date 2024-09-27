#include <algorithm>
#include <iostream>
#include <vector>

#include "Light.h"
template <typename T>
struct Scene {
	unsigned int M = 0;
	std::vector<Light<T>> lights;

	template <typename S>
	friend std::istream &operator>>(std::istream &is, Scene<S> &scene);
	std::vector<std::vector<T>> render();
};

template <typename T>
T kahan(std::vector<T> &values) {
	std::sort(values.begin(), values.end());
	T sum = T(0);
	T error = T(0);

	for (const T value : values) {
		T y = value - error;	// recover error
		T t = sum + y;			// add corrected value
		error = (t - sum) - y;	// compute new error
		sum = t;				// update sum
	}

	return sum;
}

template <typename T>
std::istream &operator>>(std::istream &is, Scene<T> &scene) {
	is >> scene.M;
	unsigned int numberOfLights = 0;
	is >> numberOfLights;
	scene.lights.resize(numberOfLights);
	for (auto i = 0; i < numberOfLights; i++) {
		is >> scene.lights[i];
	}

	return is;
}

template <typename T>
std::vector<std::vector<T>> Scene<T>::render() {
	std::vector<std::vector<T>> result(this->M, std::vector<T>(this->M, 0));
	for (auto row = 0; row < this->M; row++) {
		for (auto col = 0; col < this->M; col++) {
			T contribution = 0;
			Vector<T> pixel{static_cast<T>(col) / static_cast<T>(this->M),
							static_cast<T>(row) / static_cast<T>(this->M)};

			pixel[0] = (2.0f * pixel[0]) - 1.0f;
			pixel[1] = 1.0f - (2.0f * pixel[1]);

			// std::vector<T> values;
			// values.reserve(this->lights.size());
			for (auto &light : this->lights) {
				// values.push_back(light.contributionToPixel(pixel));
				contribution += light.contributionToPixel(pixel);
			}

			// contribution = kahan(values);

			result[row][col] = contribution;
		}
	}

	return result;
}
