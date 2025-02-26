#pragma once

#include <libds/mm/memory_manager.h>
#include <libds/mm/memory_omanip.h>
#include <libds/constants.h>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <utility>

namespace ds::mm {

    template<typename BlockType>
    class CompactMemoryManager : public MemoryManager<BlockType> {
    public:

        // KONäTRUKTORY

        CompactMemoryManager(); // Bez parametrick˝
        CompactMemoryManager(size_t size); // S danou poËitatoËnou veækosùou / size_t s˙ bajty
        CompactMemoryManager(const CompactMemoryManager<BlockType>& other); // KopÌrovacÌ konötruktor, vytv·rame novÈho spr·vcu kopÌrovanÌm z inÈho

        // KONäTRUKTORY


        // DEäTRUKTOR

        ~CompactMemoryManager() override; // UkonËenie ûivotnosti objektu

        // DEäTRUKTOR


        // OPER¡CIE

        BlockType* allocateMemory() override;
        BlockType* allocateMemoryAt(size_t index);
        void releaseMemory(BlockType* pointer) override;
        void releaseMemoryAt(size_t index);
        void releaseMemory();

        size_t getCapacity() const;

        CompactMemoryManager<BlockType>& assign(const CompactMemoryManager<BlockType>& other);
        void changeCapacity(size_t newCapacity);
        void shrinkMemory();
        void clear();
        bool equals(const CompactMemoryManager<BlockType>& other) const;
        void* calculateAddress(const BlockType& data);
        size_t calculateIndex(const BlockType& data);
        BlockType& getBlockAt(size_t index);
        void swap(size_t index1, size_t index2);

        void print(std::ostream& os);

        // OPER¡CIE

    private:
        size_t getAllocatedBlocksSize() const;
        size_t getAllocatedCapacitySize() const;

    private:
        BlockType* base_; // Ukazuje na zaËiatok alokovanej pam‰te
        BlockType* end_; // Ukazuje na koniec doteraz vyuûitej Ëasti (prv˝ voæn˝ blok)
        BlockType* limit_; // Ukazuje na celkov˝ koniec alokovanÈho priestoru

