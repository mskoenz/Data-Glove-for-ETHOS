// Author:  Mario S. Könz <mskoenz@gmx.net>
// Date:    07.11.2012 10:29:54 CET
// File:    test_tree.cpp

#include <iostream>
#define GCC
#include "range_tree.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    range_tree_class<double> tree;
    tree.insert(1,2);
    detail::intervall_class<double>::incr_k();
    tree.insert(2,3);
    detail::intervall_class<double>::incr_k();
    tree.insert(4,5);
    detail::intervall_class<double>::incr_k();
    tree.insert(3,4);
    detail::intervall_class<double>::incr_k();
    tree.insert(5,6);
    detail::intervall_class<double>::incr_k();
    tree.insert(6,7);
    detail::intervall_class<double>::incr_k();
    tree.insert(1,7);
    
    tree.build_tree();
    tree.print();
    
    std::cout << tree.query(3.4) << std::endl;
    
    vector_tree_class<int, 3> vTree;
    
    int m1[] = {1,2,2};
    int m2[] = {3,2,4};
    int n1[] = {2,1,1};
    int n2[] = {4,5,3};
    
    vTree.insert(m1, m2);
    vTree.insert(n1, n2);
    
    vTree.build_tree();
    
    int q[] = {2,2,2};
    std::cout << "vector_tree" << std::endl;
    std::cout << vTree.query(q) << std::endl;
    std::cout << vTree.query(q) << std::endl;
    std::cout << vTree.query(q) << std::endl;
    
    vTree.print();
    return 0;
}
