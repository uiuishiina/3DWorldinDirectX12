#include"FreeList.h"
#include <numeric>
#include"../debug/debugsystem.h"

[[nodiscard]] bool FreeList::CreateList(size_t ListSize) {
	if (!FreeList_.empty()) {
		return false;
	}

	std::iota(FreeList_.begin(), FreeList_.end(), 0);

	return true;
}

[[nodsicard]] std::optional<size_t> FreeList::GetFreeIndex() {
	DEBUG_ASSERT(!FreeList_.empty());
	const auto index = FreeList_.back();
	FreeList_.pop_back();
	return index;
}

void FreeList::ReturnIndex(size_t index) {
	DEBUG_ASSERT(!FreeList_.empty());
	FreeList_.push_back(index);
}