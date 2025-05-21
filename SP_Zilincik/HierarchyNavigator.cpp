#include "HierarchyNavigator.h"
#include "Colors.h"  // pridaj pre farby

using Hierarchy = ds::amt::MultiWayExplicitHierarchy<UzemnaJednotka*>;
using HierarchyBlock = ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>;

void HierarchyNavigator::moveToParent()
{
    if (currentPosition_->parent_ != nullptr) {
        currentPosition_ = static_cast<HierarchyBlock*>(hierarchy_->accessParent(*currentPosition_));
        std::cout << COLOR_INFO << "\n[INFO]" << COLOR_RESET << " You moved to: "
            << currentPosition_->data_->getName() << "\n";
    }
    else {
        std::cout << COLOR_INFO << "\n[INFO]" << COLOR_RESET << " You're already at the root.\n";
    }
}

void HierarchyNavigator::moveToChild(size_t index)
{
    const size_t sonsCount = hierarchy_->degree(*currentPosition_);
    if (index < sonsCount) {
        HierarchyBlock* child = hierarchy_->accessSon(*currentPosition_, index);
        if (child) {
            currentPosition_ = child;
            std::cout << COLOR_INFO << "\n[INFO]" << COLOR_RESET << " You moved to: "
                << currentPosition_->data_->getName() << "\n";
        }
        else {
            std::cout << COLOR_ERROR << "\n[ERROR]" << COLOR_RESET << " Son on given index doesn't exist.\n";
        }
    }
    else {
        std::cout << COLOR_ERROR << "\n[ERROR]" << COLOR_RESET << " Invalid son index.\n";
    }
}

void HierarchyNavigator::listChildren() const
{
    const size_t sonsCount = hierarchy_->degree(*currentPosition_);
    if (sonsCount == 0) {
        std::cout << COLOR_INFO << "\n[INFO]" << COLOR_RESET << " This node has no children.\n";
        return;
    }

    std::cout << COLOR_INFO << "\n[INFO]" << COLOR_RESET << " Children of current node:\n";
    for (size_t i = 0; i < sonsCount; ++i) {
        HierarchyBlock* son = hierarchy_->accessSon(*currentPosition_, i);
        if (son && son->data_) {
            std::cout << "  [" << i << "] " << son->data_->getName() << "\n";
        }
    }
}

void HierarchyNavigator::clearHierarchy()
{
    if (!hierarchy_) {
        std::cerr << COLOR_ERROR << "[ERROR]" << COLOR_RESET << " Hierarchy does not exist.\n";
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
