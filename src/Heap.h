#ifndef __HEAP_H__
#define __HEAP_H__

#include <vector>

// 堆类型
typedef enum {
	MIN_HEAP = 0, //最小堆
	MAX_HEAP	  //最大堆
}heap_type;

// 堆模板
template<class _Type, const heap_type _heap_type = MIN_HEAP, const bool _real_del = true>
class CHeap
{
public:
	CHeap() : m_curSize(0),
			  m_topIndex(0),
			  m_heapType(_heap_type){}
	~CHeap() { m_heap.clear(); }

	// 添加元素到堆尾。
	void insert(const _Type& v_) {
		int _idx = empty() ? 0 : getLastIndex() + 1;
		m_heap.insert(m_heap.begin() + _idx, v_);
		m_curSize++;
		up(getLastIndex());
	}

	// 删除堆顶元素
	void deleteTop() {
		_Type _top = getTop();
		std::swap(m_heap[getTopIndex()], m_heap[getLastIndex()]);
		if (_real_del) {
			m_heap.erase(m_heap.end() - 1);
		}
		
		m_curSize--;
		down(getTopIndex());
	}

	// 获取堆顶元素
	_Type& getTop() {
		return m_heap[getTopIndex()];
	}

	bool empty() const {
		return (m_curSize <= 0);
	}

	size_t size() const {
		return (m_curSize);
	}

	void print() {
		if (empty()) {
			printf("<null>\n");
			return;
		}
		for (auto i = getTopIndex(); i <= getLastIndex(); i++) {
			printf("%d ", m_heap[i]);
		}
		printf("\n");
	}

	// 排序，演示用
	void sort(std::vector<_Type> &heap) {
		if (_real_del) {
			printf("不支持排序\n");
			return;
		}

		initWith(heap);
		heapify();
		while (size()) {
			deleteTop();
		}

		m_heap.swap(heap);
		m_curSize = 0;
	}
protected:
	// 上浮
	void up(const int idx_) {
		if (idx_ == getTopIndex()) {
			return;
		}
		int _cur = idx_;
		int _p = getParentIndex(_cur);
		while ((_cur > getTopIndex()) && compare(_p, _cur)) {
			std::swap(m_heap[_cur], m_heap[_p]);
			_cur = _p;
			_p = getParentIndex(_cur);
		}
	}

	// 下沉
	void down(const int idx_) {
		int _cur = idx_;
		int _child = getLeftIndex(_cur);

		while (!outRange(_child))
		{
			if ((!outRange(_child)) && compare(_child, _child + 1)) {
				_child++;
			}

			if (!compare(_cur, _child)) {
				break;
			}

			std::swap(m_heap[_cur], m_heap[_child]);
			_cur = _child;
			_child = getLeftIndex(_cur);
		}
	}

	// 比较两个结点是否满足上浮或者下沉的条件
	// 比较结点为父子关系时，lhs表示父结点，rhs表示子结点
	bool compare(const size_t lhs, const size_t rhs) const {
		if (outRange(lhs) || outRange(rhs)) {
			return false;
		}

		// 最小堆比较
		if ((m_heap[lhs] > m_heap[rhs]) && (m_heapType == MIN_HEAP)) {
			return true;
		}

		// 最大堆比较
		if ((m_heap[lhs] < m_heap[rhs]) && (m_heapType == MAX_HEAP)) {
			return true;
		}

		return false;
	}

	bool outRange(const int idx_) const {
		if (idx_ >= getTopIndex() + m_curSize) {
			return true;
		}

		return false;
	}

	// 堆积化
	void heapify() {
		if (empty()) {
			return;
		}
		int _i = 0;
		for (_i = (int)(getLastIndex() / 2); _i >= (int)getTopIndex(); --_i) {
			down(_i);
		}
	}

	void initWith(std::vector<_Type> &heap) {
		m_heap.clear();
		m_heap.swap(heap);
		m_heap.insert(m_heap.begin(), getTopIndex(), _Type());
		m_curSize = m_heap.size() - getTopIndex();
	}
private:
	int getParentIndex(const int idx_) const {
		return (idx_ - 1 + getTopIndex()) / 2;
	}

	int getLeftIndex(const int idx_) const {
		return  2 * idx_ + 1 - getTopIndex();
	}

	int getRightIndex(const int idx_) const {
		return  2 * idx_ + 2 - getTopIndex();
	}

	int getTopIndex() const {
		return m_topIndex;
	}

	// 为空时返回-1，先判断empty，再调用此接口
	int getLastIndex() const {
		return m_topIndex + m_curSize - 1;
	}
private:
	int m_curSize;
	int m_topIndex;
	const heap_type m_heapType;
	std::vector<_Type> m_heap;
};

#endif