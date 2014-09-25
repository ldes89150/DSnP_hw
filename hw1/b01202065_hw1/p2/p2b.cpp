#include <iostream>
#include <vector>


using namespace std;



class Compare
{
public:
    bool operator ()(int& a,int& b) const
   {
        return compare(a,b);
    }
private:
    virtual bool compare(int& a,int& b) const = 0 ;
};

class Less: public Compare
{
private:
       virtual bool compare(int& a,int& b) const
       {
            return (bool) (a<b);
       }
};

class Greater: public Compare
{
private:
       virtual bool compare(int& a,int& b) const
       {
            return (bool) (a>b);
       }
};

void mySwap(int& a,int& b);
void selectionSort(vector<int>& array, const Compare& compare);

int main()
{
    const Less compare_less;
    const Greater compare_greater;
    int n;
    cout<<"How many numbers? ";
    cin>>n;

    vector<int> numvec(n);
    for(vector<int>::iterator c = numvec.begin(); c != numvec.end();c++)
    {
        cin>>*c;
    }
    cout<<"Before sort:"<<endl;
    for(vector<int>::iterator c = numvec.begin(); c != numvec.end();c++)
    {
        cout<<*c<<" ";
    }
    cout<<endl;
    selectionSort(numvec,compare_less);
    cout<<"Ascending sort:"<<endl;
    for(vector<int>::iterator c = numvec.begin(); c != numvec.end();c++)
    {
        cout<<*c<<" ";
    }
    cout<<endl;
    selectionSort(numvec,compare_greater);
    cout<<"Descending sort:"<<endl;
    for(vector<int>::iterator c = numvec.begin(); c != numvec.end();c++)
    {
        cout<<*c<<" ";
    }
    cout<<endl;
    return 0;
}

void mySwap(int& a,int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
    return;
}

void selectionSort(vector<int>& array, const Compare& compare)
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


