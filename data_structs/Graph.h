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
    enum state {undiscovered=0, discovered, processed};
    struct Vertex {
        list <pair <int, size_t>>    adj_list;
        state                        vertex_state;
        size_t                       degree;
        int                          parent;
        int                          dfs_entry;
        int                          dfs_exit;

        Vertex () :
            degree(0), vertex_state(undiscovered),
            parent(INT_MAX),
            dfs_entry(0), dfs_exit(0)
        {}

        Vertex (int v2, size_t weight) :
            Vertex()
        {
            degree++;
            adj_list.push_back(make_pair(v2, weight));
        }

        virtual ~Vertex () {}

        void display_adj_list () const
        {
            for (auto it = adj_list.begin();
                    it != adj_list.end(); ++it) {
                cout << (*it).second << "---->" << (*it).first
                    << "----";
            }
            cout << "|";
        }

    };

    unordered_map<int, Vertex>       gmap;
    size_t                           nedges;
    size_t                           nvertices;
    bool                             directed;
    size_t                           dfs_time;
    pv_t                             pv_early;
    pv_t                             pv_late;
    pe_t                             pedge;

public:
    Graph(bool dir) :
        nedges(0), nvertices(0),
        directed(dir), dfs_time(0),
        pv_early([](int v) {}),
        pv_late([](int v) {}),
        pedge([](int u, int v){})
    {}
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

    void set_process_functions (pv_t f1, pv_t f2, pe_t f3)
    {
        pv_early = f1,
        pv_late  = f2;
        pedge    = f3;
    }

    void bfs (int key)
    {

        if (gmap.find(key) == gmap.end())
            return;

        __init_graph();
        __bfs(key, pv_early, pv_late, pedge);
    }

    void dfs (int key)
    {
        if (gmap.find(key) == gmap.end())
            return;

        __init_graph();
        __dfs(key, pv_early, pv_late, pedge);
    }

    void find_path (int u, int v)
    {
        __init_graph();
        __bfs(u, [](int u){}, [](int u){},
            [](int u, int v){});
        cout << "Path";
        __find_path(u, v);
        cout << endl;
    }

    int connected_components()
    {
        int c = 0;

        __init_graph();
        for (auto &x: gmap) {
            if (x.second.vertex_state == undiscovered) {
                ++c;
                __bfs(x.first, [](int u){}, [](int u){},
                      [](int u, int v){});
            }
        }
        return c;
    }

private:
    void __init_graph()
    {
        for (auto &x: gmap) {
            x.second.parent = INT_MAX;
            x.second.vertex_state = undiscovered;
            x.second.dfs_entry = 0;
            x.second.dfs_exit = 0;
            dfs_time = 0;
        }
    }

    void __add_edge(const int v1, const int v2,
            const size_t w, bool dir)
    {
        if (!gmap.count(v1)) {
            gmap[v1] = Vertex(v2, w);
            nvertices++;
        } else {
            gmap[v1].adj_list.push_back(make_pair(v2, w));
            gmap[v1].degree++;
        }

        if (!dir) {
            __add_edge(v2, v1, w, true);
        } else {
            nedges++;
        }
    }

    void __bfs (int key, pv_t process_vertex_early,
                pv_t process_vertex_late,
                pe_t process_edge)
    {
        queue<int> q;
        q.push(key);
        gmap[key].vertex_state = discovered;
        gmap[key].parent = INT_MAX; // assuming INT_MAX never used in graph
        while(!q.empty()) {
            int u = q.front();
            process_vertex_early(u);
            for (auto it = gmap[u].adj_list.begin();
                    it != gmap[u].adj_list.end(); ++it) {
                int v = (*it).first;
                if (gmap[v].vertex_state == undiscovered) {
                    gmap[v].vertex_state = discovered;
                    gmap[v].parent = u;
                    process_edge(u, v);
                    q.push(v);
                }
            }
            gmap[u].vertex_state = processed;
            process_vertex_late(u);
            q.pop();
        }
    }
    void __dfs (int u, pv_t process_vertex_early,
                pv_t process_vertex_late,
                pe_t process_edge)
    {
        gmap[u].vertex_state = discovered;
        gmap[u].dfs_entry = dfs_time++;
        process_vertex_early(u);
        for (auto it = gmap[u].adj_list.begin();
             it != gmap[u].adj_list.end(); ++it) {
            int v = (*it).first;
            process_edge(u, v);
            if (gmap[v].vertex_state == undiscovered) {
                gmap[v].parent = u;
                __dfs(v, process_vertex_early,
                      process_vertex_late, process_edge);
            }
        }
        gmap[u].dfs_exit = dfs_time++;
        gmap[u].vertex_state = processed;
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
