#include <iostream>
#include <stdlib.h> 
using namespace std;



/*Two4Tree class
    


*/

template <typename keytype, typename valuetype>
class Two4Tree{

    struct Pair{
        keytype key; 
        valuetype val;
    };


    /*This class is a self ordering node for up to three value pair
    */
    class Node{
        public:
            //pointer to the three possible value pairs 
            //storage method: lowest on right, highest on left 
            Pair** vals;
            //pointer to an array of pointers, useful for swaping children 
            Node** children;
            Node* parent;
            int size;
            int numChildren;
            int keysBelow;

            //returns 1 if a > b, -1 if b > a and 0 else
            //status: Testing 
            int comparePairs(Pair* a, Pair* b){
                if(a->key > b->key){
                    return 1;
                }
                else if(b->key > a->key){
                    return -1;
                }
                else{
                    return 0;
                }
            }

            
            Node(){
                
                vals = new Pair*[3];
                for(int i = 0; i < 3; i++){
                    vals[i] = nullptr;
                }
                children = new Node*[4];
                for(int i = 0; i < 4; i++){
                    children[i] = nullptr;
                }
                parent = nullptr;
                size = 0;
                numChildren = 0;
                keysBelow = 0;
            }
            ~Node(){
                for(int i = 0; i < size; i++){
                    if(vals[i]!=nullptr){
                        delete vals[i];
                    }    
                }
                delete [] vals;
                
            }


            //not sure if this function is needed since I'm always passing by reference
            //right now this doesn't manage children 
            Node(Node& a){
                vals = new Pair*[3];
                for(int i = 0; i < 3; i++){
                    vals[i] = nullptr;
                }
                children = new Node*[4];
                for(int i = 0; i < 4; i++){
                    children[i] = nullptr;
                }

                for(int i = 0; i < a.size; i++){
                    this->addPair(a.vals[i].key, a.vals[i].val);
                }
            }


            void updateParentKeysBelow(Node* current, int n){
                Node* ancestor = parent;
                while(ancestor!=nullptr){
                    ancestor->keysBelow += n;
                    ancestor = ancestor->parent;
                }
            }
            void addPair(keytype keyToAdd, valuetype valToAdd){
                Pair* addedPair = new Pair();
                addedPair->key = keyToAdd;
                addedPair->val = valToAdd;

                

                int comparison;
                //case: empty node 
                if(size == 0){
                    vals[0] = addedPair;
                    size++;
                    return;
                }

                //this acts to balance the inserts 
                for(int i = 0; i < size; i++){
                    comparison = comparePairs(vals[i], addedPair);
                    if(comparison == 1){
                        //found index to insert, shift up
                        for(int j = size; j > i; j--){
                            vals[j] = vals[j-1];
                        }
                        vals[i] = addedPair;
                        size++;
                        return;
                    }
                }
                //larger than all pairs
                vals[size] = addedPair;
                size++;
                return;
                
            }
            
            //overload if you already have the pair to add
            void addPair(Pair* addedPair){

                

                int comparison;
                //case: empty node 
                if(size == 0){
                    vals[0] = addedPair;
                    size++;
                    return;
                }

                //this acts to balance the inserts 
                for(int i = 0; i < size; i++){
                    comparison = comparePairs(vals[i], addedPair);
                    if(comparison == 1){
                        //found index to insert, shift up
                        for(int j = size; j > i; j--){
                            vals[j] = vals[j-1];
                        }
                        vals[i] = addedPair;
                        size++;
                        return;
                    }
                }
                //larger than all pairs
                vals[size] = addedPair;
                size++;
                return;
            }

            //passes the burden of memory management to the user
            //Didn't want to memory manage here so I can reuse the pairs and add them to different nodes 
            Pair* removePair(int indexToRemove){
                Pair* temp = vals[indexToRemove];

                

                if(indexToRemove == size - 1){
                    vals[indexToRemove] = nullptr;
                    size--;
                    return temp;
                }
                else{
                    int i;
                    for(i = indexToRemove; i < size - 1; i++){
                        vals[i] = vals[i+1];
                    }
                    vals[i] = nullptr;
                    size--;
                    return temp;
                }             
            }

