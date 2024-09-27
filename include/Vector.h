#include <array>
#include <cmath>
#include <cstddef>

template <typename T>
struct Vector {
	std::array<T, 2> values;

	[[nodiscard]] T getSquareLength() const;
	[[nodiscard]] T getLength() const;
	void normalize();
	[[nodiscard]] Vector getNormalized() const;
	const T &operator[](const size_t index) const;
	T &operator[](const size_t index);
	Vector operator+(const Vector &other) const;
	Vector operator-(const Vector &other) const;

	T dot(const Vector &other);
};

template <typename T>
T Vector<T>::getSquareLength() const {
	T result = 0;
	for (auto value : this->values) {
		result += value * value;
	}
	return result;
}

template <typename T>
T Vector<T>::getLength() const {
	return std::sqrt(this->getSquareLength());
}

template <typename T>
void Vector<T>::normalize() {
	T length = this->getLength();
	if (length == 0) {
		return;
	}
	length = 1.0f / length;
	for (auto &value : this->values) {
		value *= length;
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
	return Vector{this->values[0] + other.values[0],
				  this->values[1] + other.values[1]};
}

template <typename T>
Vector<T> Vector<T>::operator-(const Vector &other) const {
	return Vector{this->values[0] - other.values[0],
				  this->values[1] - other.values[1]};
}

template <typename T>
T Vector<T>::dot(const Vector &other) {
	return this->values[0] * other.values[0] +
		   this->values[1] * other.values[1];
}
