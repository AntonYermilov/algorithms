// given a rooted tree
vector<int> g[N];

int w[N], dep[N], par[N];
int pid[N], vid[N], top[N], PID = 0, VID = 0;

// segment tree or any other data structure
SegmentTree tree;

// update all node values on the path from v to u with value c
void update(int v, int u, int c) {
    while (pid[v] != pid[u]) {
        if (dep[top[v]] < dep[top[u]])
            std::swap(v, u);
        tree.update(vid[top[v]], vid[v] + 1, c);
        v = par[top[v]];
    }
    tree.update(std::min(vid[v], vid[u]), std::max(vid[v], vid[u]) + 1, c);
}

// decompose tree
void hld_init(int v, int p) {
    vid[v] = VID++;
    pid[v] = top[v] == v ? PID++ : pid[p];

    bool first = true;
    for (int u : g[v]) {
        top[u] = first ? top[v] : u;
        hld_init(u, v);
        first = false;
    }
}

// precalc w[v], dep[v] and par[v]
void dfs(int v, int p) {
    w[v] = 1;
    for (int u : g[v]) {
        if (u != p) {
            dep[u] = dep[v] + 1, par[u] = v;
            dfs(u, v);
            w[v] += w[u];
        }
    }
}

// initialize hld
void hld_init(int n) {
    dfs(0, 0);
    for (int v = 0; v != n; ++v) {
        sort(g[v].begin(), g[v].end(), [&v](int a, int b) { return w[a] > w[b]; });
        if (v) g[v].erase(g[v].begin());
    }
    hld_init(0, 0);
}

