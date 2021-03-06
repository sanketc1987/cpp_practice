#pragma once
#include <unordered_map>
#include <list>
#include <iostream>
#include <queue>

using namespace std;


class Graph {

private:
    typedef function<void (int)> pv_t;
    typedef function<void (int, int)> pe_t;
    enum state_t {undiscovered=0, discovered, processed};
    enum edge_t  {unclassified=0, tree, forward, cross, back};

    struct Edge {
        size_t     weight;
        edge_t     edge_class;

        Edge() :
            weight(0), edge_class(unclassified) {}
    };

    struct Vertex {
        unordered_map<int, Edge>    adj_list;
        state_t                     state;
        size_t                      degree;
        int                         parent;
        int                         dfs_entry;
        int                         dfs_exit;
        int                         dfs_low;
        int                         dfs_treeout;

        Vertex () :
            degree(0), state(undiscovered),
            parent(INT_MAX),
            dfs_entry(0), dfs_exit(0)
        {}

        virtual ~Vertex () {}

        void add_neghibor (int v, size_t weight)
        {
            adj_list[v].weight = weight;
            degree++;
        }

        void classify_edge (int v, edge_t e)
        {
            adj_list[v].edge_class = e;
        }

        edge_t get_edge_class (int v)
        {
            return adj_list[v].edge_class;
        }

        void display_adj_list () const
        {
            for (auto x : adj_list) {
                cout << x.first
                    << "---->";
            }
            cout << "|";
        }
    };

    unordered_map<int, Vertex>   gmap;
    size_t                       nedges;
    size_t                       nvertices;
    bool                         directed;
    size_t                       dfs_time;

public:
    Graph(bool dir) :
        nedges(0), nvertices(0),
        directed(dir), dfs_time(0) {}

    virtual ~Graph() {}

    void add_edge (const int v1, const int v2,
            const size_t w)
    {
        __add_edge(v1, v2, w, directed);
    }


    void display_graph ()
    {
        cout << "Edges# " << nedges << " Vertices " << nvertices << endl;
        for (const auto& v: gmap) {
            cout << v.first << "----";
            v.second.display_adj_list();
            cout << endl;
        }
    }


    void bfs (int key, pv_t pv_early, pv_t pv_late,
              pe_t pe_early, pe_t pe_late)
    {

        if (gmap.find(key) == gmap.end())
            return;

        __init_graph();
        __bfs(key, pv_early, pv_late, pe_early, pe_late);
    }

    void dfs (int key, pv_t pv_early, pv_t pv_late,
              pe_t pe_early, pe_t pe_late)
    {
        if (gmap.find(key) == gmap.end())
            return;

        __init_graph();
        __dfs(key, pv_early, pv_late, pe_early, pe_late);
    }

    void find_bfs_path (int u, int v)
    {
        if (directed) {
            cout << "Not possible on directed graphs" << endl;
            return;
        }
        __init_graph();
        __bfs(u, [](int u){}, [](int u){},
            [](int u, int v){}, [](int u, int v){});
        cout << "Path";
        __find_path(u, v);
        cout << endl;
    }

    int connected_components()
    {
        int c = 0;

        __init_graph();
        for (auto &x: gmap) {
            if (x.second.state == undiscovered) {
                ++c;
                __bfs(x.first, [](int u){}, [](int u){},
                      [](int u, int v){}, [](int u, int v){});
            }
        }
        return c;
    }

    void find_articulation_points()
    {
        __init_graph();

        for (auto &x: gmap) {
            if (x.second.state == undiscovered) {
                __dfs(x.first, [](int u){}, [](int u){},
                      [](int u, int v){}, [](int u, int v){});
            }
        }
        cout << "Articulation point are\t";

        for (auto &x: gmap) {
            if (x.second.parent == INT_MAX) {
                if (x.second.dfs_treeout > 1) {
                    cout << x.first << "\t";
                }
            } else {
                for (auto e : x.second.adj_list) {
                    if (gmap[e.first].dfs_low >= x.second.dfs_entry) {
                        cout << x.first << "\t";
                        break;
                    }
                }
            }
        }
        cout << endl;
    }

private:
    void __init_graph()
    {
        for (auto &x: gmap) {
            x.second.parent = INT_MAX;
            x.second.state = undiscovered;
            x.second.dfs_entry = 0;
            x.second.dfs_exit = 0;
            x.second.dfs_low = INT_MAX;
            x.second.dfs_treeout = 0;
            dfs_time = 0;
        }
    }

    void __add_edge(const int u, const int v,
                    const size_t w, bool dir)
    {
        gmap[u].add_neghibor(v, w);

        if (!dir) {
            __add_edge(v, u, w, true);
        } else {
            nedges++;
        }
    }

    void __bfs (int key,
                pv_t process_vertex_early,
                pv_t process_vertex_late,
                pe_t process_edge_early,
                pe_t process_edge_late)
    {
        queue<int> q;
        q.push(key);
        gmap[key].state = discovered;
        gmap[key].parent = INT_MAX; // assuming INT_MAX never used in graph
        while(!q.empty()) {
            int u = q.front();
            process_vertex_early(u);
            for (auto edge : gmap[u].adj_list) {
                int v = edge.first;
                process_edge_early(u, v);
                if (gmap[v].state == undiscovered) {
                    gmap[v].state = discovered;
                    gmap[v].parent = u;
                    q.push(v);
                }
                process_edge_late(u, v);
            }
            gmap[u].state = processed;
            process_vertex_late(u);
            q.pop();
        }
    }

    void __name_dfs_edge(int u, int v)
    {
        if (!directed &&
            gmap[v].get_edge_class(u) != unclassified) {
            gmap[u].classify_edge(v, gmap[v].get_edge_class(u));
            return;
        }

        if (gmap[v].state == undiscovered) {
            gmap[u].classify_edge(v, tree);
        } else if (gmap[v].state == discovered) {
            gmap[u].classify_edge(v, back);
        } else {
            if (gmap[u].dfs_entry <
                gmap[v].dfs_entry) {
                gmap[u].classify_edge(v, forward);
            } else {
                gmap[u].classify_edge(v, cross);
            }
        }
    }

    void __dfs (int u,
                pv_t process_vertex_early,
                pv_t process_vertex_late,
                pe_t process_edge_early,
                pe_t process_edge_late)
    {
        gmap[u].state = discovered;
        gmap[u].dfs_entry = ++dfs_time;

        gmap[u].dfs_low  = gmap[u].dfs_entry;
        process_vertex_early(u);
        for (auto edge : gmap[u].adj_list) {
            int v = edge.first;
            process_edge_early(u, v);
            __name_dfs_edge(u, v);

            if (gmap[v].state == undiscovered) {
                gmap[v].parent = u;
                __dfs(v, process_vertex_early,
                      process_vertex_late, process_edge_early,
                      process_edge_late);
                gmap[u].dfs_low = min(gmap[u].dfs_low, gmap[v].dfs_low);
                ++gmap[u].dfs_treeout;
            } else if (gmap[u].get_edge_class(v) == back) {
                gmap[u].dfs_low = min(gmap[u].dfs_low, gmap[v].dfs_entry);
            }
            process_edge_late(u, v);
        }
        gmap[u].dfs_exit = ++dfs_time;
        gmap[u].state = processed;
        process_vertex_late(u);
    }

    void __find_path(int u, int v)
    {
        if (u == v || v == INT_MAX) {
            cout << "\t" << u;
        } else {
            __find_path(u, gmap[v].parent);
            cout << "\t" << v;
        }
    }
};
