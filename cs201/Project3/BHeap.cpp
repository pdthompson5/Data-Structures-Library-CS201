#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;


template <typename keytype>
struct Node{
        keytype key;
        Node* next;
        Node* prev;
        Node* leftmostChild;
        Node* parent;
};

//Children are stored in a doubly linked list
template <typename keytype>
class BinomialTree{
    private:
        int degree;
        Node<keytype>* head;
        //next and prev are not used within this class, except instantiating to null
        BinomialTree* next;
        BinomialTree* prev;
    public:
        /*Default constructor 
            -sets degree to 0
            -Sets all pointers to null
        Runtime: 0(1)
        */
        BinomialTree(){
            degree = 0;
            head = nullptr;
            next = nullptr;
            prev = nullptr;
        }

        /*Constuctor 
            -Sets the head to the given head
            -Sets the degree to the given degree
            -Does not manually alloate memory 
            -Runtime: O(1)
        */
        BinomialTree(Node<keytype>* head, int degree){
            this->head = head;
            this->degree = degree;
            next = nullptr;
            prev = nullptr;
        }

        /*Constructor 
            -Creates a tree of degree 0 with a head of keytype k
            -Manually allocates the head 
            -Runtime: O(1)
        */
        BinomialTree(keytype k){
            this->head = new Node<keytype>;
            this->next = nullptr;
            this->prev = nullptr;

            this->head->key = k;
            this->degree = 0;
            this->head->leftmostChild = nullptr;
            this->head->next = nullptr;
            this->head->prev = nullptr;
            this->head->parent = nullptr;
        }
        
        /*Idea: free the head, create new trees with its children of degree - 1 and free those until degree = 0
        */
        ~BinomialTree(){
            
            //tree has been merged in the past and is no longer accesible
            if(degree == -1){
                return;
            }
            //base case: Degree == 0
            if(degree == 0){
                delete head;
                return;
            }

            Node<keytype>* currentChild = head->leftmostChild;
            BinomialTree<keytype>* currentTree;

            //For every child of the head 
            while(currentChild->next != nullptr){
                //this constuctor doesn't manually allocated memory so it is not wasteful to use it 
                currentTree = new BinomialTree(currentChild, this->degree - 1);
                currentChild = currentChild->next;
                delete currentTree;
            }

            delete head;
        }


        int getDegree(){
            return degree;
        }

        Node<keytype>* getHead(){
            return head;
        }

        BinomialTree* getNext(){
            return next;
        }

        BinomialTree* getPrev(){
            return prev;
        }

        void setDegree(int degree){
            this->degree = degree;
        }

        void setHead(Node<keytype>* head){
            this->head = head;
        }

        void setNext(BinomialTree* next){
            this->next = next;
        }

        void setPrev(BinomialTree* prev){
            this->prev = prev;
        }

        /*Merge
            -Combines two binomial trees of the same degree 
            -The head of the tree is the head with the smaller key
            -Increments the degree of the tree 
            -Runtime: O(1)
        */
        void merge(BinomialTree* a){
            
            //case: a's key is smaller 
            if(this->head->key > a->getHead()->key){
                
                
                Node<keytype>* oldLeft = a->getHead()->leftmostChild;
                
                if(oldLeft != nullptr){
                    //Add head to children list
                    oldLeft->prev = this->head;
                    this->head->next = oldLeft;
                }
  
                //make head leftmostChild
                this->head->parent = a->getHead();
                a->getHead()->leftmostChild = this->head;
                
                //change head to a's head;
                head = a->getHead();

                
            }
            //case: this->head is smaller or equal
            else{
                Node<keytype>* oldLeft = this->head->leftmostChild;       

                //add a's head to children list 
                if(oldLeft != nullptr){
                    oldLeft->prev = a->getHead();
                    a->getHead()->next = oldLeft;
                }
                
                //make a's head leftmostChild
                a->getHead()->parent = this->head;
                this->head->leftmostChild = a->getHead();
            }
            //shows that tree a is no longer accesible 
            a->degree = -1;

            //Note: none of a's pointers need to be deallocated becuase they are all still in use 
            degree++;
        }

        /*PrintTree
            -Prints the content of the tree 
            -Recursive algorithm
            -prints keys comma seperated, goes from top to bottom left then to bottom right 
        */
        void printTree(Node<keytype>* current){
            if((current->parent == nullptr && current->leftmostChild == nullptr)
                || (current->next == nullptr && current->parent == head)){
                cout << current->key;
                return;
            }
            cout << current->key << ", ";
            if(current->leftmostChild == nullptr){
                return;
            }
            current = current->leftmostChild;
            while(current != nullptr){
                printTree(current);
                current = current->next;
            }
        }


};





template <typename keytype>
class BHeap{
    private:
        BinomialTree<keytype>* head;
        int size;

        int maxDeg(){
            if(size == 0){
                return 1;
            }
            return log2(size) + 1;
        }

        void fixHeap(){
            
            BinomialTree<keytype>* degrees[maxDeg()];
            for(int i = 0; i < maxDeg(); i++){
                degrees[i] = nullptr;
            }
            int degree;
            BinomialTree<keytype>* current = head;
            
            while(current != nullptr){
                
                degree = current->getDegree();
                
                
                //case: tree of degree has not been found yet
                if(degrees[degree] == nullptr){
                    degrees[degree] = current;
                    current = current->getNext();
                }
                //case: tree of degree has been found so merge 
                else{
                    //pluck the previously found tree from the list 
                    BinomialTree<keytype>* other = degrees[degree];
                    if(other == head){
                        head = head->getNext();
                    }

                    if(other->getPrev() != nullptr){
                        other->getPrev()->setNext(other->getNext());
                    }
                    if(other->getNext() != nullptr){
                        other->getNext()->setPrev(other->getPrev());
                    }
                    
                    //merge the trees
                    
                    current->merge(other);
                    degrees[degree] = nullptr;
                }                
            
            }
        }

