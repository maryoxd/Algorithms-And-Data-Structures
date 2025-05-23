#include "HierarchyNavigator.h"

using Hierarchy = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>;
using HierarchyBlock = ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>;

void HierarchyNavigator::moveToParent()
{
    if (currentPosition_->parent_ != nullptr) {
        currentPosition_ = static_cast<HierarchyBlock*>(hierarchy_->accessParent(*currentPosition_));
        std::cout << "\n[INFO] You moved to: " << currentPosition_->data_->getName() << "\n";
    }
    else {
        std::cout << "\n[INFO] You're already at the root.\n";
    }
}

void HierarchyNavigator::moveToChild(size_t index)
{
    const size_t sonsCount = hierarchy_->degree(*currentPosition_);
    if (index < sonsCount) {
        HierarchyBlock* child = hierarchy_->accessSon(*currentPosition_, index);
        if (child) {
            currentPosition_ = child;
            std::cout << "\n[INFO] You moved to: " << currentPosition_->data_->getName() << "\n";   
        }
        else {
            std::cout << "\n[ERROR] Son on given index doesn't exist.\n"; 
        }
    }
    else {
        std::cout << "\n[ERROR] Invalid son index.\n"; 
    }
}

bool HierarchyNavigator::listChildren() const
{
    const size_t sonsCount = hierarchy_->degree(*currentPosition_);
    if (sonsCount == 0) {
        std::cout << "\n[INFO] This node has no sons.\n";
        return false;
    }

    std::cout << "\n[INFO] Sons of the current node:\n";
    for (size_t i = 0; i < sonsCount; ++i) {
        auto* son = hierarchy_->accessSon(*currentPosition_, i);
        if (son && son->data_) {
            std::cout << "  [" << i << "] " << son->data_->getName() << "\n";
        }
    }
    return true;
}

void HierarchyNavigator::clearHierarchy()
{
    if (!hierarchy_) {
        std::cerr << "[ERROR] Hierarchy does not exist.\n";
        return;
    }

    HierarchyBlock* root = hierarchy_->accessRoot();
    if (!root) return;

    std::function<void(HierarchyBlock*)> deleteRecursive = [&](HierarchyBlock* node) {
        if (!node) return;

        const size_t sonsCount = hierarchy_->degree(*node);
        for (size_t i = 0; i < sonsCount; ++i) {
            deleteRecursive(hierarchy_->accessSon(*node, i));
        }

        if (node->data_ && node->data_->getType() != Typ::OBEC) {
            delete node->data_;
            node->data_ = nullptr;
        }
        };

    deleteRecursive(root);
}
