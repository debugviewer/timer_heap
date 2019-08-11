#include <stdio.h>
#include <atomic>
#include "Heap.h"
#include "TimerHeap.h"

typedef int KEY_TYPE;

std::atomic<bool> _continue(true);

void timer_func(void *data)
{
	UserData * pData = (UserData*)data;
	printf("event id = %d\n", pData->m_eventId);
}

void test_timer_heap() {
	printf("timer wheel loop begin......\n");
	TimerHeap _heap;
	_heap.setTimer(TimerEntry(100, 1000, timer_func, LOOP_TIMER));
	for (int i = 1; i <= 5; i++) {
		const uint64_t _interval = i * 2000;
		_heap.setTimer(TimerEntry(i, _interval, timer_func, ONCE_TIMER));
	}
	
	while (_continue) {
		_heap.timerLoop();
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}
	
	printf("timer wheel loop end......\n");
}

void test_heap_sort() {
	std::vector<KEY_TYPE> _vec = { 4, 6, 1, 3, 5, 7, 2, 8 };
	CHeap<KEY_TYPE, MIN_HEAP> _minHeap;
	CHeap<KEY_TYPE, MAX_HEAP, false> _maxHeap;
	CHeap<KEY_TYPE, MIN_HEAP, false> _sortMinHeap;
	CHeap<KEY_TYPE, MAX_HEAP, false> _sortMaxHeap;
	
	printf("\nmin heap insert:\n");
	for (size_t i = 0; i < _vec.size(); i++) {
		_minHeap.insert(_vec[i]);
		_minHeap.print();
	}

	_minHeap.deleteTop();
	_minHeap.print();
	_minHeap.insert(20);
	_minHeap.print();

	printf("\nmin heap delete:\n");
	_minHeap.print();
	while (!_minHeap.empty()) {
		_minHeap.deleteTop();
		_minHeap.print();
	}

	printf("\nmax heap insert:\n");
	for (size_t i = 0; i < _vec.size(); i++) {
		_maxHeap.insert(_vec[i]);
		_maxHeap.print();
	}

	printf("\nmax heap delete:\n");
	_maxHeap.print();
	while (!_maxHeap.empty()) {
		_maxHeap.deleteTop();
		_maxHeap.print();
	}

	printf("\nmax heap sort:\n");
	_sortMaxHeap.sort(_vec);
	for (size_t _i = 0; _i < _vec.size(); _i++) {
		printf("%d ", _vec[_i]);
	}

	printf("\nmin heap sort:\n");
	_sortMinHeap.sort(_vec);
	for (size_t _i = 0; _i < _vec.size(); _i++) {
		printf("%d ", _vec[_i]);
	}

	printf("\n\n");
}

int main() {
	printf("main thread begin......\n");
	test_heap_sort();

	std::thread t(test_timer_heap);
	t.detach();
	getchar();
	_continue = false;
	printf("main thread end......\n");
}
