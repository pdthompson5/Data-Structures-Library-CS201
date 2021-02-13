#include <iostream>
#include <stdlib.h> 
using namespace std;

/**@author Philip Thompson
 * @last_edit 1/22/2021
 * This class implements a Circular Dynamic Array abstract data type.
 * As this class is a library, it does not include a user interface
 */




template <typename elmtype>
class CDA{
    private:
        elmtype* arrayStart;
        //returns this uninstantiated value if there is a out of bounds access 
        elmtype junkValue;
        unsigned int startIndex;
        unsigned int endIndex;
        int size;
        int capacity;
        //0 = unordered, 1 = ordered ascending, -1 = ordered descending
        int ordered;

        /* Resizes the array to the new size
        Procedure
            create a copy array
            free current array 
        status: test
        */
        void Resize(int newCapacity){
            //cout << "Resize " << capacity << " to " << newCapacity << endl;
            //create copy
            //cout << startIndex << endl;
            elmtype* temp = new elmtype [capacity];
            for(int i = 0; i < size; i++){
                temp[i] = arrayStart[(startIndex + i) % capacity];
            }
            
            //delete old
            delete [] arrayStart;

            
            


            //allocate new 
            arrayStart = new elmtype [newCapacity];

            //fill out new array with altered capacity 
            capacity = newCapacity;
            startIndex = 0;
            endIndex = size-1;
            for(int i = 0; i < size; i++){
                arrayStart[i] = temp[i];
            }
            delete [] temp;
        }

        int binarySearchAsc(elmtype e, int start, int end){
            elmtype current;
            if(end-start >= 0){
                
                int mid = (start + end) / 2;
                //cout << mid << endl;
                current = (*this)[mid];
                if(current == e){
                    return mid;
                }
                else if(current > e){
                    return binarySearchAsc(e, start, mid-1);
                }
                else{
                    return binarySearchAsc(e, mid+1, end);
                }
            }
            return -1;
        }

        int binarySearchDesc(elmtype e, int start, int end){

            elmtype current;
            if(end-start >= 0){
                
                int mid = (start + end) / 2;
                //cout << mid << endl;
                current = (*this)[mid];
                if(current == e){
                    return mid;
                }
                else if(current < e){
                    return binarySearchDesc(e, start, mid-1);
                }
                else{
                    return binarySearchDesc(e, mid+1, end);
                }
            }
            return -1;
        }
        
        


    


        elmtype quickSelect(elmtype arr[], int k, int size){
            //selects a random pivot element within range 0-size-1
            int pivotAddress = rand() % size;
            elmtype pivot = arr[pivotAddress];

            int LSize = 0;
            int ESize = 0;
            int GSize = 0;

            //Create three arrays L = less, E = equal, G = greater
            elmtype L[size];
            elmtype E[size];
            elmtype G[size];

            elmtype x;
            //Place each element in the array into their appropriate subarrays
            for(int i = 0; i < size; i++){
                x = arr[i];
                if(x < pivot){
                    L[LSize] = x;
                    LSize++;
                }
                else if(x == pivot){
                    E[ESize] = x;
                    ESize++;
                }
                else{
                    G[GSize] = x;
                    GSize++;
                }
            }

            //Runs select on L if k is in L
            if(k <= LSize){
                return quickSelect(L, k, LSize);
            }
            //returns pivot if k is the pivot
            else if(k <= LSize + ESize){
                return pivot;
            }
            //Runs select on G taking away the smaller elements taken away
            else{
                return quickSelect(G, k - LSize - ESize, GSize);
            }

        }


        void merge(int l, int m, int r){
            
            int LSize = m-l+1;
            int RSize = r-m;

            //Creates two subarrays containing the left and right elements
            elmtype L[LSize];
            for(int a = 0; a < LSize; a++){
                L[a] = (*this)[l+a];
            }
            elmtype R[RSize];
            for(int a = 0; a < RSize; a++){
                R[a] = (*this)[m+a+1];
            }

            //Array index
            int i = l;
            //Left index
            int j = 0;
            //Right index
            int k = 0;
            while((j<LSize) && (k<RSize)){
                //If Right element is larger add it to array
                if(R[k] > L[j]){
                    (*this)[i] = R[k];
                    i++;
                    k++;
                }
                //If Left element is larger add it to array
                else if(R[k] < L[j]){
                    (*this)[i] = L[j];
                    i++;
                    j++;
                }
                //Else add both
                else{
                    (*this)[i] = L[j];
                    i++;
                    j++;
                    (*this)[i] = R[k];
                    i++;
                    k++;
                }
            }

            //If any elements remain add them to the array
            while(j < LSize){
                (*this)[i] = L[j];
                i++;
                j++;
            }

            while(k < RSize){
                (*this)[i] = R[k];
                i++;
                k++;
            }


        }


        void mergeSortRec(int l, int r){
            if(l < r){
                int m = (l+r) / 2;

                mergeSortRec(l, m);
                mergeSortRec(m+1, r);

                merge(l, m, r);
            }
        }

    public:

