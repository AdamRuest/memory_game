#include "sleep_ms.h"

#include <chrono>
#include <thread>

void sleep_ms(size_t millisecondes)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(millisecondes));
}

