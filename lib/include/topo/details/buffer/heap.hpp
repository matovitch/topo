#pragma once

#include "topo/details/buffer/abstract.hpp"

#include <type_traits>
#include <cstdint>

namespace topo_details
{

namespace buffer
{

template <class Traits>
class THeap : public Traits::Abstract
{
    using TypeStorage = typename Traits::TypeStorage;
    using Type        = typename Traits::Type;

public:

    THeap(const std::size_t size) :
        _memory{reinterpret_cast<Type*>(new TypeStorage[size])}
    {
        _head = _memory;
        _tail = _memory + size;
    }

    Type* allocate() override
    {
        if (_head == _tail)
        {
            return nullptr;
        }

        return _head++;
    }

    void clean()
    {
        auto temp = _memory;

        while (temp != _head)
        {
            temp->~Type();
            temp++;
        }

        _head = _memory;
    }

    ~THeap()
    {
        clean();
        delete[] _memory;
        _memory = nullptr;
    }

private:

          Type* _head;
    const Type* _tail;
          Type* _memory;
};

namespace heap
{

template <class HeapType>
struct TTraits
{
    using Type        = HeapType;
    using Abstract    = TAbstract<Type>;
    using TypeStorage = std::aligned_storage_t<sizeof  (Type),
                                               alignof (Type)>;
};

template <class Type>
using TMake = THeap<TTraits<Type>>;

} // namespace heap

} // namespace buffer

} // namespace topo_details
