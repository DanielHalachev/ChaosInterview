#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

// const double MULTIPLIER = 100.0;

template <typename T>
T toRadians(T degrees) {
	degrees = degrees / T(180);
	return degrees * M_PI;
}

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
