#include <bits/stdc++.h>

using namespace std;
#define int long long

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int n, m;
// DSU - data structure for union find.
vector<int> link;
vector<int> sz;
int find(int x) {
    if (link[x] == x) return x;
    return link[x] = find(link[x]);
}
bool same(int x, int y) { return find(x) == find(y); }
void unite(int a, int b) {
    a = find(a);
    b = find(b);
    if (sz[a] < sz[b]) swap(a, b);
    link[b] = a;
    sz[a] += sz[b];
}
// end of DSU

struct edge {
    int a, b, c;  // a -> b cost c
};
bool cmp(edge a, edge b) { return a.c < b.c; }

// kruskal algorithmn
void kruskal(vector<edge> &edges, vector<edge> &T) {
    sort(edges.begin(), edges.end(), cmp);
    for (int i = 0; i < edges.size(); i++) {
        if (!same(edges[i].a, edges[i].b)) {
            T.push_back(edges[i]);
            unite(edges[i].a, edges[i].b);
        }
    }
}
// filters the edges with the dsu.
vector<edge> filter(vector<edge> &edges, vector<edge> &T) {
    vector<edge> filteredEdges;
    for (int i = 0; i < edges.size(); i++) {
        if (!same(edges[i].a, edges[i].b)) {
            filteredEdges.push_back(edges[i]);
        }
    }
    return filteredEdges;
}

int kruskalThreshold(vector<edge> edges) {
    if (edges.size() == 0) return 1;
    int comparativeValue = edges[0].c;
    for (int i = 1; i < edges.size(); i++) {
        if (edges[i].c != comparativeValue) {
            return n * log(n) / 2;
        }
    }
    return edges.size() + 1;
}
vector<edge> vetRand(vector<edge> &vet) {
    int k = vet.size();
    int x = rng() % k;
    vector<edge> ans;
    set<int> used;
    for (int i = 0; i * i < k; i++) {
        while (used.count(x)) {
            x = rng() % k;
        }
        used.insert(x);
        ans.push_back(vet[x]);
    }
    return ans;
}
edge choosePivot(vector<edge> &edges) {
    vector<edge> median = vetRand(edges);
    sort(median.begin(), median.end(), cmp);
    return median[(median.size() - 1) / 2];
}
void filterKruskal(vector<edge> &edges, vector<edge> &T) {
    if (edges.size() <
        kruskalThreshold(edges)) {  // runs normal kruskal in the edge
                                    // size bellow a treshold
        kruskal(edges, T);
    } else {  // separetes the edges in 2 different vectors, one bellow pivot
              // value and one bigger then pivot value, run filter Kruskal on
              // the first and filters the second before running filter Kruskal
              // on it
        edge pivot = choosePivot(edges);
        vector<edge> lowestEdges;
        vector<edge> biggestEdges;
        for (int i = 0; i < edges.size(); i++) {
            if (edges[i].c <= pivot.c) {
                lowestEdges.push_back(edges[i]);
            } else {
                biggestEdges.push_back(edges[i]);
            }
        }
        filterKruskal(lowestEdges, T);
        biggestEdges = filter(biggestEdges, T);
        filterKruskal(biggestEdges, T);
    }
}
signed main() {
    cin.tie(NULL), cout.tie(NULL);
    cin >> n >> m;
    vector<edge> edges(m);
    link.resize(m + 1);
    sz.resize(m + 1);
    for (int i = 1; i <= n; i++) {
        link[i] = i;
        sz[i] = 1;
    }
    for (int i = 0; i < m; i++) {
        edge aux;
        cin >> aux.a >> aux.b >> aux.c;
        edges[i] = aux;
    }
    vector<edge> T;
    filterKruskal(edges, T);
    // kruskal(edges, T);
    set<int> res;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        res.insert(find(i));
    }
    if (res.size() > 1) {
        cout << "IMPOSSIBLE" << endl;
    } else {
        for (int i = 0; i < T.size(); i++) {
            ans += T[i].c;
        }
        cout << ans << endl;
    }
}