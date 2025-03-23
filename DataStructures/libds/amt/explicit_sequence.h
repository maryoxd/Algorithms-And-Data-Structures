#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/sequence.h>

namespace ds::amt {

    template<typename BlockType>
    class ExplicitSequence :
        public Sequence<BlockType>,
        public ExplicitAMS<BlockType>
    {
    public:
        ExplicitSequence();
        ExplicitSequence(const ExplicitSequence& other);
        ~ExplicitSequence() override;

        AMT& assign(const AMT& other) override;
        void clear() override;
        bool equals(const AMT& other) override;

        size_t calculateIndex(BlockType& data) override;

        BlockType* accessFirst() const override;
        BlockType* accessLast() const override;
        BlockType* access(size_t index) const override;
        BlockType* accessNext(const BlockType& block) const override;
        BlockType* accessPrevious(const BlockType& block) const override;

        BlockType& insertFirst() override;
        BlockType& insertLast() override;
        BlockType& insert(size_t index) override;
        BlockType& insertAfter(BlockType& block) override;
        BlockType& insertBefore(BlockType& block) override;

        void removeFirst() override;
        void removeLast() override;
        void remove(size_t index) override;
        void removeNext(const BlockType& block) override;
        void removePrevious(const BlockType& block) override;

    protected:
        virtual void connectBlocks(BlockType* previous, BlockType* next);
        virtual void disconnectBlock(BlockType* block);

        BlockType* first_;
        BlockType* last_;

    public:
        using DataType = typename BlockType::DataT;

        class ExplicitSequenceIterator
        {
        public:
            explicit ExplicitSequenceIterator(BlockType* position);
            ExplicitSequenceIterator(const ExplicitSequenceIterator& other);
            ExplicitSequenceIterator& operator++();
            ExplicitSequenceIterator operator++(int);
            bool operator==(const ExplicitSequenceIterator& other) const;
            bool operator!=(const ExplicitSequenceIterator& other) const;
            DataType& operator*();

        private:
            BlockType* position_;
        };

        ExplicitSequenceIterator begin();
        ExplicitSequenceIterator end();

        using IteratorType = ExplicitSequenceIterator;
    };

    template<typename BlockType>
    using ES = ExplicitSequence<BlockType>;

    //----------

    template<typename DataType>
    struct SinglyLinkedSequenceBlock :
        public MemoryBlock<DataType>
    {

        SinglyLinkedSequenceBlock() : next_(nullptr) {}
        ~SinglyLinkedSequenceBlock() { next_ = nullptr; }

        SinglyLinkedSequenceBlock<DataType>* next_;
    };
    template<typename DataType>
    using SLSBlock = SinglyLinkedSequenceBlock<DataType>;

    template<typename DataType>
    class SinglyLinkedSequence :
        public ES<SLSBlock<DataType>>
    {
    public:
        using BlockType = SinglyLinkedSequenceBlock<DataType>;
    };

    template<typename DataType>
    using SinglyLS = SinglyLinkedSequence<DataType>;

    template<typename DataType>
    class SinglyCyclicLinkedSequence :
        public SinglyLS<DataType>
    {
    };

    template<typename DataType>
    using SinglyCLS = SinglyCyclicLinkedSequence<DataType>;

    //----------

    template<typename DataType>
    struct DoublyLinkedSequenceBlock :
        public SLSBlock<DataType>
    {

        DoublyLinkedSequenceBlock() : previous_(nullptr) {}
        ~DoublyLinkedSequenceBlock() { previous_ = nullptr; }

        DoublyLinkedSequenceBlock<DataType>* previous_;
    };

    template<typename DataType>
    using DLSBlock = DoublyLinkedSequenceBlock<DataType>;

    template<typename DataType>
    class DoublyLinkedSequence :
        public ES<DLSBlock<DataType>>
    {
    public:
        using BlockType = DLSBlock<DataType>;

        BlockType* access(size_t index) const override;
    	BlockType* accessPrevious(const BlockType& block) const override;

        void removeFirst() override;

    protected:
        void connectBlocks(BlockType* previous, BlockType* next) override;
        void disconnectBlock(BlockType* block) override;
    };

