#pragma once

#include <algorithm>
#include <cmath>
#include <ostream>

#include "Light.h"
#include "Vector.h"

template <typename T>
class BoundingBox {
   private:
	Vector<T> minPoint;
	Vector<T> maxPoint;

	void initializeMinMaxPoints() {
		this->minPoint = Vector<T>{std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
		this->maxPoint = Vector<T>{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};
	}

   public:
	BoundingBox() { initializeMinMaxPoints(); }
	BoundingBox(const Vector<T> &minPoint, const Vector<T> &maxPoint) : minPoint{minPoint}, maxPoint{maxPoint} {}

	explicit BoundingBox(const Light<T> &light) {
		initializeMinMaxPoints();
		Vector<T> leftBoundary{std::cos(light.rotation - light.halfSpread),
							   std::sin(light.rotation - light.halfSpread)};
		Vector<T> rightBoundary{std::cos(light.rotation + light.halfSpread),
								std::sin(light.rotation + light.halfSpread)};
		leftBoundary.normalize();
		rightBoundary.normalize();

		if (leftBoundary[0] == rightBoundary[0] && leftBoundary[1] == -rightBoundary[1]) {
			this->minPoint = {0.0, 0.0};
			this->maxPoint = {1.0, 1.0};
			return;
		}

		Vector<T> leftUpper{-1, 1};
		Vector<T> rightUpper{1, 1};
		Vector<T> leftLower{-1, -1};
		Vector<T> rightLower{1, -1};

		leftUpper -= light.position;
		rightUpper -= light.position;
		leftLower -= light.position;
		rightLower -= light.position;

		T maxMeaningfullDistance = std::sqrt(std::max({leftUpper.getSquareLength(), rightUpper.getSquareLength(),
													   leftLower.getSquareLength(), rightLower.getSquareLength()}));

		Vector<T> leftMostPoint = light.position + leftBoundary * maxMeaningfullDistance;
		// leftMostPoint[0] = std::max(std::min(leftMostPoint[0], T(0)), T(1));
		// leftMostPoint[1] = std::max(std::min(leftMostPoint[0], T(0)), T(1));
		Vector<T> rightMostPoint = light.position + rightBoundary * maxMeaningfullDistance;
		// rightMostPoint[0] = std::max(std::min(leftMostPoint[0], T(0)), T(1));
		// rightMostPoint[1] = std::max(std::min(leftMostPoint[0], T(0)), T(1));

		this->minPoint[0] = {std::min({light.position[0], leftMostPoint[0], rightMostPoint[0]})};
		this->minPoint[1] = {std::min({light.position[1], leftMostPoint[1], rightMostPoint[1]})};
		this->minPoint[0] = std::max(this->minPoint[0], 0.0);
		this->minPoint[1] = std::max(this->minPoint[1], 0.0);

		this->maxPoint[0] = {std::max({light.position[0], leftMostPoint[0], rightMostPoint[0]})};
		this->maxPoint[1] = {std::max({light.position[1], leftMostPoint[1], rightMostPoint[1]})};
		this->maxPoint[0] = std::min(this->maxPoint[0], 1.0);
		this->maxPoint[1] = std::min(this->maxPoint[1], 1.0);
	}

	BoundingBox<T> &merge(const BoundingBox<T> &other) {
		this->minPoint[0] = std::min(this->minPoint[0], other.minPoint[0]);
		this->minPoint[1] = std::min(this->minPoint[1], other.minPoint[1]);
		this->maxPoint[0] = std::max(this->maxPoint[0], other.maxPoint[0]);
		this->maxPoint[1] = std::max(this->maxPoint[1], other.maxPoint[1]);
		return *this;
	}

	std::pair<BoundingBox, BoundingBox> split(const unsigned short axis) const {
		float middle = (this->maxPoint[axis] - this->minPoint[axis]) / 2;
		float splitPlaneCoordinate = this->minPoint[axis] + middle;
		BoundingBox first(*this);
		BoundingBox second(*this);

		first.maxPoint[axis] = splitPlaneCoordinate;
		second.minPoint[axis] = splitPlaneCoordinate;
		return {first, second};
	}

	bool intersects(const BoundingBox &box) const {
		for (auto i = 0; i < 3; i++) {
			bool notOverlapI = (this->minPoint[i] > box.maxPoint[i]) || (this->maxPoint[i] < box.minPoint[i]);
			if (notOverlapI) {
				return false;
			}
		}
		return true;
	}

	[[nodiscard]] bool containsPixel(const Vector<T> &point) const {
		if (point[0] < this->minPoint[0]) {
			return false;
		}
		if (point[1] < this->minPoint[1]) {
			return false;
		}
		if (point[0] > this->maxPoint[0]) {
			return false;
		}
		if (point[1] > this->maxPoint[1]) {
			return false;
		}
		return true;
	}

	friend std::ostream &operator<<(std::ostream &os, const BoundingBox<T> &box) {
		os << "Xmin=[" << box.minPoint[0] << "," << box.minPoint[1] << "],Xmax=[" << box.maxPoint[0] << ","
		   << box.maxPoint[1] << "]\n";
		return os;
	}
};