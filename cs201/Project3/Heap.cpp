#include <stdio.h>
using namespace std;


#include "CDA.cpp"
template <typename keytype>
class Heap{
    

    /*Properties:
        -Heap order property: Every parent is <= their child 
        -Heap structure property: Every level is filled except the last 
    */
    CDA<keytype> heap;
    private:
        //useful indexing helper functions
        int parent(int childIndex){
            return (childIndex-1) / 2;
        }
        int leftChild(int parentIndex){
            return parentIndex * 2 + 1;
        }
        int rightChild(int parentIndex){
            return parentIndex * 2 + 2;
        }

        //swap helper, ints are indexes 
        void swap(int a, int b){
            keytype temp = heap[a];
            heap[a] = heap[b];
            heap[b] = temp;
        }

        //k is the index of a node
        //should be safe for leaf nodes but does nothing to them
        void heapify(int k){
            int smallest = k;
            int left = leftChild(k);
            int right = rightChild(k);

            //finds the smallest of the node and its children 
            if(left < heap.Length() && heap[smallest] > heap[left]){
                smallest = left;
            }
            if(right < heap.Length() && heap[smallest] > heap[right]){
                smallest = right;
            }
            
            //if swap is required
            if(smallest != k){
                swap(smallest, k);
                //recursively fixes the subtrees if a swap is required
                heapify(smallest);
            }
        }


        

    public:
        /*Default constructor 
            -creates an empty heap
        Runtime O(1)
        Status: complete 
        */
        Heap(){
            
        }
        
        /*Array constructor 
            -creates a heap
            -Uses the bottom up method to insert all values in k
                -Heapify
        Runtime: O(s)
        */
        Heap(keytype k[], int s){
            for(int i = 0; i < s; i++){
                heap.AddEnd(k[i]);
            }


            //heapifies from the first internal node to the root 
            for(int i = parent(s-1); i >= 0; i--){
                heapify(i);
            }
            
        }

        /*Copy constructor
            -creates a deep copy of the heap
        Runtime: not given 
        Status: Complete
        */
        Heap(Heap &a){
            heap = a.heap;
        }

        /*Copy assignment constructor 
            -creates a deep copy of the heap and returns a reference
        Runtime: not given
        Status: Complete
        */
        Heap& operator=(Heap &a){
            heap = a.heap;
            return *this;
        }
        /*Destructor 
            -deallocates all dynamically allocated memory 
        Runtime O(1)
        Status: Testing
        */
        ~Heap(){
            
        }


        /*Peek operation
            -Returns the min key
            -does not alter the heap
        Runtime: O(1)
        Status: Complete 
        */
        keytype peekKey(){
            return heap[0];
        }

        /*Extract operation
            -Returns the min key
            -modifies the heap to maintain its properties
        Runtime: O(lgn)
        */
        keytype extractMin(){
            keytype temp = heap[0];

            //swap the min with the last
            swap(0, heap.Length() - 1);
            heap.DelEnd();

            int i = 0;
            keytype k = heap[0];
            int smallestChild;
            //while k is not a leaf 
            while(leftChild(i) < heap.Length()){
                //k only had one child 
                if(leftChild(i) == heap.Length() - 1){
                    smallestChild = leftChild(i);
                }
                //else find smaller child of the two
                else{
                    if(heap[leftChild(i)] < heap[rightChild(i)]){
                        smallestChild = leftChild(i);
                    }
                    else{
                        smallestChild = rightChild(i);
                    }
                }
                //if k is smaller than both break
                if(heap[smallestChild] > k){
                    break;
                }
                //else swap postions and continue
                else{
                    swap(smallestChild, i);
                    i = smallestChild;
                }
            }

            return temp;
        }

        /*Insert operation 
            -inserts key k
            -Alters the array to maintain its properties 
        Runtime: O(lgn)
        Status: Testing
        */
        void insert(keytype k){
            //add k to the end
            heap.AddEnd(k);
            int i = heap.Length() - 1;
            
            //while k is less than its parent and isn't the min, swap k and parent 
            while(i > 0 && (k < heap[parent(i)])){
                swap(i, parent(i));
                i = parent(i);    
            }
            
        }
        /*Print  
            -Prints the keys stored in the array
            -Starts at the root 
        Runtime: O(n)
        */
        void printKey(){
            for(int i = 0; i < heap.Length(); i++){
                cout << heap[i] << " ";
            }
            cout << endl;
        }
        




};
