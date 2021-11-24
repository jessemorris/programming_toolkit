#pragma once

#include <chrono>
#include <memory>

namespace ptk {
namespace utils {


/**
 * @brief Timer class to measure the time between two points in the program.
 * 
 */
class Timer {
    public:
        /**
         * @brief Gets the start point.
         * 
         * @return std::chrono::high_resolution_clock::time_point 
         */
        static std::chrono::high_resolution_clock::time_point tic() {
            return std::chrono::high_resolution_clock::now();
        }

        /**
         * @brief Stop timer and report duration in given time.
         * 
         * Call .count() on returned duration to have number of ticks.
         * 
         * @tparam T 
         * @param start The timepoint as given by tic().
         * @return T Returns duration in milliseconds by default.
         */
        template <typename T = std::chrono::milliseconds>
        static T toc(const std::chrono::high_resolution_clock::time_point& start) {
            return std::chrono::duration_cast<T>(
                std::chrono::high_resolution_clock::now() - start);
        }
    };

/**
 * @brief Measures the execution time of a function.
 * 
 * Usage: measure<>::execution(function, arguments)
 * 
 * @tparam T = std::chrono::milliseconds
 */
template <typename T = std::chrono::milliseconds>
struct measure {

    /**
     * @brief Executes a function and measures the timing in T
     * 
     * @tparam F function type.
     * @tparam Args The function arguments.
     * @param func The function pointer.
     * @param args The arguments
     * @return T::rep The number of counts.
     */
    template <typename F, typename... Args>
    static typename T::rep execution(F&& func, Args&&... args) {
        auto start = std::chrono::steady_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration =
            std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - start);
        return duration.count();
    }
};


}
}