        static const size_t INIT_SIZE = 4;
    };

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager() :
        CompactMemoryManager(INIT_SIZE)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(size_t size) :
        base_(static_cast<BlockType*>(std::calloc(size, sizeof(BlockType)))),
        end_(base_),
        limit_(base_ + size)
    {
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::CompactMemoryManager(const CompactMemoryManager<BlockType>& other) :
        CompactMemoryManager(other.getAllocatedBlockCount())
    {
        this->assign(other);
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>::~CompactMemoryManager()
    {
        releaseMemory(base_);
		std::free(base_);
        base_ = nullptr;
        end_ = nullptr;
        limit_ = nullptr;

        for (BlockType* p = base_; p != end_; ++p)
        {
            p->~BlockType();
        }

    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemory()
    {
        return this->allocateMemoryAt(end_ - base_);
    }

    template<typename BlockType>
    BlockType* CompactMemoryManager<BlockType>::allocateMemoryAt(size_t index)
    {
        // Ak som dosiahol limit (mÙj koniec je na celkovom konci pam‰te)
        if (end_ == limit_) {

            this->changeCapacity(2 * this->allocatedBlockCount_);

        }
        // Ak prid·vame blok doprostred, musÌm posun˙ù existuj˙ce bloky aby sa urobilo miesto pre nov˝ blok na tom indexe
        if (end_ - base_ > index) {
            std::memmove(
                base_ + index + 1, // Mieto, kam bloky posunieme o jedno Ôalej v pam‰ti
                base_ + index, // Miesto, odkiaæ sa bud˙ pres˙vaù existuj˙ce bloky
                (end_ - base_ - index) * sizeof(BlockType) // Veækosù ktor˙ pres˙vam
            );


        }

        ++end_;
        ++allocatedBlockCount_;


        return placement_new<BlockType>(base_ + index);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory(BlockType* pointer)
    {
        BlockType* p = pointer; // Lok·lna premenn· na uloûenie
        while (p != end_) { // Idem od bloku pam‰te z parametra aû po aktu·lny koniec
            p->~BlockType(); // Vol·m deötruktor
            p++; // PripoËÌtavam aby som sa pohol v while cykle
        }

        end_ = pointer; // NastavÌm kde je aktu·lny koniec (tam kde bol zaËiatok)
        allocatedBlockCount_ = end_ - base_; // ZmenÌm akt·lny poËet alokovan˝ch blokov (koneËn˝ - prv˝)
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemoryAt(size_t index)
    {
        BlockType& block = this->getBlockAt(index);
        block.~BlockType();
        memmove(
            base_ + index,
            base_ + index + 1,
            (end_ - base_ - index - 1) * sizeof(BlockType)
        );

        end_--;
        allocatedBlockCount_--;
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::releaseMemory()
    {
        releaseMemory(end_ - 1);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getCapacity() const
    {
        return limit_ - base_;
    }

    template<typename BlockType>
    CompactMemoryManager<BlockType>& CompactMemoryManager<BlockType>::assign(const CompactMemoryManager<BlockType>& other)
    {
        if (this != &other)
        {
            this->releaseMemory(base_); // Odstr·nime star˙ pam‰ù
            this->allocatedBlockCount_ = other.getAllocatedBlockCount();

            // Alokujeme nov˙ pam‰ù
            void* newBase =  std::realloc(base_, other.getAllocatedCapacitySize());

            if (newBase == nullptr)
            {
                throw std::bad_alloc(); // Zlyhala alok·cia
            }

            base_ = static_cast<BlockType*>(newBase);
            end_ = base_ + this->allocatedBlockCount_;
            limit_ = base_ + (other.limit_ - other.base_);

            // KopÌrovanie hodnÙt medzi `other.base_` a `base_`
            for (int i = 0; i < this->allocatedBlockCount_; ++i)
            {
                placement_copy(base_ + i, *(other.base_ + i));
            }
        }
        return *this;
    }


    template<typename BlockType>
    void CompactMemoryManager<BlockType>::shrinkMemory()
    {
        size_t newCapacity = static_cast<size_t>(end_ - base_);

        if (newCapacity < CompactMemoryManager<BlockType>::INIT_SIZE)
        {
            newCapacity = CompactMemoryManager<BlockType>::INIT_SIZE;
        }

        this->changeCapacity(newCapacity);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::changeCapacity(size_t newCapacity)
    {
        if (newCapacity == this->getCapacity()) {
            return;
        }

        if (newCapacity < this->allocatedBlockCount_) {
            this->releaseMemory(base_ + newCapacity);
        }

        void* newBase = std::realloc(base_, newCapacity * sizeof(BlockType));

        if (newBase == nullptr)
        {
            throw std::bad_alloc();
        }

        base_ = static_cast<BlockType*>(newBase);
        end_ = base_ + allocatedBlockCount_;
        limit_ = base_ + newCapacity;


    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::clear()
    {
        releaseMemory(base_);
    }

    template<typename BlockType>
    bool CompactMemoryManager<BlockType>::equals(const CompactMemoryManager<BlockType>& other) const
    {
        return (this == &other) ||
            (this->allocatedBlockCount_ == other.allocatedBlockCount_
            && memcmp(this->base_,
                other.base_,
                this->allocatedBlockCount_ * sizeof(BlockType)) == 0);
    }

    template<typename BlockType>
    void* CompactMemoryManager<BlockType>::calculateAddress(const BlockType& data)
    {
        BlockType* p = base_;

        while (p != end_ && p != &data)
        {
            p++;
        }
        if (p == end_)
        {
            return nullptr;
        }

        return p;

    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::calculateIndex(const BlockType& data)
    {
        if (&data < end_ && &data >= base_)
        {
            return &data - base_;
        }

        return std::numeric_limits<size_t>::max(); // neplatny index? (FIX)
    }

    template<typename BlockType>
    BlockType& CompactMemoryManager<BlockType>::getBlockAt(size_t index)
    {
        return *(base_ + index);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::swap(size_t index1, size_t index2)
    {
        std::swap(this->getBlockAt(index1), this->getBlockAt(index2));
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedBlocksSize() const
    {
        return (limit_ - base_) * sizeof(BlockType);
    }

    template<typename BlockType>
    size_t CompactMemoryManager<BlockType>::getAllocatedCapacitySize() const
    {
        return (limit_ - base_) * sizeof(BlockType);
    }

    template<typename BlockType>
    void CompactMemoryManager<BlockType>::print(std::ostream& os)
    {
        os << "first = " << base_ << std::endl;
        os << "last = " << end_ << std::endl;
        os << "limit = " << limit_ << std::endl;
        os << "block size = " << sizeof(BlockType) << "B" << '\n';

        BlockType* ptr = base_;
        while (ptr != limit_)
        {
            std::cout << ptr;
            os << PtrPrintBin<BlockType>(ptr);

            if (ptr == base_) {
                os << "<- first";
            }
            else if (ptr == end_) {
                os << "<- last";
            }
            os << '\n';
            ++ptr;
        }

        os << limit_ << "|<- limit" << std::endl;
    }

}