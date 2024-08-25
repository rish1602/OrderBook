#pragma once

inline uint64_t unix_time()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	
	//this line returns the time elapsed in nanoseconds.
	//std::chrono::system_clock::now().time_since_epoch()   returns the time elapsed since jan 1 1970
	//std::chrono::duration_cast<std::chrono::nanoseconds>  converts the time in nanoseconds
	//.count() metond converts the nanoseconds time as integer value
}
