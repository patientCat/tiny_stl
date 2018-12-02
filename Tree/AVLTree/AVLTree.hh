#ifndef _AVLTREE_HH_
#define _AVLTREE_HH_

#include <iostream> 

using namespace std;

template <typename Comparable>
class AVLTree 
{ 
public: 
    AVLTree ()
    : _root{ nullptr }
    {}

    AVLTree ( const AVLTree & rhs)
    {
        _root = clone(rhs._root);
    }

    AVLTree ( AVLTree && rhs)
    {
        _root = clone(std::move(rhs._root)); 
    }

    ~AVLTree ()
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

    AVLTree & operator=(const AVLTree & rhs);
    AVLTree & operator=(AVLTree && rhs);

private: 

    struct AVLNode
    {
        Comparable element;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode( const Comparable & rhElement, AVLNode *lt = nullptr, AVLNode *rt = nullptr, int h = 0)
        : element{ rhElement }
        , left{ lt }
        , right{ rt }
        , height{ h }
        {}

        AVLNode( Comparable && rhElement, AVLNode *lt = nullptr, AVLNode *rt = nullptr, int h = 0)
        : element{ std::move(rhElement) }
        , left{ lt }
        , right{ rt }
        , height{ h }
        {}
    };

    /* 
     * 这里有一些传引用调用使用传递指针变量的技巧，这使得public型成员函数能够把指向根节点的指针
     * 传递给private函数进行递归调用，借此可以改变根的值
     */
    AVLNode * _root;
    void insert( const Comparable &x, AVLNode * & t)
    {
        if( nullptr == t)
            t = new AVLNode{x};
        else if( x < t->element )
            insert(x, t->left);
        else if( x > t->element )
            insert(x, t->right);
        else
            ; // 重复元，这里可以声明都不做，也可以，如果在节点中添加辅助数据结构这里可以添加重复元

        balance(T);
    }

    void insert( Comparable &&x, AVLNode * &t)
    {
        if( nullptr == t)
            t = new AVLNode{x};
        else if( x < t->element )
            insert(std::move(x), t->left);
        else if( x > t->element )
            insert(std::move(x), t->right);
        else
            ; // 重复元，这里可以声明都不做，也可以，如果在节点中添加辅助数据结构这里可以添加重复元

        balance(t);
    }

    void remove( const Comparable &x, AVLNode * & t)
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
            AVLNode *oldNode = t;
            t = (t->left) ? t->left : t->right;
            delete oldNode;
        }

        balance(t);
    }
    
    static const int AVL_IMBALANCE = 1;
    // 使AVL树恢复平衡
    void balance(AVLNode *& t)
    {
        if(nullptr == t)
            return ;
        if(height(t->left) - height(t->right) > AVL_IMBALANCE)
        {
            if(height(t->left->left) >= height(t->left->right))
                rotateWithLeftChild(t);
            else
                doubleRotateWithLeftChild(t);
        }
        else if(height(t->right) - height(t->left) > AVL_IMBALANCE)
        {
            if(height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t);
            else
                doubleRotateWithRightChild(t);
        }
        t->height = std::max(height(t->left), height(t->right)) + 1; 
    }

/*
                 .─.                            .─.                
               ╱(k2 )╲                        ╱(k1 )╲              
              ╱  `─'  ╲                    ╱─╱  `─'  ╲             
            ╱╱         ╲╲                 ▼          │             
          .▼.            ╲                ▲          │ .─.         
         ╱k1 ╲            ▼              ▲ ▲         ╱(k2 )╲       
        ╱ `─' ╲          ▲ ▲            ▲ A ▲       ╱  `─'  ╲      
      ╱╱       ╲        ▲ D ▲          ▲     ▲     ╱         ╲     
     ▼          ▼      ▲     ▲        ▲▲▲▲▲▲▲▲▲   ▼           ▼    
    ▲           ▲     ▲▲▲▲▲▲▲▲▲                   ▲           ▲    
   ▲ ▲         ▲ ▲                               ▲ ▲         ▲ ▲   
  ▲ A ▲       ▲ A ▲                             ▲ A ▲       ▲ D ▲  
 ▲     ▲     ▲     ▲                           ▲     ▲     ▲     ▲ 
▲▲▲▲▲▲▲▲▲   ▲▲▲▲▲▲▲▲▲                         ▲▲▲▲▲▲▲▲▲   ▲▲▲▲▲▲▲▲▲
*/
    void rotateWithLeftChild(AVLNode *& k2)
    {
        AVLNode * k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->left), height(k1->right)) + 1; 
        k2 = k1;
    }

    void rotateWithRightChild(AVLNode *& k2)
    {
        AVLNode * k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;
        k2->height = std::max(height(k2->left), height(k2->right)) + 1;
        k1->height = std::max(height(k1->left), height(k1->right)) + 1; 
        k2 = k1;
    }

    void doubleRotateWithLeftChild(AVLNode *& k3)
    {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }
