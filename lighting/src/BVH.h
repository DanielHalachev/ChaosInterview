#pragma once

#include <array>
#include <cmath>
#include <numeric>
#include <stack>
#include <vector>

#include "BoundingBox.h"
#include "Light.h"
#include "Vector.h"

const unsigned int INVALID_INDEX = std::numeric_limits<unsigned int>::max();

template <class T>
class BVH {
   private:
	struct Node {
		BoundingBox<T> box;
		std::array<unsigned int, 2> children{0, 0};
		unsigned int parent = 0;
		std::vector<unsigned int> indexes;

		T illuminate(const Vector<T> &pixel) const;

		[[nodiscard]] bool isLeaf() const { return !this->indexes.empty(); }
	};

	unsigned short MAX_ELEMENTS_IN_LEAF = 16;
	unsigned short MAX_DEPTH = 20;
	unsigned short AXIS_COUNT = 2;
	std::vector<Node> nodes;
	std::vector<BoundingBox<T>> boundingBoxes;
	const std::vector<Light<T>> *container;

   public:
	BVH() = default;
	explicit BVH(const std::vector<Light<T>> &lights) {
		// this->MAX_DEPTH = std::ceil(std::log2(static_cast<T>(lights.size()) / static_cast<T>(this->MAX_ELEMENTS_IN_LEAF)));
		this->container = &lights;
		this->boundingBoxes.reserve(lights.size());
		for (auto &light : lights) {
			boundingBoxes.push_back(BoundingBox<T>(light));
		}
		std::vector<unsigned int> indexes(lights.size());
		std::iota(indexes.begin(), indexes.end(), 0);
		BoundingBox<T> globalBox;
		for (auto index : indexes) {
			globalBox.merge(boundingBoxes[index]);
		}
		unsigned int rootIndex = this->createNode(globalBox, INVALID_INDEX,
												  INVALID_INDEX, INVALID_INDEX);

		build(rootIndex, 0, indexes);
	}

	unsigned int createNode(const BoundingBox<T> &box,
							unsigned int firstChildIndex,
							unsigned int secondChildIndex,
							unsigned int parentIndex) {
		unsigned int newNodeIndex = this->nodes.size();
		this->nodes.push_back(
			Node{box, {firstChildIndex, secondChildIndex}, parentIndex, {}});
		return newNodeIndex;
	}

	void build(const unsigned int parentIndex, const unsigned short depth,
			   const std::vector<unsigned int> &elementIndexes) {
		if (depth >= MAX_DEPTH ||
			elementIndexes.size() <= MAX_ELEMENTS_IN_LEAF) {
			this->nodes[parentIndex].indexes = elementIndexes;
			return;
		}

		std::pair<BoundingBox<T>, BoundingBox<T>> childBoxes =
			this->nodes[parentIndex].box.split(depth % AXIS_COUNT);
		std::vector<unsigned int> firstChildElements;
		std::vector<unsigned int> secondChildElements;
		firstChildElements.reserve(elementIndexes.size() / 2);
		secondChildElements.reserve(elementIndexes.size() / 2);

		for (auto elementIndex : elementIndexes) {
			if (childBoxes.first.intersects(
					this->boundingBoxes.at(elementIndex))) {
				firstChildElements.push_back(elementIndex);
			}
			if (childBoxes.second.intersects(
					this->boundingBoxes.at(elementIndex))) {
				secondChildElements.push_back(elementIndex);
			}
		}

		firstChildElements.shrink_to_fit();
		secondChildElements.shrink_to_fit();

		if (!firstChildElements.empty()) {
			this->nodes[parentIndex].children[0] = this->createNode(
				childBoxes.first, INVALID_INDEX, INVALID_INDEX, parentIndex);
			build(this->nodes[parentIndex].children[0], depth + 1,
				  firstChildElements);
		}

		if (!secondChildElements.empty()) {
			this->nodes[parentIndex].children[1] = this->createNode(
				childBoxes.second, INVALID_INDEX, INVALID_INDEX, parentIndex);
			build(this->nodes[parentIndex].children[1], depth + 1,
				  secondChildElements);
		}
	}

	T illuminate(const Vector<T> &pixel) {
		std::vector<unsigned int> intersections;
		std::stack<unsigned int> indexesToCheck;
		indexesToCheck.push(0);
		while (!indexesToCheck.empty()) {
			unsigned int currentIndex = indexesToCheck.top();
			const Node &currentNode = this->nodes[currentIndex];
			indexesToCheck.pop();
			if (currentNode.box.containsPixel(pixel)) {
				if (currentNode.isLeaf()) {
					for (auto lightIndex : currentNode.indexes) {
						intersections.push_back(lightIndex);
					}
				} else {
					if (currentNode.children[0] != INVALID_INDEX) {
						indexesToCheck.push(currentNode.children[0]);
					}
					if (currentNode.children[1] != INVALID_INDEX) {
						indexesToCheck.push(currentNode.children[1]);
					}
				}
			}
		}
		if (intersections.empty()) {
			return 0;
		}
		T contribution = 0;
		for (auto lightIndex : intersections) {
			contribution +=
				this->container->at(lightIndex).contributionToPixel(pixel);
		}
		return contribution;
	}
};