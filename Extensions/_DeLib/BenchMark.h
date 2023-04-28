#pragma once

#include <chrono>
#include <format>
#include <string>
#include <functional>

// OutputDebugString(BenchMark::BenchMark(100000000,
//     [&] (size_t i) {
//         std::stod(l[i % 6]);
//         return true;
//     },
//     [&] (size_t i) {
//         ston<double>(l[i % 6]);
//         return true;
//     }).c_str());

namespace BenchMark {
    using BenchMarkCallback = std::function<bool(size_t)>;

    inline std::wstring BenchMark(const size_t loop,
    const BenchMarkCallback& a, const BenchMarkCallback& b) {
        const auto ms = std::chrono_literals::operator ""ms(1ull);

        auto loopFunction = [&] (const BenchMarkCallback& cb) {
            auto t = std::chrono::steady_clock::now();
            bool ret = false;
            
            for (size_t i = 0; i < loop; i++) {
                ret = cb(i);
            }
            return (std::chrono::steady_clock::now() - t) / ms;
        };

        return std::format(L"A: {}, B: {}", loopFunction(a), loopFunction(b));
    }
};