            Pair* removePairViaKey(keytype keyToRemove){
                int indexToRemove;
                for(indexToRemove = 0; indexToRemove < size; indexToRemove++){
                    if(vals[indexToRemove]->key == keyToRemove){
                        break;
                    }
                }
                Pair* temp = vals[indexToRemove];

                

                if(size == 1){
                    vals[0] = nullptr;
                }
                else if(size == 2){
                    if(indexToRemove == 0){
                        vals[0] = vals[1];
                        vals[1] = nullptr;
                    }
                    else if(indexToRemove == 1){
                        vals[1] = nullptr;
                    }
                }
                else if(size == 3){
                    if(indexToRemove == 0){
                        vals[0] = vals[1];
                        vals[1] = vals[2];
                        vals[2] = nullptr;
                    }
                    else if(indexToRemove == 1){
                        vals[1] = vals[2];
                        vals[2] = nullptr;
                    }
                    else if(indexToRemove == 2){
                        vals[2] = nullptr;
                    }
                }   
                
                size--;
                return temp; 
                
            }
            void printNode(){        

                for(int i = 0; i < size; i++){
                    cout << vals[i]->key << " " << flush;
                }
            }
            valuetype* searchNode(keytype keyToFind){
                for(int i = 0; i < size; i++){
                    if(vals[i]->key == keyToFind){
                        return &(vals[i]->val);
                    }
                }

                return nullptr;
            }
            
            //based on the key given, returns the next node to check
            Node* determinePath(keytype key){
                int i = 0;
                
                for(i = 0; i < size; i++){
                    if(vals[i]->key > key){
                        break;
                    }
                }
                

                return children[i];
            }
            
            //balanced insert 
            //It doesn't matter which key is used for comparisons since they all fall into the same bucket 
            void addChild(Node* newChild){
                
                int comparison;

                if(newChild == nullptr){
                    return;
                }

                
                newChild->parent = this;
                

                
                
                if(numChildren == 0){
                    children[0] = newChild;
                    numChildren++;
                    return;
                }
                else{
                    
                    for(int i = 0; i < numChildren; i++){
                        
                        comparison = comparePairs(children[i]->vals[0], newChild->vals[0]);
                       
                        if(comparison == 1){
                            for(int j = numChildren; j > i; j--){
                                children[j] = children[j-1];
                            }
                            children[i] = newChild;
                            numChildren++;
                            return;
                        }
                    }
                    
                    children[numChildren] = newChild;
                    numChildren++;
                    return;
                }
                
            }

            Node* removeChild(int index){
                
                Node* temp = children[index];
                children[index] = nullptr;
                if(temp != nullptr){
                    numChildren--;
                    
                }
                
                
                return temp;
            }

            Node* removeChild(Node* childToRemove){
                int indexToRemove;
                for(int i = 0; i < numChildren; i++){
                    if(children[i] == childToRemove){
                        indexToRemove = i;
                        break;
                    }
                }

                Node* temp = children[indexToRemove];

            

                if(indexToRemove == numChildren - 1){
                    children[indexToRemove] = nullptr;
                    numChildren--;
                    return temp;
                }
                else{
                    for(int i = indexToRemove; i < numChildren-1; i++){
                        children[i] = children[i+1];
                    }
                    children[numChildren-1] = nullptr;
                    numChildren--;
                    return temp;
                }
            }
    };

    private:
        int Size;
        Node* root;

        void merge(Node* current){

        }
        Node* split(Node* current, keytype k){

            //remove the right and left pairs
            Pair* rightPair = current->removePair(2);
            Pair* leftPair = current->removePair(0);

            //create the new nodes 
            Node* rightNode = new Node();
            Node* leftNode = new Node();

            rightNode->addPair(rightPair);
            leftNode->addPair(leftPair);


            if(current->children[0]!=nullptr){
                leftNode->keysBelow += current->children[0]->keysBelow + current->children[0]->size;
            }
            if(current->children[1]!=nullptr){
                leftNode->keysBelow += current->children[1]->keysBelow + current->children[1]->size;
            }
            if(current->children[2]!=nullptr){
                rightNode->keysBelow += current->children[2]->keysBelow + current->children[2]->size;
            }
            if(current->children[3]!=nullptr){
                rightNode->keysBelow += current->children[3]->keysBelow + current->children[3]->size;
            }


            

            //manage child nodes
            leftNode->addChild(current->removeChild(0));
            leftNode->addChild(current->removeChild(1));
            rightNode->addChild(current->removeChild(2));
            rightNode->addChild(current->removeChild(3));

            //split root 
            if(current->parent == nullptr){ 
                current->addChild(leftNode);
                current->addChild(rightNode);
                current->keysBelow += 2;
                return current;
            }
            else{
                
                Node* parent = current->parent;
                
                parent->addPair(current->removePair(0));
                
                parent->removeChild(current);
                parent->addChild(leftNode);
                parent->addChild(rightNode);

                parent->keysBelow--;
                 
                
                delete current;
                
            
                return parent;    
            }
            

            
        }

