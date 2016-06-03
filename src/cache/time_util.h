#include <chrono>

namespace util{
	inline std::chrono::time_point<std::chrono::steady_clock> get_time() { return std::chrono::steady_clock::now(); }
	
	inline double get_diff_ms(std::chrono::time_point<std::chrono::steady_clock> end, 
								std::chrono::time_point<std::chrono::steady_clock> start)
	{ return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); }
}