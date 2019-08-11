#ifndef __HEAP_H__
#define __HEAP_H__

#include <vector>

// ������
typedef enum {
	MIN_HEAP = 0, //��С��
	MAX_HEAP	  //����
}heap_type;

// ��ģ��
template<class _Type, const heap_type _heap_type = MIN_HEAP, const bool _real_del = true>
class CHeap
{
public:
	CHeap() : m_curSize(0),
			  m_topIndex(0),
			  m_heapType(_heap_type){}
	~CHeap() { m_heap.clear(); }

	// ���Ԫ�ص���β��
	void insert(const _Type& v_) {
		int _idx = empty() ? 0 : getLastIndex() + 1;
		m_heap.insert(m_heap.begin() + _idx, v_);
		m_curSize++;
		up(getLastIndex());
	}

	// ɾ���Ѷ�Ԫ��
	void deleteTop() {
		_Type _top = getTop();
		std::swap(m_heap[getTopIndex()], m_heap[getLastIndex()]);
		if (_real_del) {
			m_heap.erase(m_heap.end() - 1);
		}
		
		m_curSize--;
		down(getTopIndex());
	}

	// ��ȡ�Ѷ�Ԫ��
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

	// ������ʾ��
	void sort(std::vector<_Type> &heap) {
		if (_real_del) {
			printf("��֧������\n");
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
	// �ϸ�
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

	// �³�
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

	// �Ƚ���������Ƿ������ϸ������³�������
	// �ȽϽ��Ϊ���ӹ�ϵʱ��lhs��ʾ����㣬rhs��ʾ�ӽ��
	bool compare(const size_t lhs, const size_t rhs) const {
		if (outRange(lhs) || outRange(rhs)) {
			return false;
		}

		// ��С�ѱȽ�
		if ((m_heap[lhs] > m_heap[rhs]) && (m_heapType == MIN_HEAP)) {
			return true;
		}

		// ���ѱȽ�
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

	// �ѻ���
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

	// Ϊ��ʱ����-1�����ж�empty���ٵ��ô˽ӿ�
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