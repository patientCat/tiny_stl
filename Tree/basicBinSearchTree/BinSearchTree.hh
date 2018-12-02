#ifndef _BINSEARCHTRR_HH_
#define _BINSEARCHTRR_HH_

#include <iostream> 

using namespace std;

template <typename Comparable>
class BinarySearchTree 
{ 
public: 
    BinarySearchTree ()
    : _root{ nullptr }
    {}

    BinarySearchTree ( const BinarySearchTree & rhs)
    {
        _root = clone(rhs._root);
    }

    BinarySearchTree ( BinarySearchTree && rhs)
    {
        _root = clone(std::move(rhs._root)); 
    }

    ~BinarySearchTree ()
    {
        makeEmpty();
    }

    const Comparable & findMin() const;
    const Comparable & findMax() const;

    // 如果在树中找到x，则返回true
    bool contains(const Comparable &x) const 
    {
        return contains(x, _root);
    }

    bool isEmpty() const { return !_root;   }
    void printTree( ostream & out = cout) const
    {
        printTree(_root, out);
    }
     
    void makeEmpty()
    {
        makeEmpty(_root);
    }

    // 将x插入到树中，忽略重复元
    void insert(const Comparable & x)
    {
        insert(x, _root);
    }

    void insert(Comparable && x)
    {
        insert(std::move(x), _root);
    }

    // 将x从树中删除，如果没找到x，则什么也不做
    void remove(const Comparable &x)
    {
        remove(x, _root);
    }

    BinarySearchTree & operator=(const BinarySearchTree & rhs);
    BinarySearchTree & operator=(BinarySearchTree && rhs);

private: 

    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode( const Comparable & rhElement, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr)
        : element{ rhElement }
        , left{ lt }
        , right{ rt }
        {}

        BinaryNode( Comparable && rhElement, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr)
        : element{ std::move(rhElement) }
        , left{ lt }
        , right{ rt }
        {}
    };

    /* 
     * 这里有一些传引用调用使用传递指针变量的技巧，这使得public型成员函数能够把指向根节点的指针
     * 传递给private函数进行递归调用，借此可以改变根的值
     */
    BinaryNode * _root;
    void insert( const Comparable &x, BinaryNode * & t)
    {
        if( nullptr == t)
            t = new BinaryNode{x};
        else if( x < t->element )
            insert(x, t->left);
        else if( x > t->element )
            insert(x, t->right);
        else
            ; // 重复元，这里可以声明都不做，也可以，如果在节点中添加辅助数据结构这里可以添加重复元
    }

    void insert( Comparable &&x, BinaryNode * &t)
    {
        if( nullptr == t)
            t = new BinaryNode{x};
        else if( x < t->element )
            insert(std::move(x), t->left);
        else if( x > t->element )
            insert(std::move(x), t->right);
        else
            ; // 重复元，这里可以声明都不做，也可以，如果在节点中添加辅助数据结构这里可以添加重复元
    }

    void remove( const Comparable &x, BinaryNode * & t)
    {
        if(t == nullptr)
            return; // 项没找到，什么都不做
        if(x < t->element )
            remove( x, t->left );
        else if( t->element < x )
            remove( x, t->right );
        else if( t->left && t->right)
        {
            t->element = findMin(t->right)->element;
            remove(t->element, t->right);
        }
        else { // 有单个孩子的版本
            BinaryNode *oldNode = t;
            t = (t->left) ? t->left : t->right;
            delete oldNode;
        }
    }

    // 找到树中最小元素
    BinaryNode * findMin(BinaryNode *t) const
    {
        if(!t) return nullptr;
        if(!t->left) return t;
        return findMin(t->left);
    }
    BinaryNode * findMax(BinaryNode *t) const
    {
        if(!t) return nullptr;
        while(t->right)
            t = t->right;
        return t;
    }

    // 测试x是否在子树t上
    bool contains(const Comparable &x, BinaryNode *t) const
    {
        if(!t)  return 
            false;
        else if(x < t->element)
            return contains(x, t->left);
        else if(t->element < x)
            return contains(x, t->right);
        else
            return true;
    }
    void makeEmpty(BinaryNode *&t)
    {
        if(t)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

    void printTree(BinaryNode *t, ostream &out) const
    {
        if(!t)
            return ;
        printTree(t->left, out);
        out << t->element << endl;
        printTree(t->right, out);
    }

    BinaryNode * clone(BinaryNode *t) const
    {
        if( nullptr == t)
            return nullptr;
        else
            return new BinaryNode{t->element, clone(t->left), clone(t->right)};
    }
}; 


#endif // BinSearchTree.hh
