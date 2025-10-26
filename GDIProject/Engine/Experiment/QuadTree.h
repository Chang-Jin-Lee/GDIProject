#pragma once
#include "Platform.h"
#include "AABBBox.h"
#include <Windows.h>
#include <vector>

template <typename ElementType, int32 NodeCapacity = 4>
class TQuadTree
{
private:
	int MAX_OBJECTS = 8;
	int MAX_LEVELS = 5;

public:
	TQuadTree();
	TQuadTree(int level, FAABBBox* Box);
	~TQuadTree();

	typedef TQuadTree<ElementType, NodeCapacity> TreeType;

	void Split();
	void Insert(const ElementType& Element, const FAABBBox& Box);
	void Clear();
	std::vector<ElementType> GetElements(const FAABBBox& Box, std::vector<ElementType>& ElementsOut);

	int32 GetQuadsIndex(const FAABBBox& Box) const;

private:

	enum QuadNames
	{
		TopLeft = 0,
		TopRight = 1,
		BottomLeft = 2,
		BottomRight = 3
	};

	struct FNode
	{
		FAABBBox Box;
		ElementType Element;

		FNode() {};

		FNode(const ElementType& InElement, const FAABBBox& InBox)
			: Box(InBox)
			, Element(InElement)
		{
		}
	};

	int level;								// 쿼드트리의 level
	FAABBBox boundBox;							// 쿼드트리 최소, 최대 좌표
#define QUADTREE_MAX_SUBNODE_SIZE 4
	TreeType* subNodes[QUADTREE_MAX_SUBNODE_SIZE] = { nullptr };	// 현재 노드의 자식 노드들
	std::vector<FNode> nodes;				// 트리를 순회하여 노드를 담아 리턴할 노드들의 정보
	FVector2 centerPosition;				// 쿼드트리 한 칸의 중간점
	bool IsleafNode;							// sub-tree 내부 혹은 leaf node인지를 판단 true면 leaf.
};

template <typename ElementType, int32 NodeCapacity>
TQuadTree<ElementType, NodeCapacity>::TQuadTree()
	: level(0)
	, IsleafNode(true)
{
}

template <typename ElementType, int32 NodeCapacity>
TQuadTree<ElementType, NodeCapacity>::TQuadTree(int level, FAABBBox* Box)
	: boundBox(*Box)
	, centerPosition(Box->GetCenter())
	, level(level)
	, IsleafNode(true)
{
	subNodes[0] = subNodes[1] = subNodes[2] = subNodes[3] = nullptr;
}

template <typename ElementType, int32 NodeCapacity>
TQuadTree<ElementType, NodeCapacity>::~TQuadTree()
{
	for (TreeType* node : subNodes)
	{
		delete node;
		node = nullptr;
	}
}

template<typename ElementType, int32 NodeCapacity>
inline void TQuadTree<ElementType, NodeCapacity>::Split()
{
	float subWidth = (boundBox.GetMaxVector().x - boundBox.GetMinVector().x) / 2.0f;
	float subHeight = (boundBox.GetMaxVector().y - boundBox.GetMinVector().y) / 2.0f;
	float x = centerPosition.x;
	float y = centerPosition.y;

	// centerPosition이 c이고, max - c, c-min이 각각 subWidth, subHeight가 된다. 
	// 그러니 다음 노드에게는 똑같이 최소, 최대 점인 min, max만 던져주자.
	/*  ___________max
	* |     |     |
	* |     |     |
	* |-----c------
	* |     |     |
	* min___|_____|
	*/

	// 나눌 수 있으니 더 이상 이 노드는 leaf node가 아님
	IsleafNode = false;
	subNodes[QuadNames::TopLeft] = new TreeType(level + 1, new FAABBBox(x - subWidth, y, x, y + subHeight));
	subNodes[QuadNames::TopRight] = new TreeType(level + 1, new FAABBBox(x, y, x + subWidth, y + subHeight));
	subNodes[QuadNames::BottomLeft] = new TreeType(level + 1, new FAABBBox(x - subWidth, y - subHeight, x, y));
	subNodes[QuadNames::BottomRight] = new TreeType(level + 1, new FAABBBox(x, y - subHeight, x + subWidth, y));
}

template<typename ElementType, int32 NodeCapacity>
inline void TQuadTree<ElementType, NodeCapacity>::Insert(const ElementType& Element, const FAABBBox& Box)
{
	//if (nodes[0] != NULL) {
	if (nodes.empty() == false)
	{
		const int32 QuadsIndex = GetQuadsIndex(Box);

		if (QuadsIndex != -1) {
			subNodes[QuadsIndex]->Insert(Element, Box);
			return;
		}
	}

	nodes.emplace_back(Element, Box);

	if (nodes.size() > MAX_OBJECTS && level < MAX_LEVELS)
	{
		if (subNodes[0] == NULL)
		{
			Split();
		}

		int i = 0;
		while (i < nodes.size())
		{
			int index = GetQuadsIndex(nodes[i].Box);
			if (index != -1)
			{
				subNodes[index]->Insert(nodes[i].Element, nodes[i].Box);
				nodes.erase(nodes.begin() + i);
			}
			else {
				i++;
			}
		}
	}
}

template<typename ElementType, int32 NodeCapacity>
inline void TQuadTree<ElementType, NodeCapacity>::Clear()
{
	nodes.clear();
	for (int i = 0; i < QUADTREE_MAX_SUBNODE_SIZE; i++) {
		if (subNodes[i] != nullptr) {
			subNodes[i]->Clear();
			delete subNodes[i];
			subNodes[i] = nullptr;
		}
	}

	IsleafNode = true;
}

template<typename ElementType, int32 NodeCapacity>
inline std::vector<ElementType> TQuadTree<ElementType, NodeCapacity>::GetElements(const FAABBBox& Box, std::vector<ElementType>& ElementsOut)
{
	int index = GetQuadsIndex(Box);
	if (index != -1 && subNodes[0] != nullptr) {
		subNodes[index]->GetElements(Box, ElementsOut);
	}

	for (const FNode& node : nodes)
	{
		ElementsOut.emplace_back(node.Element);
	}

	return ElementsOut;
}

template<typename ElementType, int32 NodeCapacity>
inline int32 TQuadTree<ElementType, NodeCapacity>::GetQuadsIndex(const FAABBBox& Box) const
{
	int32 QuadCount = -1;
	if (IsleafNode == false)
	{
		bool bNegX = Box.GetMinVector().x <= centerPosition.x;
		bool bNegY = Box.GetMinVector().y <= centerPosition.y;

		bool bPosX = Box.GetMaxVector().x >= centerPosition.x;
		bool bPosY = Box.GetMaxVector().y >= centerPosition.y;

		if (bNegX && bNegY)
		{
			QuadCount = QuadNames::BottomLeft;
		}

		if (bPosX && bNegY)
		{
			QuadCount = QuadNames::BottomRight;
		}

		if (bNegX && bPosY)
		{
			QuadCount = QuadNames::TopLeft;
		}

		if (bPosX && bPosY)
		{
			QuadCount = QuadNames::TopRight;
		}
	}

	return QuadCount;
}