        void inorderRecursive(Node* current){
            if(current == nullptr){
                return;
            }

            for(int i = 0; i < current->size; i++){
                //left
                inorderRecursive(current->children[0]);
                cout << current->vals[i]->key << " ";
                //right
                if(i == current->size-1){
                    inorderRecursive(current->children[current->size]);
                }  
            }    

        }
        void preorderRecursive(Node* current){
            current->printNode();
            if(current->numChildren == 0){
                return;
            }
            for(int i = 0; i < current->numChildren; i++){
                preorderRecursive(current->children[i]);
            }
            
        }
        void postorderRecursive(Node* current){
            if(current->numChildren == 0){
                current->printNode();
                return;
            }
            for(int i = 0; i < current->numChildren; i++){
                postorderRecursive(current->children[i]);
            }
            current->printNode();
            
        }
        

        keytype selectRecursive(Node* current, int pos){
            int i;
            for(i = 0; i < current->size; i++){
                
                if(pos == 0){
                    return current->vals[i]->key;
                }
                else if(current->numChildren == 0){
                    if(pos==1){
                        return current->vals[i]->key;
                    }
                    pos--;
                }
                else if(pos == current->children[i]->keysBelow + current->children[i]->size + 1){
                    return current->vals[i]->key;
                }
                else if(pos < current->children[i]->keysBelow + current->children[i]->size + 1){
                    
                    return selectRecursive(current->children[i], pos);
                }
                else{
                    pos -= current->children[i]->keysBelow + current->children[i]->size + 1;
                }
                
            }
            return selectRecursive(current->children[i], pos);

        }

        valuetype* recursiveSearch(Node* root, keytype k){
            //Base case: key not in tree
           
            if(root == nullptr){
                return nullptr;
            }
            

            //Base case: root contains the keytype 
            valuetype* search = root->searchNode(k);  
            if(search != nullptr){
                return search;
            }
            

            //recursive case: 
            return recursiveSearch(root->determinePath(k), k);
        }
    
        int rankRecursive(Node* current, int n, keytype k){
            if(current == nullptr){
                return 0;
            }
            

            int i = 0;
            for(i = 0; i < current->size; i++){
                if(current->vals[i]->key == k){
                    if(current->numChildren > 0){
                        n += current->children[i]->keysBelow + current->children[i]->size;
                    } 
                    return n + i + 1;
                }
                else if(current->vals[i]->key > k){
                    
                    return rankRecursive(current->children[i], n, k);
                }
                else{
                    if(current->numChildren > 0){
                        n += current->children[i]->keysBelow + current->children[i]->size + 1;
                    } 
                }
            } 
            return rankRecursive(current->children[i], n, k);
        }
        Node* nodeSearch(Node* current, keytype k){
            
            if(current->searchNode(k) != nullptr){
                return current;
            }
            if(current->determinePath(k) == nullptr){
                return nullptr;
            }
            return nodeSearch(current->determinePath(k), k);
        }
    public:

        void printTree(){
            root->printNode();
            
            for(int i = 0;i < root->numChildren; i++){
                cout << endl;
                root->children[i]->printNode();
                
            }

            for(int i = 0;i < root->numChildren; i++){
                for(int j = 0; j < root->children[i]->numChildren; j++){
                    cout << endl;
                    root->children[i]->children[j]->printNode();
                }
            }
            
        }


        

        /*Runtime : O(1)
            Default constructor 
            tree should be empty
            root = nullptr;
            size = 0;
            status: Testing
           
        */
        Two4Tree(){
            root = nullptr;
            Size = 0;
        }


        /*Runtime: O(s lgs)
            Construct a tree based a linked arrays of values and keys with size s
            A sequence of inserts should be the best way

            status: testing 
            
        */
        Two4Tree(keytype k[], valuetype v[], int s){
            root = nullptr;
            Size = 0;

            for (int i = 0; i < s; i++){
                this->insert(k[i], v[i]);
            }         
        }


        /*Runtime: Not given
            Make a deep copy of a tree
            does this depend on a node copy constructor?
            be sure to manage child and parent pointers in this method

            Status: Incomplete
        */
        Two4Tree(Two4Tree& a){

            for(int i = 0; i < a.size(); i++){

            }

        }

        /*Runtime: Not given
            Make a deep copy of a then return a reference

            Status: Incomplete
        */
        Two4Tree& operator=(Two4Tree& a){

        }

        /*Runtime: O(lgn)
            Deallocate all heap allocated memory 

            status: Incomplete 
            

        */
        ~Two4Tree(){

        }



        /*Runtime: O(lgn)
            Traditional search
            Will probably look a lot like a binary search tree search 
            Status: testing 
        */
        valuetype* search(keytype k){           
            return this->recursiveSearch(root, k);
        }
        
