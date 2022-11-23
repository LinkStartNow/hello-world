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
        printf("%c%c", f + 'A', " \n"[cnt == n]); // �������������ˣ���ĿҲû˵����û����ĩ�ո񣬴����pe
        for(auto x : e[f]) if(!vis[x]) {q.push(x), vis[x] = 1;}
    }
}

int main()
{
    cin >> n;
    run(i, 1, n)
    {
        cin >> s;
        if(i == 1) beg = s[0] - 'A'; // �ڶ����������е�Ψһ����
    }
    while(cin >> s)
    {
        cin >> d;
        add(s[0] - 'A', d[0] - 'A');
    }
    run(i, 0, 30) sort(e[i].begin(), e[i].end()); // ��û��һ�ֿ��ܣ���Ŀû��Ҫ��Ψһ�ı�׼���������������������û����
    bfs(beg);
}