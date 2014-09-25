#include <iostream>
#include "p1a.h"
using namespace std;


void printSize()
{
    P1a a;
    cout<<"The size of class P1a is "<<sizeof(a)<<"."<<endl;
}

void printStaticArraySize()
{
    P1a arr1b_1[5];
    cout<<"============================"<<endl;
    cout<<"Addresses of arr1b_1[5] are:"<<endl;
    for(int i=0;i<5;i++)
    {
        cout<<"&arr1b_1["<<i<<"]: "<<&arr1b_1[i]<<endl;
    }
}

void printDynamicArraySize()
{
    P1a *arr1b_2 = new P1a[5];
    cout<<"============================"<<endl;
    cout<<"Addresses of arr1b_2[5] are:"<<endl;
    for(int i=0;i<5;i++)
    {
        cout<<"&arr1b_2["<<i<<"]: "<<&arr1b_2[i]<<endl;
    }
}

void printPointerArraySize()
{
    P1a **arr1b_3 = new P1a *[5];
    for (size_t i = 0; i < 5; ++i)
        arr1b_3[i] = new P1a;
    cout<<"============================"<<endl;
    cout<<"Addresses of arr1b_3[5] are:"<<endl;
    for(int i=0;i<5;i++)
    {
        cout<<"&arr1b_3["<<i<<"]: "<<&arr1b_3[i]<<endl;
    }
    cout<<"============================"<<endl;
    cout<<"Contents of arr1b_3[5] are:"<<endl;
    for(int i=0;i<5;i++)
    {
        cout<<"arr1b_3["<<i<<"]: "<<arr1b_3[i]<<endl;
    }
}
