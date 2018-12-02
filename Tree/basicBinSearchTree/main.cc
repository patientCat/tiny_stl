//c++ program
//design by >^.^<
//@date    2018-11-20 11:03:38
#include "BinSearchTree.hh" 
#include <iostream>

#define TEST(msg, id) \
do{ \
    fprintf(stdout, "test_id: %d, %s\n", ++id, msg); \
}while(0); \

int ID;

void test1()
{
    BinarySearchTree<int> bst;
    TEST("insert test", ID);

    bst.insert(12);
    bst.insert(33);
    bst.insert(24);
    bst.insert(98);
    bst.printTree(cout);
    
    TEST("remove test", ID);
    bst.remove(12);
    bst.remove(0);
    bst.remove(33);
    bst.printTree(cout);
}

int main()
{
    test1();
    return 0;
}

