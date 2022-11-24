#include<bits/stdc++.h>

#define run(i, a, b) for(int i = a; i <= b; i ++)

using namespace std;

const int N = 30, M = 12345678;

int n, e[N][N];

pair<int, int> lyz[N];

char s[2], d[2];

int main()
{
    run(i, 0, 29) run(j, 0, 29) e[i][j] = M;
    cin >> n;
    run(i, 1, n) cin >> s;
    while(cin >> s)
    {
        int t;
        cin >> d >> t;
        int a = s[0] - 'A', b = d[0] - 'A';
        e[a][b] = e[b][a] = t;
    }
    run(k, 0, 29) run(j, 0, 29) run(i, 0, 29) e[j][i] = e[i][j] = min(e[i][j], e[i][k] + e[k][j]);
    run(i, 0, 29) lyz[i] = {M, i};
    run(i, 0, 29)
    {
        int ma = 0;
        run(j, 0, 29) if(e[i][j] != M) ma = max(ma, e[i][j]);
        lyz[i].first = ma ? ma : lyz[i].first;
    }
    sort(lyz, lyz + 29);
    printf("%c\n", lyz[0].second + 'A');
}
