#include <cmath>
#include <istream>
#include <limits>

#include "Vector.h"
template <typename T>
struct Light {
	static constexpr T DEG_TO_RAD_MODIFIER = 180.0;

	Vector<T> position{};
	Vector<T> direction{};

	T halfSpread = 0;
	T halfSpreadCosine = 0;
	T intensity = 0;

	Light() = default;
	Light(T x, T y, T r, T s, T i);

	template <typename S>
	friend std::istream &operator>>(std::istream &is, Light<S> &light);

	[[nodiscard]] T contributionToPixel(const Vector<T> &point) const;
};

template <typename T>
T toRadians(T degrees) {
	degrees = degrees / Light<T>::DEG_TO_RAD_MODIFIER;
	return degrees * M_PI;
}

template <typename T>
Light<T>::Light(T x, T y, T r, T s, T i)
	: position{x, y},
	  direction{std::cos(toRadians(r)), std::sin(toRadians(r))},
	  halfSpreadCosine(std::cos(toRadians(s) / 2)),
	  halfSpread(s/2),
	  intensity{i} {
	direction.normalize();
}

template <typename T>
T Light<T>::contributionToPixel(const Vector<T> &point) const {
	Vector distanceToLight = point - this->position;
	T squareDistance = distanceToLight.getSquareLength();
	if (squareDistance < std::numeric_limits<T>::epsilon()) {
		return 0;
	}
	distanceToLight.normalize();
	if (distanceToLight.dot(this->direction) >= this->halfSpreadCosine) {
		// our coordinate system is twice as big,
		// [0,1] -> [-1,1] for every axis, therefore
		// the "real" distance is twice as small for each axis
		// we are using squared distance
		// therefore the "real" distance is 4 times as small
		return 4.0 * this->intensity / squareDistance;
	}

	return 0;
}

template <typename T>
std::istream &operator>>(std::istream &is, Light<T> &light) {
	// px, py
	is >> light.position[0];
	is >> light.position[1];

	light.position[0] = (2.0f * light.position[0]) - 1.0f;
	light.position[1] = (2.0f * light.position[1]) - 1.0f;

	// r - turning
	T degrees = 0;
	is >> degrees;
	degrees = toRadians(degrees);
	light.direction = {std::cos(degrees), std::sin(degrees)};
	light.direction.normalize();

	// s - spread
	is >> degrees;
	degrees = toRadians(degrees) / 2;
	light.halfSpread = degrees;
	light.halfSpreadCosine = std::cos(degrees);

	// i
	is >> light.intensity;

	return is;
}