        /*Runtime: O(1)
        Creates a array of 
            capacity = 1
            size = 0
            ordered = 0
        */
        CDA(){
            //allocates on space
            arrayStart = new elmtype [1];
            endIndex = -1;
            startIndex = -1;
            size = 0;
            capacity = 1;
            ordered = 0;   
        }



        /*Runtime: O(1)
        Creates an array of
            capacity = s
            size = s
            ordered = 0

        This function is intended to be used alongside manual data assignment using the access operator.
        This function works well with the traditional for-loop style array assignment:
            CDA sample(size)
            for(int i = 0; i < size; i++){
                sample[i] = i;
            }
        This function only allocates the memory with no assignments. It assumes that the user will fill out the spaces they allocated.
        */
        CDA(int s){
            //allocates s spaces
            arrayStart = new elmtype [s];
            endIndex = s-1;
            startIndex = 0;
            size = s;
            capacity = s;
            ordered = 0; 
        }



        /*Runtime: O(n)
        Copy constructor
            -Creates a deep copy of the array 
        */
        CDA(CDA &a){
            //shallow copy of all fields
            size = a.Length();
            capacity = size + a.EmptySlots();
            arrayStart = new elmtype [capacity];
            startIndex = 0;
            endIndex = size;
            //deep copy of the array
            for(int i = 0; i < a.Length(); i++){
                arrayStart[i] = a[i]; 
            }
        }
        

        
        /*Runtime: O(1)
        Destructor 
        */
        ~CDA(){
            //Delete the only value manually allocated 
            delete [] arrayStart;
        }



        /*Runtime: 
        Copy assignment constructor 
            -creates a deep copy of the array and returns a reference 
        */
        CDA& operator=(CDA& a){
            //uses the copy constructure to create a deep copy
            size = a.Length();
            capacity = size + a.EmptySlots();
            arrayStart = new elmtype [capacity];
            startIndex = 0;
            endIndex = size;
            //deep copy of the array
            for(int i = 0; i < a.Length(); i++){
                arrayStart[i] = a[i]; 
            }

            return *this;
        }


        

        
        /*Runtime: O(1) 
        Returns a refrence to the elmtype at address i
        Checks if out of bounds 
            returns the junkValue if the index is out of bounds
        */
        elmtype& operator[](int i){
            //checks if within bounds 
            if(i < 0 || i > (size-1)){
                cout << "Index out of bounds" << endl;
                return junkValue;
            }
            //the % operation enables the indexing to be circular 
            return arrayStart[(startIndex + i) % capacity];
        }
        

        
        /*Runtime: O(1) amortized
        increases size by 1
        stores at the end of the array 
        doubles capacity if size == capacity
        if ordered is true then check if is still true after
            It only needs to compare the new and previous elements
        */
        void AddEnd(elmtype v){
            //resize if full
            if(size == capacity){
                this->Resize(capacity * 2);
            }   

            //change endIndex to the next index, maintaining circularity
            endIndex = (endIndex + 1) % capacity;

            //add element
            arrayStart[endIndex] = v;

            //if first element in a new array, fix the startIndex
            if(startIndex == -1){
                startIndex = 0;
            }


            //checks if the array is still ordered
            if(ordered == 1){
                if(v < arrayStart[(endIndex - 1) % capacity]){
                    ordered = 0;
                }
            }
            else if(ordered == -1){
                if(v > arrayStart[(endIndex - 1) % capacity]){
                    ordered = 0;
                }
            }

            size++;
        }
        

        
        /*Runtime: O(1) amortized
        increases size by 1
        stores at the front of the array 
        doubles capacity if size == capacity
        if ordered is true then check if is still true after
        */
        void AddFront(elmtype v){
            //resize if full
            if(size == capacity){
                this->Resize(capacity * 2);
            }

            //change startIndex to one before the previous, maintaining circularity
            startIndex = (startIndex - 1 + capacity) % capacity;

            //add the element
            arrayStart[startIndex] = v;

            //fixes index if first 
            if(endIndex == -1){
                endIndex = 0;
            }

            //checks if still ordered
            if(ordered == 1){
                if(v > arrayStart[(startIndex + 1) % capacity]){
                    ordered = 0;
                }
            }
            else if(ordered == -1){
                if(v < arrayStart[(startIndex + 1) % capacity]){
                    ordered = 0;
                }
            }
            //increment size
            size++;
        }
        

        
        /*Runtime: O(1) amortized
        reduces array size by one
        note: memory deallocation is not required, we do that in resize 
        shrinks the capacity if only 25% is being used
        Specification: capacity should never be below 4
        */
        void DelEnd(){
            size--;
            //check if resize is needed
            if(((double)(size-1) / capacity <= .25) && capacity > 4){
                if((capacity / 2) < 4){
                    this->Resize(4);
                }
                else{
                    this->Resize(capacity / 2);
                }   
            }
            
            //changes index to effectivley delete the end element 
            endIndex = (endIndex - 1 + capacity) % capacity;
              
        }



