//c++ program
//design by >^.^<
//@date    2018-11-19 11:02:56
#include <iostream>
#include <string> 
#include <vector> 
#include <cctype> 
#include <stack> 

using namespace std;
// 构造一颗表达式树
class BNode 
{ 
public: 
    BNode (char vv, BNode *l = nullptr, BNode *r = nullptr)
    : val{ vv }
    , left{ l }
    , right{ r}
    {}
    ~BNode (); 
     
    char val;
    BNode *left;
    BNode *right;
}; 

class ExpressionTree 
{ 
public: 
    bool isSymbol(char c)
    {
        return !isOp(c);
    }

    bool isOp(char c)
    {
        return isalpha(c);
    }

    void buildTree(const string & expr)
    {

        for(int i = 0; (size_t)i != expr.size(); ++i)
        {
            if(isOp(expr[i]))
            {
                BNode *node = new BNode{expr[i]};
                _stack.push(node);
            }
            else{
                BNode *t1 = _stack.top(); _stack.pop();
                BNode *t2 = _stack.top(); _stack.pop();
                BNode *node = new BNode{expr[i], t2, t1};
                _root = node;
                _stack.push(node);
            }
        }

    }

    void postTraverse()
    {
        postTraverseHelper(_root);
    }
    void postTraverseHelper(BNode * p)
    {
        if(!p)
            return ;
        postTraverseHelper(p->left);
        postTraverseHelper(p->right);
        cout << p->val ;
    }
     
private: 
    stack <BNode *> _stack;
    BNode * _root;
};  

int main()
{
    string arr {"ab+cde+**"};
    ExpressionTree etree{};
    etree.buildTree(arr);
    etree.postTraverse();
}
