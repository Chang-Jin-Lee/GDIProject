// SmartCast.h
#pragma once

#include <memory>
#include <type_traits>

namespace Utils
{
    // shared_ptr<T> ¡æ weak_ptr<U>
    template <typename To, typename From>
    std::enable_if_t<std::is_base_of_v<To, From>, std::weak_ptr<To>>
        Cast(const std::shared_ptr<From>& src)
    {
        return std::weak_ptr<To>(std::dynamic_pointer_cast<To>(src));
    }

    // weak_ptr<T> ¡æ shared_ptr<U>
    template <typename To, typename From>
    std::enable_if_t<std::is_base_of_v<To, From>, std::shared_ptr<To>>
        Cast(const std::weak_ptr<From>& src)
    {
        return std::dynamic_pointer_cast<To>(src.lock());
    }

    // raw pointer ¡æ raw pointer
    template <typename To, typename From>
    To* Cast(From* src)
    {
        return dynamic_cast<To*>(src);
    }
}

using namespace Utils;