        /*Runtime: O(1) amortized
        reduces array size by one
        note: memory deallocation is not required, we do that in resize 
        shrinks the capacity if only 25% is being used
        capacity should never be below 4
        */
        void DelFront(){
            //check if resize is needed
            if(((double)(size-1) / capacity <= .25) && capacity > 4){
                if((capacity / 2) < 4){
                    this->Resize(4);
                 }
                 else{
                    this->Resize(capacity / 2);
                }  
            }
        
            //changes index to effectivley delete the front element 
            startIndex = (startIndex + 1) % capacity;
            
            size--;
          
        }

        
        

        /*Runtime: O(1)
        returns size
        */
        int Length(){
            return size;
        }
        

        
        /*Runtime: O(1)
        returns capcity - size
        */
        int EmptySlots(){
            return (capacity - size);
        }
        

        
        /*Runtime: O(1)
        frees all current array allocations
        restarts with array of capacity 1 and size 0
        */
        void Clear(){
            delete [] arrayStart;
            arrayStart = new elmtype [1];
            endIndex = -1;
            startIndex = -1;
            size = 0;
            capacity = 1;
            ordered = 0;   
        }
        

        
        /*Runtime: O(1)
        returns ordered flag
        1 = increasing
        -1 = decreasing
        0 = unordered
        */
        int Ordered(){
            return ordered;
        }
        

        
        /*Runtime: O(size)
        checks order
        1 = increasing
        -1 = decreasing
        0 = unordered
        */
        int SetOrdered(){
            //these ints will be set to zero if false
            int increasing = 1;
            int decreasing = -1;
            elmtype current, next;
            current = arrayStart[startIndex];

            //iterate through the array with next = arrayStart[i]
            //if increasing or decreasing is false, set it
            for(int i = 1; i < size; i++){
                next = arrayStart[(startIndex + i) % capacity];
                if(current > next){
                    //cout << current << ">" << next << endl;
                    increasing = 0;
                }
                if(current < next){
                    //cout << current << "<" << next << endl;
                    decreasing = 0;
                }

                current = next;
            }

            //returns 1 if both are true - only when all values are the same 
            if(increasing == 1 && decreasing == -1){
                ordered = 1;
                return 1;
            }
            ordered = (increasing + decreasing);
            return(increasing + decreasing);
        }
        




    //Well-defined complex functions
        //The strucutre of the CDA is not dependent on these functions 
    
        /*Runtime: O(1) if ordered, O(size) else
        Returns the kth size element in the array 
        If the array is sorted, this problem is trivial 
        If the array is unsorted this function utilizes a "quickSelect" algorithm similar to quickSort 
        */
        elmtype Select(int k){
            
            //ordered ascending 
            if(ordered == 1){
                return (*this)[k-1];
            }
            //ordered descending
            else if(ordered == -1){
                return (*this)[size - k];
            }
            else{
                elmtype list[size];
                for(int i = 0; i < size; i++){
                    list[i] = (*this)[i];
                }
                return quickSelect(list, k, size);
            }
            
        }
        

        
        /*Runtime: O(size*size)
        Executes a simple descending order insertion sort on the CDA
        */
        void InsertionSort(){
            ordered = -1;
            int j, i, k;
            for(i = 1; i < size; i++){
                k = (*this)[i];
                j = i - 1;
                while(j >= 0 && (*this)[j] < k){
                    (*this)[j+1] = (*this)[j];
                    j--;
                }
                (*this)[j+1] = k;
            }
        }
        

        
        /*Runtime: O(size lg(size))
        Executes a descending order merge sort on the CDA
        */
        void MergeSort(){
            ordered = -1;
            mergeSortRec(0, size-1);
        }
        

        
        /*Runtime: O(size + m)
        Executes a descending order counting sort on the CDA
        This function only works with an array of integers 
        */
        void CountingSort(int m){
            ordered = -1;
            int count[m+1];
    

            for(int i = 0; i < m+1; i++){
                count[i] = 0;
            }

            for(int i = 0; i < size; i++){
                count[(*this)[i]]++;
            }

            int temp;
            int total = 0;
            for(int i = 0; i < m+1; i++){
                temp = count[i];
                count[i] = total;
                total += temp;
            }

            
            elmtype* output = new elmtype[size];
            for(int i = 0; i < size; i++){
                output[count[(*this)[i]]] = (*this)[i];
                count[(*this)[i]] += 1;
            }    

            for(int i = 0; i < size; i++){
                (*this)[size - i - 1] = output[i];
            }

            delete [] output;
            
        }
        

        
        /*Runtime: O(lg size) if ordered, O(size) else
        Searches the array for the element with elmtype e
        Uses a binary search if the CDA is ordered
        otherwise it utilizes a linear search 
        */
        int Search(elmtype e){
            elmtype current;
            
            //binary search if ordered
            if(ordered == 1){
                return binarySearchAsc(e, 0, size-1);
            }
            else if(ordered == -1){
                return binarySearchDesc(e, 0, size-1);
            }
            else{
                //linear search unordered               
                for(int i = 0; i < size; i++){
                    current = (*this)[i];
                    if(current == e){
                        return i;
                    }
                }
            }
            return -1;

            
        }




};