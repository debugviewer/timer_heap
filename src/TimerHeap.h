#pragma once
#include <vector>
#include <functional>
#include <chrono>
#include <thread>
#include "Heap.h"

typedef std::function<void(void *)> TimerCallback;

typedef enum {
	ONCE_TIMER = 0,
	LOOP_TIMER
}timer_type;

class UserData {
public:
	UserData() : m_eventId(0) {}
	int m_eventId;
};

inline uint64_t getTimeStamp() {
	auto _now = std::chrono::system_clock::now();
	return std::chrono::duration_cast<std::chrono::milliseconds>(_now.time_since_epoch()).count();
}

class TimerEntry {
public:
	TimerEntry(int id_, const uint64_t interval_, const TimerCallback func_, timer_type type_ = ONCE_TIMER) :
		m_interval(interval_),
		m_timerFun(func_),
		m_loopType(type_)
	{
		m_data.m_eventId = id_;
		m_expires = getTimeStamp() + interval_;
	}
	TimerEntry() :m_interval(0), m_expires(0), m_timerFun(nullptr), m_loopType(ONCE_TIMER) {}

	TimerEntry& operator= (const TimerEntry& other) {
		if (this == &other) {
			return *this;
		}

		m_interval = other.m_interval;
		m_timerFun = other.m_timerFun;
		m_loopType = other.m_loopType;
		m_expires = other.m_expires;
		m_data.m_eventId = other.m_data.m_eventId;
		
		return *this;
	}

	TimerEntry(const TimerEntry& other) :
		m_interval(other.m_interval),
		m_timerFun(other.m_timerFun),
		m_loopType(other.m_loopType) {
		if (this == &other) {
			return;
		}
		m_data.m_eventId = other.m_data.m_eventId;
		m_expires = other.m_expires;
	}

	~TimerEntry() {}
	bool operator<(const TimerEntry& d)const {
		return this->m_expires < d.m_expires;
	}
	bool operator>(const TimerEntry& d)const {
		return this->m_expires > d.m_expires;
	}

	bool loop() const { return m_loopType == LOOP_TIMER; }
	uint64_t expires() const { return m_expires; }

	void reset() {
		m_expires = getTimeStamp() + m_interval;
	}

	void OnTimer() {
		if (m_timerFun) {
			m_timerFun(&m_data);
		}
	}
	protected:
	timer_type m_loopType;
	uint64_t m_interval;//milliseconds
	uint64_t m_expires;//milliseconds
	UserData m_data;
	TimerCallback m_timerFun;
};

class TimerHeap {
public:
	TimerHeap() {}
	~TimerHeap() {}
	void setTimer(const TimerEntry& timer_) {
		m_timerHeap.insert(timer_);
	}

	void killTimer(const int id_) {}

	void timerLoop() {
		const uint64_t _now_time = getTimeStamp();
		while (!m_timerHeap.empty()) {
			TimerEntry _timer = m_timerHeap.getTop();
			if (_timer.expires() > _now_time) {
				break;
			}
			
			m_timerHeap.deleteTop();
			_timer.OnTimer();

			if (_timer.loop()) {
				_timer.reset();
				setTimer(_timer);
			}	
		}
	}
private:
	CHeap<TimerEntry, MIN_HEAP> m_timerHeap;
};