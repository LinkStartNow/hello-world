#include<bits/stdc++.h>

#define run(i, a, b) for(int i = a; i <= b; i ++)
#define _ int
#define __ >>
#define ___ <<
#define ____ f
#define _____ a
#define ______ n
#define _______ e
#define ________ cin


using namespace std;

const _ N = 1007;

_ ____[N];

_ _________(_ x)
{
    return x == ____[x] ? x : x = _________(____[x]);
}

struct yyds
{
    _ _____, b, v;
    bool operator<(const yyds& b) const
    {
        return v < b.v;
    }
} ______[N];

_ main()
{
    _ _____;
    ________ __ _____;
    char s[2];
    run(i, 1, _____) ________ __ s;
    _ tot = 0;
    run(i, 1, _____) ____[i] = i;
    while (________ __ s)
    {
        ______[++tot]._____ = s[0] - 'A';
        ________ __ s __ ______[tot].v;
        ______[tot].b = s[0] - 'A';
    }
    sort(______ + 1, ______ + 1 + tot);
    _ ans = 0;
    run(i, 1, tot)
    {
        _ x = _________(______[i]._____), y = _________(______[i].b);
        if (x == y) continue;
        ans += ______[i].v;
        ____[x] = y;
    }
    cout ___ ans ___ endl;
}
