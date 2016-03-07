#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "eecs281priority_queue.h"


//A specialized version of the 'heap' ADT implemented as a binary heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class binary_heap : public eecs281priority_queue<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  binary_heap(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  binary_heap(COMP comp = COMP());

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n)
  virtual void make_heap();

  //Description: Add a new element to the heap.
  //Runtime: O(log(n))
  virtual void push(const TYPE& val);

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap.
  //Runtime: O(1)
  virtual size_type size() const
    { return data.size();/*** Fill this in - might be very simple depending on implementation ***/ }

  //Description: Return true if the heap is empty.
  //Runtime: O(1)
  virtual bool empty() const
    { return data.empty();/*** Fill this in - might be very simple depending on implementation ***/ }
private:
  //Note: This vector *must* be used your heap implementation.
  std::vector<TYPE> data;
private:
    
  //***Add any additional member functions or data you require here.
   /* int left(int parent)
    {
        int l = 2 * parent + 1;
        if (l < (int)data.size())
            return l;
        else 
            return -1;
    }
    int right(int parent)
    {
        int r = 2 * parent + 2;
        if (r < (int)data.size())
            return r;
        else
            return -1;
    }
    int parent(int child)
    {
        int p = (child - 1) / 2; 
        if (child == 0)
            return -1;
        else return p;
    }
    void fixUp(int index)
    {
        if(index >= 0 && parent(index) >= 0  && this->compare(data.at(parent(index)), data.at(index)))
        {
            TYPE temp = data.at(index);
            data.at(index) = data.at(parent(index));
            data.at(parent(index)) = temp; 
            fixUp(parent(index));
        } 
    }
    void fixDown(int index)
    {
        int lchild = left(index);
        int rchild = right(index);
        if(lchild >= 0 && rchild >= 0 && this->compare(data.at(lchild), data.at(rchild)))
        {
            lchild = rchild;
        }
        if(lchild > 0)
        {
            TYPE temp = data.at(index);
            data.at(index) = data.at(lchild);
            data.at(lchild) = temp;
            fixDown(lchild);
        }
    }*/
   
    void fixDown(int k)
    {
        while(2 * k+1 <= (int)data.size()-1)
        {
            int j = 2 * k+1;
            if(j < (int)data.size()-1 && this->compare(data.at(j), data.at(j+1)))
                ++j;
            if(this->compare(data.at(j), data.at(k)))
                break;
            TYPE temp = data.at(k);
            data.at(k) = data.at(j);
            data.at(j) = temp;
            k = j;
        }
    }   
    void fixUp(int k)
    {
        while(k > 0 && this->compare(data.at((k-1)/2), data.at(k)))
        {
            TYPE temp = data.at(k);
            data.at(k) = data.at((k-1)/2);
            data.at((k-1)/2) = temp;
            k = (k-1)/2;
        }
    }
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
binary_heap<TYPE, COMP>::binary_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
  //Your code.
    auto it = data.begin();
    data.insert(it, start, end); 
    this->compare = comp;
    make_heap();
}

template<typename TYPE, typename COMP>
binary_heap<TYPE, COMP>::binary_heap(COMP comp) {
  //Your code.
    this->compare = comp;
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::make_heap() {
  //Your code.
    
    int last = (int)data.size() - 1;
    while(last >= 0 )
    {
        fixDown(last);
        last--;
    }
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::push(const TYPE& val) {
  //Your code.
    data.push_back(val);
    fixUp(data.size()-1);
}

template<typename TYPE, typename COMP>
void binary_heap<TYPE, COMP>::pop() {
    data.at(0)= data.at(data.size()-1);
    data.pop_back();
    fixDown(0);
  //Your code.
}

template<typename TYPE, typename COMP>
const TYPE& binary_heap<TYPE, COMP>::top() const {
  //Your code.
    return data.at(0);
    
    //This line present only so that this provided file compiles.
}

#endif //BINARY_HEAP_H

