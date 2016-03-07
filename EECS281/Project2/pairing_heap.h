#ifndef PAIRING_HEAP_H
#define PAIRING_HEAP_H

#include "eecs281priority_queue.h"


//A specialized version of the 'heap' ADT implemented as a pairing heap.
template<typename TYPE, typename COMP = std::less<TYPE>>
class pairing_heap : public eecs281priority_queue<TYPE, COMP> {
public:
  typedef unsigned size_type;

  //Description: Construct a heap out of an iterator range with an optional
  //             comparison functor.
  //Runtime: O(n) where n is number of elements in range.
  template<typename InputIterator>
  pairing_heap(InputIterator start, InputIterator end, COMP comp = COMP());

  //Description: Construct an empty heap with an optional comparison functor.
  //Runtime: O(1)
  pairing_heap(COMP comp = COMP());
  
  //Description: Copy constructor.
  //Runtime: O(n)
  pairing_heap(const pairing_heap& other);
  
  //Description: Copy assignment operator.
  //Runtime: O(n)
  pairing_heap& operator=(const pairing_heap& rhs);
  
  //Description: Destructor
  //Runtime: O(n)
  ~pairing_heap();

  //Description: Assumes that all elements inside the heap are out of order and
  //             'rebuilds' the heap by fixing the heap invariant.
  //Runtime: O(n)
  virtual void make_heap();

  //Description: Add a new element to the heap. This has been provided for you,
  //             in that you should implement push functionality in the
  //             add_node function.
  //Runtime: Amortized O(1)
  virtual void push(const TYPE& val) { add_node(val); }

  //Description: Remove the most extreme (defined by 'compare') element from
  //             the heap.
  //Note: We will not run tests on your code that would require it to pop an
  //element when the heap is empty. Though you are welcome to if you are
  //familiar with them, you do not need to use exceptions in this project.
  //Runtime: Amortized O(log(n))
  virtual void pop();

  //Description: Return the most extreme (defined by 'compare') element of
  //             the heap.
  //Runtime: O(1)
  virtual const TYPE& top() const;

  //Description: Get the number of elements in the heap.
  //Runtime: O(1)
  virtual size_type size() const { return num;/*** Fill this in ***/ }

  //Description: Return true if the heap is empty.
  //Runtime: O(1)
  virtual bool empty() const { return root == NULL;/*** Fill this in ***/ }

  class Node {
    //This node class will allow external users to update the priority of
    //elements that are already inside the heap.
  public:
      
      Node(TYPE e)
      {
          elt = e;
          child = NULL;
          prev = NULL;
          next = NULL;
      }
    //***Add any constructors you need here.
  public:
    //Description: Allows access to the element at that Node's position.
    //Runtime: O(1) - this has been provided for you.
    const TYPE& operator*() const { return elt; }

    //The following line allows you to access any private data members of this
    //Node class from within the pairing_heap class. (ie: myNode.elt is a legal
    //statement in pairing_heap's add_node() function).
    friend pairing_heap;
  private:
    TYPE elt;

  private:
    //***Add any additional member functions or data you require here.
      Node *child;
      Node *prev;
      Node *next;
  };

  //Description: Updates the priority of an element already in the heap by
  //             replacing the element refered to by the Node with new_value.
  //             Must maintain heap invariants.
  //
  //PRECONDITION: The new priority, given by 'new_value' must be more extreme
  //              (as defined by comp) than the old priority.
  //
  //Runtime: As discussed in reading material.
  void updateElt(Node* node, TYPE new_value);

  //Description: Add a new element to the heap. Returns a Node* corresponding
  //             to the newly added element.
  //Runtime: Amortized O(1)
  Node* add_node(const TYPE& val);

private:
  //***Add any additional member functions or data you require here.
  //***We recommend creating a 'meld' function (see the reading).
   Node *root;
   size_type num;
   std::vector <Node*> vec;
   void meld(Node *&lnode, Node *&rnode)
   {
       if(rnode == NULL)
           return;
       if(this->compare(**lnode, **rnode))
       {
           rnode->prev = lnode->prev;
           lnode->prev = rnode;
           lnode->next = rnode->child;
           if(lnode->next != NULL)
               lnode->next->prev = lnode;
           rnode->child = lnode;
           lnode = rnode;
       }
       else
       {
           meld(rnode, lnode);
           /*rnode->prev = lnode;
           lnode->next = rnode->next;
           if(lnode->next != NULL)
               lnode->next->prev =lnode;
           rnode->next = lnode->child;
           if(rnode->next != NULL)
               rnode->next->prev = rnode;
           lnode->child = rnode;*/
       }
   }/*
   void delHelper(Node *d)
   {
       if(d != NULL)
       {
           delHelper(d->child);
           delHelper(d->next);
           delete d;
       }
   }*/
   void makeHelper(Node *m)
   {
       if(m != NULL)
       {
           makeHelper(m->child);
           makeHelper(m->next);
           m->child = NULL;
           m->next = NULL;
           m->prev = NULL;
           vec.push_back(m);
           
       }
   }/*
   Node* copy(Node *c)
   {
       if(c == NULL)
           return NULL;
       Node *n = new Node(c->elt);
       if((n->child = copy(c->child)) != NULL)
           n->child->prev = n;
       if((n->next = copy(c->next)) != NULL)
           n->next->prev = n;
       return n;
   }*/
};

