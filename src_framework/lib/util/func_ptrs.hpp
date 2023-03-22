#pragma once

#include <common.hpp>

namespace exl::util {

    template<typename R, typename... A>
    using GenericFuncPtr = R(*)(A...);

    template<typename>
    struct FuncPtrTraits {};

    template<typename R, typename... A>
    struct FuncPtrTraits<R (*)(A...)> {
      using CallbackType = R (*)(A...);
    };

    template<typename T, typename R, typename... A>
    struct FuncPtrTraits<R (T::*)(A...)> {
      using CallbackType = R (*)(T*, A...);
    };

    template<typename T, typename R, typename... A>
    struct FuncPtrTraits<R (T::*)(A...) const> {
      using CallbackType = R (*)(const T*, A...);
    };
}
