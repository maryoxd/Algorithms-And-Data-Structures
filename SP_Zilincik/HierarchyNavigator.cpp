#include "HierarchyNavigator.h"

void HierarchyNavigator::moveToParent()
{
    if (currentPosition_->parent_ != nullptr) {
        currentPosition_ = static_cast<ds::amt::MultiWayExplicitHierarchyBlock<UzemnaJednotka*>*>(hierarchy_->accessParent(*currentPosition_));
        std::cout << "[INFO] Presunul si sa na: " << currentPosition_->data_->getName() << "\n";
    }
    else {
        std::cout << "[INFO] Si už na koreòovom uzle.\n";
    }

}

void HierarchyNavigator::moveToChild(size_t index)
{
    if (index >= 0 && index < hierarchy_->degree(*currentPosition_)) {
        auto* child = hierarchy_->accessSon(*currentPosition_, index);
        if (child) {
            currentPosition_ = child;
            std::cout << "[INFO] Presunul si sa na: " << currentPosition_->data_->getName() << "\n";
        }
        else {
            std::cout << "[ERROR] Syn na danom indexe neexistuje.\n";
        }
    }
    else {
        std::cout << "[ERROR] Neplatný index syna.\n";
    }
}

void HierarchyNavigator::listChildren()
{
    size_t sonsCount = hierarchy_->degree(*currentPosition_);
    if (sonsCount == 0) {
        std::cout << "[INFO] Tento vrchol nemá žiadne deti.\n";
        return;
    }

    std::cout << "Synovia aktuálnej pozície:\n";
    for (size_t i = 0; i < sonsCount; ++i) {
        auto* son = hierarchy_->accessSon(*currentPosition_, i);
        if (son && son->data_) {
            std::cout << "[" << i << "] " << son->data_->getName() << "\n";
        }
    }
}