    template<typename DataType>
    using DoublyLS = DoublyLinkedSequence<DataType>;

    //----------

    template<typename DataType>
    class DoublyCyclicLinkedSequence :
        public DoublyLS<DataType>
    {
    };

    template<typename DataType>
    using DoublyCLS = DoublyCyclicLinkedSequence<DataType>;

    //----------

    template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence() :
        first_(nullptr),
        last_(nullptr)
    {
    }

    template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence(const ExplicitSequence& other) :
        ExplicitSequence()
    {
        this->assign(other);
    }

    template<typename BlockType>
    ExplicitSequence<BlockType>::~ExplicitSequence()
    {
        clear();
        
    }

    template<typename BlockType>
    AMT& ExplicitSequence<BlockType>::assign(const AMT& other)
    {
        if (this != &other)
        {
            this->clear();

            const ExplicitSequence<BlockType>& otherExplicitSequence = dynamic_cast<const ExplicitSequence<BlockType>&>(other);
            otherExplicitSequence.processAllBlocksForward([&](const BlockType* b)
            {
                this->insertLast().data_ = b->data_;
            });
        }

        return *this;
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::clear()
    {
        BlockType* current = this->accessFirst();
        while (current != nullptr)
        {
			BlockType* next = this->accessNext(*current);
            AMS<BlockType>::memoryManager_->releaseMemory(current);
            current = next;
        }
        first_ = nullptr;
		last_ = nullptr;

    }

    template<typename BlockType>
    bool ExplicitSequence<BlockType>::equals(const AMT& other)
    {
        if (this == &other) {
            return true;
        }

        if (this->size() != other.size()) {
            return false;
        }

        const ExplicitSequence<BlockType>* otherSequence =
			dynamic_cast<const ExplicitSequence<BlockType>*>(&other);

        if (otherSequence == nullptr) {
            return false;
        }

		BlockType* my = this->accessFirst();
		BlockType* his = otherSequence->accessFirst();
        while (my != nullptr) {

            if (my->data_ != his->data_) {
                return false;
            }

			my = this->accessNext(*my);
			his = otherSequence->accessNext(*his);
        }
        return true;

    }

    template<typename BlockType>
    size_t ExplicitSequence<BlockType>::calculateIndex(BlockType& data)
    {
        size_t index = 0;
		BlockType* current = this->accessFirst();
        while (current != nullptr && current != &data) {
            ++index;
			current = this->accessNext(*current);
        }
        return current == nullptr
			? INVALID_INDEX : index;
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessFirst() const
    {
        return first_;
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessLast() const
    {
        return last_;
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::access(size_t index) const
    {
        if (index >= this->size()) {
            return nullptr;
        }

        BlockType* current = this->accessFirst();
        for (size_t i = 0; i < index; ++i) {
            current = this->accessNext(*current);
        }
        return current;
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessNext(const BlockType& block) const
    {
        return static_cast<BlockType*>(block.next_);
    }

    template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessPrevious(const BlockType& block) const
    {
        return this->findBlockWithProperty([&](BlockType* b)
            {
                return b->next_ == &block;
            });
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertFirst()
    {
        if (this->size() == 0) {
            first_ = (last_ = AMS<BlockType>::memoryManager_->allocateMemory());
            return *first_;
        }
        return this->insertBefore(*first_);
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertLast()
    {
        if (this->size() == 0) {

			first_ = (last_ = AMS<BlockType>::memoryManager_->allocateMemory());
            return *last_;
        }
        return this->insertAfter(*last_);
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insert(size_t index)
    {
        return index == 0
            ? this->insertFirst()
            : index == this->size()
                ? this->insertLast()
                : this->insertAfter(*this->access(index - 1));
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertAfter(BlockType& block)
    {
        BlockType* next = this->accessNext(block);
		BlockType* newBlock = AMS<BlockType>::memoryManager_->allocateMemory();

		this->connectBlocks(&block, newBlock);
		this->connectBlocks(newBlock, next);
		
		if (&block == last_)
		{
			last_ = newBlock;
		}
		return *newBlock;
    }

    template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertBefore(BlockType& block)
    {
		BlockType* previous = this->accessPrevious(block);
		BlockType* newBlock = AMS<BlockType>::memoryManager_->allocateMemory();

		this->connectBlocks(previous, newBlock);
		this->connectBlocks(newBlock, &block);

		if (first_ == &block)
		{
			first_ = newBlock;
		}
        return *newBlock;
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removeFirst()
    {
        if (first_ == last_) {
            AMS<BlockType>::memoryManager_->releaseMemory(first_);
			first_ = (last_ = nullptr);
        }
        else {
            BlockType* newFirst_ = this->accessNext(*first_);
			first_ = newFirst_;
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removeLast()
    {
        if (first_ == last_) {
            AMS<BlockType>::memoryManager_->releaseMemory(last_);
            first_ = (last_ = nullptr);
        }
        else {
            BlockType* newLast = this->accessPrevious(*last_);
			AMS<BlockType>::memoryManager_->releaseMemory(last_);
			last_ = newLast;
			last_->next_ = nullptr;
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::remove(size_t index)
    {
        if (index == 0)
        {
            this->removeFirst();
        }
        else
        {
            this->removeNext(*this->access(index - 1));
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removeNext(const BlockType& block)
    {
		BlockType* removing = this->accessNext(block);
        if (removing == last_) {
            this->removeLast();
        }
        else {
			this->disconnectBlock(removing);
			AMS<BlockType>::memoryManager_->releaseMemory(removing);
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::removePrevious(const BlockType& block)
    {
		BlockType* removing = this->accessPrevious(block);
        if (removing == first_) {
            this->removeFirst();
        }
        else {
            this->disconnectBlock(removing);
            AMS<BlockType>::memoryManager_->releaseMemory(removing);
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::connectBlocks(BlockType* previous, BlockType* next)
    {
        if (previous != nullptr)
        {
            previous->next_ = next;
        }
    }

    template<typename BlockType>
    void ExplicitSequence<BlockType>::disconnectBlock(BlockType* block)
    {
        this->connectBlocks(this->accessPrevious(*block), this->accessNext(*block));
        block->next_ = nullptr;
    }

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(BlockType* position) :
        position_(position)
    {
    }

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(
        const ExplicitSequenceIterator& other
    ) :
        position_(other.position_)
    {
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator& ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++()
    {
        position_ = static_cast<BlockType*>(position_->next_);
		return *this;
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++(int)
    {
        ExplicitSequenceIterator tmp(*this);
    	this->operator++();
        return tmp;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator==(const ExplicitSequenceIterator& other) const
    {
		return position_ == other.position_;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator!=(const ExplicitSequenceIterator& other) const
    {
        return !(*this == other);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::DataType& ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator*()
    {
		return position_->data_;
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::begin()
    {
        return ExplicitSequenceIterator(first_);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::end()
    {
        return ExplicitSequenceIterator(nullptr);
    }

    template<typename DataType>
    typename DoublyLinkedSequence<DataType>::BlockType* DoublyLinkedSequence<DataType>::access(size_t index) const
    {
        // TODO 04
        // po implementacii vymazte vyhodenie vynimky!
        throw std::runtime_error("Not implemented yet");
    }

    template<typename DataType>
    typename DoublyLinkedSequence<DataType>::BlockType* DoublyLinkedSequence<DataType>::accessPrevious(const BlockType& block) const
    {
        return block.previous_;
    }

    template<typename DataType>
    void DoublyLinkedSequence<DataType>::removeFirst()
    {
        ExplicitSequence<BlockType>::removeFirst();

        if (ExplicitSequence<BlockType>::first_ != nullptr)
        {
            ExplicitSequence<BlockType>::first_->previous_ = nullptr;
        }
    }

    template<typename DataType>
    void DoublyLinkedSequence<DataType>::connectBlocks(BlockType* previous, BlockType* next)
    {
        ExplicitSequence<BlockType>::connectBlocks(previous, next);

        if (next != nullptr)
        {
            next->previous_ = const_cast<BlockType*>(previous);
        }
    }

    template<typename DataType>
    void DoublyLinkedSequence<DataType>::disconnectBlock(BlockType* block)
    {
        ExplicitSequence<BlockType>::disconnectBlock(block);
        block->previous_ = nullptr;
    }

}