        /*Runtime: O(lgn)
            Insert a node into the tree 
            Cases:



            Status: Deep testing 
        */
        void insert(keytype k, valuetype v){
            
            if(root == nullptr){
                root = new Node();
                root->addPair(k, v);
                return;
            }
            

            // search for place to insert
            // if a three node is found execute a split
            // I just need to find the node in which to insert then addPair(k, v) and do premptive splits on the way down 
            Node* search = root;
            if(search->size == 3){
                search = this->split(search, k);
            } 
            while(search->determinePath(k) != nullptr){
        
                search = search->determinePath(k);
                if(search->size == 3){
                    search = this->split(search, k);
                }    
            }

            search->addPair(k, v);
            search->updateParentKeysBelow(search, 1);
            
            Size++;
         }

        /*Runtime: O(lgn)
            Removes the node with key k
            Calls search
            returns 1 if found 
            return 0 if not found
            cases:


            Status: Incomplete
        */
        int remove(keytype k){
            Node* current = nodeSearch(root, k);
            if(current == nullptr){
                return 0;
            }
            int i;
            for(i = 0; i < current->size; i++){
                if(current->vals[i]->key == k){
                    break;
                }
            }
            current->removePair(i);
            return 1;
        }

        /*Runtime: O(lgn)
            returns the rank of key k
            returns O if not found 
            The smallest item is rank 1

            Status: testing
        */
        int rank(keytype k){
            return rankRecursive(root, 0, k);   
        }

        /*Runtime: O(lg n)
            Returns the key at postion pos
            select(1) returns the smallest key
            select(n) returns the largest key

            Status: Incomplete 
        */
        keytype select(int pos){
            return selectRecursive(root, pos);
        }


        /*Runtime: O(lgn)
            returns the key after k in the tree
            based on rank?
            not the same as sucessor for a subtree 

            Status: testing

        */
        keytype successor(keytype k){
            return select(rank(k) + 1);      
        }


        /*Runtime: O(lg n)
            returns the key before k in the tree
            based on rank?
            not the same as sucessor for a subtree 

            Status: Testing 


        */
        keytype predecessor(keytype k){
            return select(rank(k) - 1);
        }

        /*Runtime: O(1)
            returns the size of the tree 
            simple
            status:Testing 
        */
        int size(){
            return Size;
        }


        /*Runtime: O(n)
            prints the preorder traversal
            groups all key in a node 

            Status: testing
        */
        void preorder(){
            this->preorderRecursive(root);
            cout << endl;
        }

        /*Runtime: O(n)
            prints the inorder traversal of the tree 
            does not group keys in a node

            Status: testing
        */
        void inorder(){
            this->inorderRecursive(root);
            cout << endl;
        }

        /*Runtime: O(n)
            prints the postorder traversal of the tree
            groups keys in a node

            Status: testing
        */
        void postorder(){
            this->postorderRecursive(root);
            cout << endl;
        }
        
};




int main(){
	string K[10] = {"A","B","C","D","E","F","G","H","I","K"};
	int V[10] = {10,9,8,7,6,5,4,3,2,1};
	
	Two4Tree<string,int> T1, T2(K,V,10);
	
	for(int i=0; i<10; i++) T1.insert(K[i],V[i]);
	// T1 and T2 should be identical trees
	
	cout << *(T2.search("C")) << endl;
	// Should output 8
	
	cout << T1.rank("C") << endl;
	//Should output 3
	
	cout << T1.select(3) << endl;
	//Should output C 
	
	T2.preorder();
	//Should output D B A C F H E G I K
	
	T2.inorder();
	//Should output	A B C D E F G H I K
	
	cout << T2.remove("C") << endl;
	//Should output 1
	
	T2.preorder();
	//Should output F B A D E H G I K
	
	T2.inorder();
	//Should output	A B D E F G H I K
	
	cout << T2.remove("J") << endl;
	//Should output 0
	
	cout <<T2.rank("G") << endl;
	//Should output 6
	
	cout <<T1.successor("C")<<endl;
	//Should output D
	
	cout <<T1.predecessor("C")<<endl;
	//Should output B		

	
    Two4Tree<int,int> X;
	for (int i=1;i<100000;i++) X.insert(i,i);
	for (int i=1;i<100000;i++) {
		if(X.rank(i) != i) cout << "Rank error" << endl;
		if(X.select(i) != i) cout << "Select error" << endl;
		if(*(X.search(i)) != i) cout << "Search error" << endl;
	}  
	//Should be no output and should take seconds, not minutes
	return 0;
}