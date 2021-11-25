#ifndef PTK_UTILITIES_STATISTICS
#define PTK_UTILITIES_STATISTICS

#include <chrono>
#include <map>
#include <mutex>
#include <string>
#include <vector>


///
// Example usage:
//
// #define ENABLE_STATISTICS 1 // Turn on/off the statistics calculation
// #include <utils/Statistics.h>
//
// double my_distance = measureDistance();
// utils::DebugStatsCollector distance_stat("Distance measurement");
// distance_stat.AddSample(my_distance);
//
// std::cout << utils::Statistics::Print();

namespace ptk {
namespace utils {
class Collectable {

	public:
		Collectable() = default;
		~Collectable() = default;

		

};

/**
 * @brief Should evaluate over a collection of collectables
 * 
 */

class Statistics {

	public:
		Statistics(const std::string& name_)
			:	name(name_) {}

		virtual void addCollectable(Collectable* collectable) = 0;

	private:
		const std::string name;
};

}  // namespace utils
}  // namespace ptk

#endif