#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include "UzemnaJednotka.h"

class HierarchyNavigator {
private:
	ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy_;
	ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* currentPosition_;
	
public:
	HierarchyNavigator(ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>* hierarchy)
		: hierarchy_(hierarchy), currentPosition_(hierarchy->accessRoot()) {}
	
	void moveToParent();
	void moveToChild(size_t index);
	void listChildren();
	ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>* getCurrent() const { return currentPosition_; }
	void clearHierarchy();
};