template<typename TYPE, typename COMP>
template<typename InputIterator>
pairing_heap<TYPE, COMP>::pairing_heap(
  InputIterator start,
  InputIterator end,
  COMP comp
) {
    root = NULL;
    num = 0;
    this->compare = comp;//Your code.
    for(auto it = start; it != end; it++)
    {
        push(*it);
    }
  make_heap();
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(COMP comp) {
  this->compare = comp;
  root = NULL;
  num = 0;
  //Your code.
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::pairing_heap(const pairing_heap& other) {
   root = NULL;
    *this = other;//Your code.
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>&
pairing_heap<TYPE, COMP>::operator=(const pairing_heap& rhs) {
 //Your code.  
   // delHelper(root);
  root = NULL;
  *this = rhs;
  return *this;
}

template<typename TYPE, typename COMP>
pairing_heap<TYPE, COMP>::~pairing_heap() {
  /*delHelper(root);
  root = NULL;
  num = 0;*/

}
    
template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::make_heap() {
    if(num == 0)
      return;
    vec.clear();
    makeHelper(root);

  int numSib = (int)vec.size();
  if(numSib == 0)
      return;
            int i = 0;
            Node *min = root;
            for(; i < numSib; i ++)
            {
                if(this->compare(**min, **vec.at(i)))
                    min = vec.at(i);
            }
            i = 0;
            for(; i < numSib - 1; i +=2)
            {
                meld(vec.at(i),vec.at(i+1));

               // std::cout << i+1 << "\n";
            }
            int j = i-2;
            
            if(j == numSib -3 && j >= 0)
            {
                meld(vec.at(j),vec.at(j+2));
            }
            for(; j >= 2; j -= 2)
                meld(vec.at(j-2), vec.at(j));
            root = min;
    vec.clear();//Your code.
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::pop() {
   Node *popee = root;
    if(root->child == NULL)
      root = NULL;
    else
    {
        Node *first = root->child;
        if(first->next == NULL)
        {
            root = first;
        }
        else
        {
            std::vector<Node*> nodes;

            while(first != NULL) 
            {
                nodes.push_back(first);
                first->prev->next = NULL;
                first = first->next;
            }
            int numSib = (int)nodes.size();
            int i = 0;
            for(; i < numSib - 1; i +=2)
            {
                meld(nodes.at(i),nodes.at(i+1));

            }
            int j = i-2;
            if(j == numSib -3)
                meld(nodes.at(j),nodes.at(j+2));
            for(; j >= 2; j -= 2)
                meld(nodes.at(j-2), nodes.at(j));
            root = nodes.front();
        }
    }
  num--;
  delete popee;
  //Your code.
}

template<typename TYPE, typename COMP>
const TYPE& pairing_heap<TYPE, COMP>::top() const {
  return **root; //This line present only so that this provided file compiles.
}

template<typename TYPE, typename COMP>
typename pairing_heap<TYPE, COMP>::Node*
pairing_heap<TYPE, COMP>::add_node(const TYPE& val) {
  Node *n = new Node(val);
  if(root == NULL)
      root = n;
  else
      meld(root, n);
  num++;
  return n;//Your code.
}

template<typename TYPE, typename COMP>
void pairing_heap<TYPE, COMP>::updateElt(Node* node, TYPE new_value) {
  
    node->elt = new_value;
    if(node != root)
    {
        if(node->next != NULL)
        {
            node->next->prev = node->prev;
        }
        if(node->prev->child == node)
        {
            node->prev->child = node->next;
        }
        else
        {
            node->prev->next = node->next;
        }
        node->next = NULL;
        meld(root, node);
    }
    //Your code.
}

#endif //PAIRING_HEAP_H

