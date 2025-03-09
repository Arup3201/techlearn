#include <bits/stdc++.h>
#define fastIO                    \
    ios_base::sync_with_stdio(0); \
    cin.tie(0), cout.tie(0);
#define pb push_back
#define mp make_pair
using namespace std;
#define uint unsigned int
#define ll long long
#define printyes cout << "YES\n"
#define printno cout << "NO\n"
#define F first
#define S second
#define space " "
#define all(x) (x).begin(), (x).end()
#define dall(x) (x).rbegin(), (x).rend()
#define R cout << "running" << endl;
#define rep(i, a, b) for (int i = (a); i < (b); i++)
// const int mod = 1e9+7;
const int mod = 998244353;

void solve()
{
    string s;
    cin >> s;
    s += '$';
    int n;
    n = s.size();

    // p -> suffix array int, c -> equivalence class
    vector<int> p(n), c(n);

    // 1. k = 0:
    // sort characters with length 1
    // create array p that contains initial suffix array integers

    // single character
    vector<pair<char, int>> a(n); // (s[i], i)
    for (int i = 0; i < n; i++)
        a[i] = {s[i], i};

    sort(a.begin(), a.end());

    for (int i = 0; i < n; i++)
        p[i] = a[i].second;

    // calculate the equaivalenve class for each s[i..i+2^k-1]
    c[p[0]] = 0;
    for (int i = 1; i < n; i++)
    {
        if (a[i].first == a[i - 1].first)
        {
            c[p[i]] = c[p[i - 1]];
        }
        else
        {
            c[p[i]] = c[p[i - 1]] + 1;
        }
    }

    // strings with 2^k length
    int k = 0;
    while ((1 << k) < n)
    {
        // k -> k+1
        vector<pair<pair<int, int>, int>> x(n);
        for (int i = 0; i < n; i++)
        {
            x[i] = {{c[i], c[(i + (1 << k)) % n]}, i};
        }

        sort(x.begin(), x.end());

        for (int i = 0; i < n; i++)
            p[i] = x[i].second;

        c[p[0]] = 0;
        for (int i = 1; i < n; i++)
        {
            if (x[i].first == x[i - 1].first)
            {
                c[p[i]] = c[p[i - 1]];
            }
            else
            {
                c[p[i]] = c[p[i - 1]] + 1;
            }
        }

        k++;
    }

    for (int i = 0; i < n; i++)
    {
        cout << p[i] << " " << s.substr(p[i], n) << "\n";
    }
}

int main()
{
    solve();
    return 0;
}