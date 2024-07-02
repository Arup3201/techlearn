#include <bits/stdc++.h>
using namespace std;

int main()
{
    vector<int> v = {1, 2, 3, 4};
    cout << for_each(v.begin(), v.end(), [](int a)
                     { cout << (a * a * a) << " "; })
         << endl;
    return 0;
}