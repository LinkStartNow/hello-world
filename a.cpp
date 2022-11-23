#include<bits/stdc++.h>

#define run(i, a, b) for(int i = a; i <= b; i ++)

using namespace std;

const int N = 1007;

int f[N], n, tot, ans;

char s[2];

int fa(int x)
{
    return x == f[x] ? x : x = fa(f[x]);
}

struct lyz
{
    int a, b, v;
    bool operator<(const lyz &b) const
    {
        return v < b.v;
    }
} e[N];

int main()
{
    cin >> n; run(i, 1, n) cin >> s; // 笑死，处理没用数据
    run(i, 1, n) f[i] = i;
    while(cin >> s)
    {
        e[tot].a = s[0] - 'A';
        cin >> s >> e[++ tot].v;
        e[tot].b = s[0] - 'A';
    }
    sort(e + 1, e + 1 + tot);
    run(i, 1, tot)
    {
        int x = fa(e[i].a), y = fa(e[i].b);
        if(x == y) continue;
        ans += e[i].v;
        f[x] = y;
    }
    cout << ans << endl;
}