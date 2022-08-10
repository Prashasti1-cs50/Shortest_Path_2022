#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <utility>
#include <vector>
#include "Graph.h"

template<typename TKey, typename TValue>
class MinHeap
{
private:
    // array for the elements which should be heap-sorted
    std::vector<std::pair<TKey, TValue>> m_vec;

public:
    MinHeap() {}

	void Push(TKey key, TValue value) {			//push new dictionary
		this->m_vec.push_back(make_pair(key, value));
		if (this->m_vec.size() == 1) return;
		Heapify(0);	//heap sort heapify function call
	}
	   
	void Pop() {				//pop function (root)
		std::pair<TKey, TValue> p = this->m_vec.back();
		std::vector<pair<TKey, TValue>> v;
		v.push_back(make_pair(p.first, p.second));	//
		this->m_vec[this->m_vec.size() - 1] = this->m_vec[0];
		this->m_vec[0] = v[0];

		this->m_vec.pop_back();

		Heapify(0);	//heap sort heapify function call
	}
    /// <summary>
    /// get the minimum element
    /// </summary>
    ///
    /// <returns>
    /// the minimum element
    /// </returns>
	std::pair<TKey, TValue> Top() {
		std::pair<TKey, TValue> p = this->m_vec.front();	//get top node of min heap
		return p;
	}
    /// <summary>
    /// get the key-value pair which the value is the same as the target
    /// </summary>
    ///
    /// <returns>
    /// the key-value pair which the value is the same as the target
    /// </returns>
	std::pair<TKey, TValue> Get(TValue target) {	//return target node
		std::pair<TKey, TValue> p;
		int cnt = 0;
		int end = this->m_vec.size();
		while (1) {
			p = this->m_vec[cnt];
			if (p.second == target) return p;		//if found target
			else {
				if (cnt == end - 1) {
					p.first = IN_FINITY;
					p.second = IN_FINITY;
					return p;
				}
				else cnt++;
			}
		}
	}
    /// <summary>
    /// check whether this heap is empty or not
    /// </summary>
    ///
    /// <returns>
    /// true if this heap is empty
    /// </returns>
	bool IsEmpty() {
		if (this->m_vec.empty()) return true;	//empty  == true
		else return false;	//else false
	}
    /// <summary>
    /// change the key of the node which the value is the target.<para/>
    /// In general, the newKey should be smaller than the old key.<para/>
    /// </summary>
    ///
    /// <parma name="target">
    /// the target to change the key
    /// </param>
    ///
    /// <param name="newKey">
    /// new key for the target
    /// </param>
	void DecKey(TValue target, TKey newKey) {	//update target, key
		if (IsEmpty()) {
			Push(newKey, target);
			return;
		}
		std::pair<TKey, TValue> p = Get(target);	//get target node
		if (p.first == IN_FINITY && p.second == IN_FINITY) Push(newKey, target);	//if infinite value, push new node
		else {
			std::vector<pair<TKey, TValue>> v;
			int iter = 0;
			while (this->m_vec[iter] != p) iter++;
			p.first = newKey;
			v.push_back(make_pair(p.first, p.second));
			this->m_vec[iter] = v[0];
		}
		return;
	}

private:
    /// <summary>
    /// heap-sort, heapify.<para/>
    /// this function can be called recursively
    /// </summary>
	void Heapify(int index) {
		int left = (index + 1) * 2 - 1;
		int right = (index + 1) * 2;

		std::pair <TKey, TValue> p;
		int temp;

		if (this->m_vec.size() <= left) return;		//if no child node
		else if (this->m_vec.size() <= right) {		//if only left child
			std::pair<TKey, TValue> pLeft = this->m_vec[left];
			p = pLeft;
			temp = left;
		}
		else {
			std::pair<TKey, TValue> pLeft = this->m_vec[left];	//both child exist
			std::pair<TKey, TValue> pRight = this->m_vec[right];
			if (pLeft.first <= pRight.first) {
				temp = left;
				p = pLeft;
			}
			else {
				temp = right;
				p = pRight;
			}
		}

		if (this->m_vec[index] > this->m_vec[temp]) {		//compare
			std::vector<pair<TKey, TValue> > v;
			v.push_back(make_pair(p.first, p.second));
			this->m_vec[temp] = this->m_vec[index];
			this->m_vec[index] = v[0];
			index = 0;
		}
		else index = temp;
		Heapify(index);			//recursively
	}
};

#endif
