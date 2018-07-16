#include "CriticalSectionWrap.h"
#include <assert.h>

CCriticalSectionWrap::CCriticalSectionWrap(LPCRITICAL_SECTION mutex)
{
	assert(mutex);
	_mutex = mutex;
	lock();
}

CCriticalSectionWrap::~CCriticalSectionWrap()
{
	unlock();
}

void CCriticalSectionWrap::lock()
{
	EnterCriticalSection(_mutex);
}

void CCriticalSectionWrap::unlock()
{
	LeaveCriticalSection(_mutex);
}

