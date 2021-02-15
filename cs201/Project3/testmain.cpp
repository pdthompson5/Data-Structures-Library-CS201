#include <iostream>
using namespace std;
#include "Heap.cpp"
#include "BHeap.cpp"



/*Tests a simple merge of two degree zero trees 
Expected output: 
5
6
5, 6
*/
int test1(){
    BinomialTree<int> a(5);
    BinomialTree<int> b(6);
    a.printTree(a.getHead());
    cout << endl;
    a.printTree(b.getHead());
    cout << endl;
    
    a.merge(&b);
    
    a.printTree(a.getHead());
    cout << endl;
    
}

/*Tests a merge of two degree 1 trees
Expected output:
5, 7, 8, 6
*/
int test2(){
    BinomialTree<int> a(5);
    BinomialTree<int> b(6);
    a.merge(&b);
    BinomialTree<int> c(7);
    BinomialTree<int> d(8);
    c.merge(&d);
    a.merge(&c);
    a.printTree(a.getHead());
    cout << endl;
}

int test3(){
    BinomialTree<int> a(5);
    BinomialTree<int> b(6);
    a.merge(&b);
    BinomialTree<int> c(3);
    BinomialTree<int> d(4);
    c.merge(&d);
    a.merge(&c);
    BinomialTree<int> e(7);
    BinomialTree<int> f(8);
    e.merge(&f);
    BinomialTree<int> g(1);
    BinomialTree<int> h(2);
    g.merge(&h);
    e.merge(&g);
    a.merge(&e);
    a.printTree(a.getHead());
    cout << endl;
    
}

int main(){
    test3();


}