    public:

        /*Default constructor 
            -creates an empty heap
        Runtime: O(1)
        */
        BHeap(){
            head = nullptr;
            size = 0;
        }


        /*Array constructor 
            -builds a heap from array k
            -uses repeated inserts
        Runtime: O(s) (amortized?)
        */
        BHeap(keytype k[], int s){
            
            head = nullptr;
            size = 0;
            for(int i = 0; i < s; i++){
                this->insert(k[i]);
            }
        }

        BHeap(BinomialTree<keytype>* head, int size){
            this->head = head;
            this->size = size;
        }

        /*Destructor 
            -deallocates all manually allocated memory 
        Runtime: O(n)
        */ 
        ~BHeap(){
            if(head == nullptr){
                return;
            }
            BinomialTree<keytype>* current = head;
            BinomialTree<keytype>* next = head->getNext();

            //for every root in the root list: delete the tree 
            while(next != nullptr){
                delete current;
                current = next;
                next = next->getNext();
            }

        }

        /*Peek operation 
            -returns min without modifying the heap
        Runtime: O(lgn)
        */
        keytype peekKey(){
            keytype min = head->getHead()->key;
            BinomialTree<keytype>* current = head->getNext();

            while(current != nullptr){
                if(current->getHead()->key < min){
                    min = current->getHead()->key;
                }
                current = current->getNext();
            }

            return min;
        }

        /*Extract min operation
            -removes the min element 
            -return the min element 
            -modifies the heap to maintain properties
        Runtime: O(lgn)
        */
        keytype extractMin(){
            keytype min = head->getHead()->key;
            BinomialTree<keytype>* minRoot = head;
            BinomialTree<keytype>* current = head->getNext();

            while(current != nullptr){
                if(current->getHead()->key < min){
                    min = current->getHead()->key;
                    minRoot = current;
                }
                current = current->getNext();
            }
            if(minRoot == head){
                head = head->getNext();
            }
            //added back - 1 in merge
            int newSize = pow(2, minRoot->getDegree());
            size -= newSize;

            if(minRoot->getDegree() == 0){
                if(minRoot->getPrev() != nullptr){
                    minRoot->getPrev()->setNext(minRoot->getNext());
                }
                if(minRoot->getNext() != nullptr){
                    minRoot->getNext()->setPrev(minRoot->getPrev());
                }
                delete minRoot;
                return min;
            }
            
            int i = minRoot->getDegree() - 1;
            Node<keytype>* currentNode = minRoot->getHead()->leftmostChild;
            BinomialTree<keytype>* currentTree = new BinomialTree<keytype>(currentNode, i);
            BinomialTree<keytype>* prevTree = nullptr;
            i--;
            BHeap<keytype> temp(currentTree, newSize-1);
            Node<keytype>* nextNode = currentNode->next;
            currentNode->parent = nullptr;
            currentNode->next = nullptr;
            currentNode->prev = nullptr;
            currentNode = nextNode;
            while(currentNode != nullptr){
                currentTree->setNext(new BinomialTree<keytype>(currentNode, i));
                currentTree->setPrev(prevTree);
                nextNode = currentNode->next;
                currentNode->parent = nullptr;
                currentNode->next = nullptr;
                currentNode->prev = nullptr;
                currentNode = nextNode;
                prevTree = currentTree;
                currentTree = currentTree->getNext();
                i--;
            }
            
            if(minRoot->getPrev() != nullptr){
                minRoot->getPrev()->setNext(minRoot->getNext());
            }
            if(minRoot->getNext() != nullptr){
                minRoot->getNext()->setPrev(minRoot->getPrev());
            }
            
            delete minRoot;
            
            this->merge(temp);
            
            return min;

        }

        /*Insert operation
            -inserts the key into the heap
            -fixes the heap
        Runtime: O(lgn) (O(1) amortized)
        */
        void insert(keytype k){
            //just creates a new head tree if the root list is empty 
            BinomialTree<keytype>* treeToInsert = new BinomialTree<keytype>(k);
            
            
            if(head == nullptr){
                head = treeToInsert;
                size++;
                return;
            }
            
            BHeap<keytype>* singletonHeap = new BHeap(treeToInsert, 1);

            this->merge(*singletonHeap);
            
        }

        /*Meld operation
            -merges BHeap H2 into the current heap
            -consumes H2
        Runtime: O(lgn)
        */
        void merge(BHeap<keytype> &H2){
            //if current heap is empty just change the head pointer
            if(head == nullptr){
                head = H2.head;
                size = H2.size;
                return;
            }
            
            //find the last root in the root list and make it's next the head of H2
            BinomialTree<keytype>* current = head;
        
            while(current->getNext() != nullptr){
                current = current->getNext();
            }
            
            
            
            
            
            
            current->setNext(H2.head);
            H2.head->setPrev(current);
            //root lists are now connected
            

            this->size += H2.size;
            this->fixHeap(); 
            
        }

        /*Print 
            -writes the key in the heap
            -prints for each binomial tree
            -
        */
        void printKey(){
            
            
            
            
            BinomialTree<keytype>* degrees[maxDeg()];
            BinomialTree<keytype>* current = head;
            for(int i = 0; i < maxDeg(); i++){
                degrees[i] = nullptr;
            }
            while(current!=nullptr){
                
                degrees[current->getDegree()] = current;
                current = current->getNext();
            }

            for(int i = 0; i < maxDeg(); i++){
                
                if(degrees[i] != nullptr){
                    cout << "B" << i << endl;
                    degrees[i]->printTree(degrees[i]->getHead());
                    cout << endl;
                }
            }
        }

        

        




};