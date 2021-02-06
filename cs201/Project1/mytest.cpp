#include <iostream>
using namespace std;
#include "CDA.cpp"

int main(){
    CDA<int> *list = new CDA<int>;

    list->AddFront(2);
    list->AddFront(6);
    // list.AddFront(4);
    // list.AddFront(3);
    // list.AddFront(1);

    list->AddEnd(2);
    list->AddEnd(6);
    // list.AddEnd(4);
    // list.AddEnd(3);
    // list.AddEnd(1);

    cout << (*list)[3] << endl;

    

    // for(int i = 0; i < list.Length(); i++){
    //     cout << list[i] << endl;
    // }

    

    // for(int i = 0; i < list.Length(); i++){
    //     cout << list[i] << endl;
        
    // }
    cout << "here" << endl;
    



    // list.SetOrdered();
    // cout << list.Ordered() << endl;
    // cout << list.Search(4) << endl;
    

    
    
    

    

    // for(int i = 0; i < list.Length(); i++){
    //     cout << list[i] << endl;
    // }

    // CDA<int> list2;
    // list2 = list;
    // list[4] = 18;

    // for(int i = 0; i < list2.Length(); i++){
    //     cout << list2[i] << endl;
    // }

   



}