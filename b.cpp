#include<bits/stdc++.h>

#define run(i, a, b) for(int i = a; i <= b; i ++)

using namespace std;

const int N = 1007;

vector<int> e[N];

int n, beg;

char s[2], d[2];

bool vis[N];

void add(int x, int y)
{
    e[x].push_back(y), e[y].push_back(x);
}

void bfs(int f)
{
    queue<int> q;
    q.push(f);
    vis[f] = 1;
    int cnt = 0;
    while(!q.empty())
    {
        f = q.front();
        q.pop();
        cnt ++;
        printf("%c%c", f + 'A', " \n"[cnt == n]); // 这里真是人麻了，题目也没说啊，没有行末空格，大意吃pe
        for(auto x : e[f]) if(!vis[x]) {q.push(x), vis[x] = 1;}
    }
}

int main()
{
    cin >> n;
    run(i, 1, n)
    {
        cin >> s;
        if(i == 1) beg = s[0] - 'A'; // 众多无用数据中的唯一有用
    }
    while(cin >> s)
    {
        cin >> d;
        add(s[0] - 'A', d[0] - 'A');
    }
    run(i, 0, 30) sort(e[i].begin(), e[i].end()); // 有没有一种可能，题目没有要求唯一的标准输出，哈哈哈，反正我是没敢试
    bfs(beg);
}