/*
          .─.                                           .─.                        
        ╱( g )╲                                       ╱( c )╲                      
       ╱  `─'  ╲                                     ╱  `─'  ╲                     
      ╱         ╲╲                                  ╱         ╲                    
     ▼            ▼.─.                            ╱╱           ╲─╲                 
    ▲            ╱( p )╲                         ╱                ╲                
   ▲ ▲          ╱  `─'  ╲                       ╱                  ╲               
  ▲ A ▲        ╱         ╲╲                    ▼                    ▼              
 ▲     ▲       ╱           ▼                  .─.                  .─.             
▲▲▲▲▲▲▲▲▲     ▼            ▲                ╱( g ) ╲             ╱( p )╲           
             .─.          ▲ ▲              ╱  `─'   ╲           ╱  `─'  ╲          
           ╱( c )╲       ▲ D ▲            ╱         │           │        ╲         
          ╱  `─'  ╲     ▲     ▲          ╱          │           │         ╲─╲      
        ╱╱         ╲╲  ▲▲▲▲▲▲▲▲▲        ╱           │           │            ╲     
       ▼             ▼                 ▼            ▼           ▼             ▼    
       ▲             ▲                 ▲            ▲           ▲             ▲    
      ▲ ▲           ▲ ▲               ▲ ▲          ▲ ▲         ▲ ▲           ▲ ▲   
     ▲ B ▲         ▲ C ▲             ▲ A ▲        ▲ B ▲       ▲ C ▲         ▲ D ▲  
    ▲     ▲       ▲     ▲           ▲     ▲      ▲     ▲     ▲     ▲       ▲     ▲ 
   ▲▲▲▲▲▲▲▲▲     ▲▲▲▲▲▲▲▲▲         ▲▲▲▲▲▲▲▲▲    ▲▲▲▲▲▲▲▲▲   ▲▲▲▲▲▲▲▲▲     ▲▲▲▲▲▲▲▲▲
*/
    void doubleRotateWithRightChild(AVLNode *&g)
    {
        AVLNode *A, *B, *C, *D;
        AVLNode *p = g->right;
        AVLNode *c = p->left;
        A = g->left, B = c->left, C = c->right, D = p->right;
        g->left = A;
        g->right = B;
        g->height = std::max(height(g->left), height(g->right)) + 1; 
        p->left = C;
        p->right = D;
        p->height = std::max(height(p->left), height(p->right)) + 1; 
        c->left = g;
        c->right = p;
        c->height = std::max(height(c->left), height(c->right)) + 1; 
        g = c;
    }

    // 找到树中最小元素
    AVLNode * findMin(AVLNode *t) const
    {
        if(!t) return nullptr;
        if(!t->left) return t;
        return findMin(t->left);
    }
    AVLNode * findMax(AVLNode *t) const
    {
        if(!t) return nullptr;
        while(t->right)
            t = t->right;
        return t;
    }

    // 测试x是否在子树t上
    bool contains(const Comparable &x, AVLNode *t) const
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
    void makeEmpty(AVLNode *&t)
    {
        if(t)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

    void printTree(AVLNode *t, ostream &out) const
    {
        if(!t)
            return ;
        printTree(t->left, out);
        out << t->element << endl;
        printTree(t->right, out);
    }

    AVLNode * clone(AVLNode *t) const
    {
        if( nullptr == t)
            return nullptr;
        else
            return new AVLNode{t->element, clone(t->left), clone(t->right)};
    }

    static int avlHeight(AVLNode *t)
    {
        return t == nullptr ? -1 : t->height;
    } 
}; 


#endif // AVLTree.hh
