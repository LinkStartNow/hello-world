#include<bits/stdc++.h>

#define run(i, a, b) for(int i = a; i <= b; i ++)

using namespace std;

const int N = 3007;

int n, beg, ed, dis[N];

bool isin[N];

struct lyz
{
    int to, nx, v;
} e[N];

int head[N], tot; // 链式前向星套装,什么？为啥不用邻接表？因为我自己的就是哈哈哈

void add(int u, int v, int w);

/* 我的评价是：都什么花里胡哨的，dfs yyds！*/
void spfa();

int main()
{
    cin >> n;
    char s[2], d[2];
    run(i, 1, n)
    {
        cin >> s;
        if(i == 1) beg = s[0] - 'A';
        if(i == n) ed = s[0] - 'A'; // 没办法，开头结尾躲不开，我也不想读啊
    }
    while(cin >> s)
    {
        int t;
        cin >> d >> t;
        int a = s[0] - 'A', b = d[0] - 'A';
        add(a, b, t);
    }
    run(i, 0, 1000) dis[i] = -66666;
    spfa();
    cout << dis[ed] << endl;
}

/* 我的评价是：都什么花里胡哨的，dfs yyds！*/
void spfa()
{
    dis[beg] = 0;
    priority_queue<pair<int, int>> q;
    q.push({0, beg});
    isin[beg] = 1;
    while(!q.empty())
    {
        int s = q.top().second;
        q.pop();
        isin[s] = 0;
        for(int i = head[s]; i; i = e[i].nx)
        {
            int to = e[i].to, v = e[i].v;
            if(dis[to] < dis[s] + v)
            {
                dis[to] = dis[s] + v;
                if(!isin[to]) q.push({dis[to], to}), isin[to] = 1;
            }
        }
    }
}

void add(int u, int v, int w)
{
    e[++ tot] = {v, head[u], w};
    head[u] = tot;
}