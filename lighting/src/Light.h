#pragma once

#include <cmath>
#include <istream>

#include "Utils.h"
#include "Vector.h"

template <typename T>
struct Light {
	Vector<T> position{};
	Vector<T> direction{};

	T rotation = 0;
	T halfSpread = 0;
	T halfSpreadCosine = 0;
	T intensity = 0;

	template <typename S>
	friend std::istream &operator>>(std::istream &is, Light<S> &light);

	[[nodiscard]] T contributionToPixel(const Vector<T> &point) const;
};

// initial first solution
// template <typename T>
// T Light<T>::contributionToPixel(const Vector<T> &point) const {
//     T theta =
//         std::atan2(point[1] - this->position[1], point[0] - this->position[0]);
//     Vector<T> distanceToLight = point - this->position;
//     if(theta < 0){
//         theta = 2*M_PIf + theta;
//     }
//     T absDiff = std::abs(theta - this->rotation);
//     absDiff = std::min(absDiff, 2 * M_PIf - absDiff);
//     if (absDiff <= this->halfSpread) {
//         return this->intensity / distanceToLight.getSquareLength();
//     }

//     return 0;
// }

// current solution
template <typename T>
T Light<T>::contributionToPixel(const Vector<T> &point) const {
	Vector<T> distanceToLight = point - this->position;
	T squareDistance = distanceToLight.getSquareLength();
	if(squareDistance == 0){
		return 0;
	}
	// experimented with reformulation of calculations
	// to avoid floating point substraction errors
	// of numbers with negligible difference
	// T squareDistance = point[0] * point[0] + point[1] * point[1] + this->position[0] * this->position[0] +
	// 				   this->position[1] * this->position[1] - 2 * point[0] * this->position[0] -
	// 2 * point[1] * this->position[1];
	// if (squareDistance < std::numeric_limits<T>::epsilon()) {
	// 	return 0;
	// }
	distanceToLight.normalize();
	T spotFactor = distanceToLight.dot(this->direction);
	if (spotFactor >= this->halfSpreadCosine) {
		// return MULTIPLIER * MULTIPLIER * this->intensity / squareDistance;
		return this->intensity / squareDistance;
	}

	return 0;
}

template <typename T>
std::istream &operator>>(std::istream &is, Light<T> &light) {
	// px, py
	is >> light.position[0];
	is >> light.position[1];

	// light.position[0] *= MULTIPLIER;
	// light.position[1] *= MULTIPLIER;

	// r - turning
	T degrees = 0;
	is >> degrees;
	degrees = toRadians(degrees);
	light.rotation = degrees;
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
