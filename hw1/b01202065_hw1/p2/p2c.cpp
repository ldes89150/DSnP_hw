#include <iostream>
#include <vector>
#include <string>

using namespace std;

template  <class T>
class Compare
{
public:
    bool operator ()(T& a,T& b) const
   {
        return compare(a,b);
    }
private:
    virtual bool compare(T& a,T& b) const = 0 ;
};

template  <class T>
class Less : public Compare<T>
{
private:
       virtual bool compare(T& a,T& b) const
       {
            return (bool) (a<b);
       }
};

template  <class T>
class Greater: public Compare<T>
{
private:
       virtual bool compare(T& a,T& b) const
       {
            return (bool) (a>b);
       }
};

template  <class T>
void mySwap(T& a,T& b);

template  <class T>
void selectionSort(vector<T>& array, const Compare<T>& compare);


int main()

{
    const Less<string> compare_less;
    int n;
    cout<<"How many strings? ";
    cin>>n;
    vector<string> numvec(n);
    for(vector<string>::iterator c = numvec.begin(); c != numvec.end(); c++)
    {
        cin>>*c;
    }
    cout<<"Before sort:"<<endl;
    for(vector<string>::iterator c = numvec.begin(); c != numvec.end(); c++)
    {
        cout<<*c<<" ";
    }
    cout<<endl;
    selectionSort<string>(numvec,compare_less);
    cout<<"Ascending sort:"<<endl;
    for(vector<string>::iterator c = numvec.begin(); c != numvec.end(); c++)
    {
        cout<<*c<<" ";
    }

    cout<<endl<<endl;
    const Greater<double> compare_greater;
    cout<<"How many doubles? ";
    cin>>n;
    vector<double> numvec2(n);
    for(vector<double>::iterator c = numvec2.begin(); c != numvec2.end(); c++)
    {
        cin>>*c;
    }
    cout<<"Before sort:"<<endl;
    for(vector<double>::iterator c = numvec2.begin(); c != numvec2.end(); c++)
    {
        cout<<*c<<" ";
    }
    cout<<endl;
    selectionSort<double>(numvec2,compare_greater);
    cout<<"Descending sort:"<<endl;
    for(vector<double>::iterator c = numvec2.begin(); c != numvec2.end(); c++)
    {
        cout<<*c<<" ";
    }
    cout<<endl;
}

template  <class T>
void mySwap(T& a,T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

template  <class T>
void selectionSort(vector<T>& array, const Compare<T>& compare)
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
            mySwap<T>(array[pivot], array[i]);
    }
}
