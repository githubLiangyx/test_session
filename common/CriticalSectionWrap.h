#pragma once
#include <synchapi.h>

class CCriticalSectionWrap
{
public:
	CCriticalSectionWrap(LPCRITICAL_SECTION mutex);

	CCriticalSectionWrap(const CCriticalSectionWrap&) = delete;

	CCriticalSectionWrap& operator=(const CCriticalSectionWrap&) = delete;

	~CCriticalSectionWrap();

	void lock();

	void unlock();
private:
	LPCRITICAL_SECTION _mutex;
};