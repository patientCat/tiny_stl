#ifndef _LIST_HH_
#define _LIST_HH_

#include <algorithm> 

template <typename T>
class List 
{ 
private:
    struct Node{
        T data;
        Node *prev;
        Node *next;

        Node(const T &dd, Node * pp = nullptr, Node *nn = nullptr)
        : data{ dd }
        , prev{ pp }
        , next{ nn }
        {}
    };

public:
    class const_iterator{
    public:
        const_iterator()
        : current{ nullptr }
        {}

        const T & operator*() const
        {   return retrieve();  }

        const_iterator operator++()
        {
            current = current->next;
            return *this;
        }

        // 使用单参数int指定后缀形式
        const_iterator operator++(int)  
        {
            const_iterator old = *this;
            ++( *this );
            return *this;
        }

        bool operator==(const const_iterator &rhs) const
        {   return current == rhs.current;  }

        bool operator!=(const const_iterator &rhs) const
        {   return !( *this == rhs );   }

    protected:
        Node *current; 
        T & retrieve() const
        {   return current->data;   }

        const_iterator( Node *p )  // 设置为protected是为了让iterator可以看到，而不被其他类看到
        : current{ p }
        {}

        friend class List<T>; // 为了让List中可以使用const_iterator的构造函数
    };

    class iterator
    : public const_iterator // 继承，说明iterator和const_iterator具有相同的功能，凡是能用到const_iterator的
      // 地方都可以使用iterator，反之则不然
    {
    public:
        iterator()
        {}

        T & operator*()
        {   return const_iterator::retrieve();  }
        const T& operator*()const // 如果不显示地写出来，会被上面这个函数覆盖
        {   return const_iterator::operator*(); }  

        iterator & operator++() // 前缀
        {
            this->current = this->current->next;
            return *this;
        }

        iterator & operator++(int) // 后缀
        {
            iterator old = *this;
            ++(*this);
            return old;
        }

        protected:
            iterator (Node * p)
            : const_iterator{ p }
            {}

            friend class List<T>;
    };

public: 
    List () {   init();    }; 
    ~List ()
    {
        clear();
        delete _head;
        delete _tail;
    }
    List (const List &rhs)
    {
        init();
        for(auto & x : rhs)
        {
            _head->push_back(x);
        }
    }

    List & operator=(const List &rhs)
    {
        List copy = rhs; 
        std::swap(*this, copy); // 非常绝妙的写法
        return *this;
    }

    List (List &&rhs)
    : _size{ rhs._size }
    , _head{ rhs._head }
    , _tail{ rhs._tail }
    {
        rhs._size = 0;
        rhs._head = nullptr;
        rhs._tail = nullptr;
    }

    List & operator=(List && rhs)
    {
        std::swap(_size, rhs._size);
        std::swap(_head, rhs._head);
        std::swap(_tail, rhs._tail);
        return *this;
    }

    // 迭代器的构造只有构造函数和无参构造函数
    iterator begin()    {   return {_head->next}; } // 非常精妙的设计
    iterator end()    {   return _tail; } 
    const_iterator begin() const    {   return _head->next; }
    const_iterator end() const   {   return _tail; }
     
    int size() const {  return _size;   }
    bool empty() const {    return !_size;  }

    void clear()
    {
        while(!empty())
            pop_front();
    }

    void push_front(const T &x)
    {   insert( begin(), x ); }
    void push_back(const T &x)
    {   insert(end(), x); }

    void pop_front(T &&x)
    {   insert( begin(), std::move(x) );   }

    void pop_back(T &&x)
    {   isnert( end(), std::move(x) );    }

    iterator insert(iterator iter, const T &x)
    {
        Node * p = iter.current;
        ++_size;
        return { p->prev->next = p->prev = new Node{x, p->prev, p} };
    }

    iterator insert(iterator iter, T &&x)
    {
        Node * p = iter.current;
        ++_size;
        return { p->prev->next = p->prev = new Node{std::move(x), p->prev, p} };
    }

    iterator erase(iterator iter)
    {
        Node *p = iter.current;
        iterator ret{ p->next };
        p->prev->next = p->next;
        p->next->prev = p->prev;
        delete p;
        --_size;
        return ret;
    }

    iterator erase(iterator from, iterator to)
    {
        for(auto iter = from; iter != to; ) // 这里不能使用++iter，因为iter在被erase后就会失效
            iter = erase(iter);
        return to;
    }

private:
    void init()
    {
        _size = 0;
        _head = new Node{T{}};
        _tail = new Node{T{}};
        _head->next = _tail;
        _tail->prev = _head;
    }
private: 
    Node *_head;
    Node *_tail;
    int _size;
}; 

#endif // List.hh
