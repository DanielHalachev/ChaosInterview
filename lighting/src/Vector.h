#pragma once

#include <algorithm>
#include <array>
#include <boost/math/ccmath/sqrt.hpp>
#include <boost/math/special_functions.hpp>
#include <boost/math/ccmath/ccmath.hpp>
#include <cmath>
#include <cstddef>
#include <numeric>
#include <vector>

#include "Utils.h"

template <typename T>
struct Vector {
	std::array<T, 2> values;

	[[nodiscard]] T getSquareLength() const;
	[[nodiscard]] T getRealSquareLength() const;
	[[nodiscard]] T getLength() const;
	void normalize();
	[[nodiscard]] Vector getNormalized() const;
	const T &operator[](const size_t index) const;
	T &operator[](const size_t index);
	Vector operator+(const Vector &other) const;
	Vector operator-(const Vector &other) const;
	Vector &operator-=(const Vector &other);
	Vector &operator+=(const Vector &other);
	Vector operator*(const T &other) const;
	Vector operator/(const T &other) const;

	T dot(const Vector &other) const;
};

template <typename T>
T Vector<T>::getSquareLength() const {
	T result = 0;
	for (auto value : this->values) {
		result += value * value;
	}
	return result;
	// std::vector<T> results(this->values.begin(), this->values.end());
	// for (auto &value : results) {
	// 	value = value * value;
	// }
	// return kahan(results);
}

template <typename T>
T Vector<T>::getLength() const {
	return sqrt(this->getSquareLength());
}

template <typename T>
void Vector<T>::normalize() {
	T length = this->getLength();
	if (length == 0) {
		return;
	}
	for (auto &value : this->values) {
		value /= length;
	}
}

template <typename T>
Vector<T> Vector<T>::getNormalized() const {
	Vector result(*this);
	result.normalize();
	return result;
}

template <typename T>
const T &Vector<T>::operator[](const size_t index) const {
	return this->values[index];
}

template <typename T>
T &Vector<T>::operator[](const size_t index) {
	return this->values[index];
}

template <typename T>
Vector<T> Vector<T>::operator+(const Vector &other) const {
	return Vector{this->values[0] + other.values[0], this->values[1] + other.values[1]};
}

template <typename T>
Vector<T> Vector<T>::operator-(const Vector &other) const {
	return Vector{this->values[0] - other.values[0], this->values[1] - other.values[1]};
}

template <typename T>
Vector<T> &Vector<T>::operator-=(const Vector &other) {
	this->values[0] -= other.values[0];
	this->values[1] -= other.values[1];
	return *this;
}

template <typename T>
Vector<T> &Vector<T>::operator+=(const Vector &other) {
	this->values[0] += other.values[0];
	this->values[1] += other.values[1];
	return *this;
}

template <typename T>
Vector<T> Vector<T>::operator*(const T &other) const {
	return Vector{this->values[0] * other, this->values[1] * other};
}

template <typename T>
Vector<T> Vector<T>::operator/(const T &other) const {
	return Vector{this->values[0] / other, this->values[1] / other};
}

template <typename T>
T Vector<T>::dot(const Vector &other) const {
	return this->values[0] * other.values[0] + this->values[1] * other.values[1];
}
