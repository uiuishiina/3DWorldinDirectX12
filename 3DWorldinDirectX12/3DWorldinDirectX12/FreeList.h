#pragma once
#include<vector>
#include<optional>

class FreeList final
{
	std::vector<size_t> FreeList_{};
public:
	FreeList() = default;
	~FreeList() = default;

	[[nodiscard]] bool CreateList(size_t ListSize);

	[[nodsicard]] std::optional<size_t> GetFreeIndex();

	void ReturnIndex(size_t index);
};