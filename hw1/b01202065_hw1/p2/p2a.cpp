#include <iostream>
#include <vector>


using namespace std;

void mySwap(int& a,int& b);
bool compare(int a, int b);
void selectionSort(vector<int>& array);

int main()
{
    int n;
    cout<<"How many numbers? ";
    cin>>n;

    vector<int> numvec(n);
    for(int &c:numvec)
    {
        cin>>c;
    }
    cout<<"Before sort:"<<endl;
    for(int &c:numvec)
    {
        cout<<c<<" ";
    }
    cout<<endl;
    selectionSort(numvec);
    cout<<"After sort:"<<endl;
    for(int &c:numvec)
    {
        cout<<c<<" ";
    }
    cout<<endl;

}

void mySwap(int& a,int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
    return;
}

bool compare(int a, int b)
{
    if(a<b)
    {
        return true;
    }
    else
    {
        return false;
    }

}


void selectionSort(vector<int>& array)
{
    for (size_t i = 0, n = array.size(); i < n - 1; ++i)
    {
        size_t pivot = i;
        for (size_t j = i+1; j < n; ++j)
        {
            if (!compare(array[pivot], array[j]))
                pivot = j;
        }
        if (pivot != i)
            mySwap(array[pivot], array[i]);
    }
}
