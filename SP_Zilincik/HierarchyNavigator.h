#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include "UzemnaJednotka.h"

class HierarchyNavigator {
private:
	using Hierarchy = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>;
	using HierarchyBlock = ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>;

	Hierarchy* hierarchy_;
	HierarchyBlock* currentPosition_;

public:
	explicit HierarchyNavigator(Hierarchy* hierarchy)
		: hierarchy_(hierarchy), currentPosition_(hierarchy->accessRoot()) {
	}

	void moveToParent();
	void moveToChild(size_t index);

	void listChildren() const;

	HierarchyBlock* getCurrent() const { return currentPosition_; }

	void clearHierarchy();
};
