#pragma once
#include <unordered_map>
#include <list>
#include <iostream>

using namespace std;

class Vertex {
    private:
        enum state {undiscovered=0, discovered, processed};
        list <pair <int, size_t>>    vertex_list;
        state                        vertex_state;
        size_t                       degree;
    public:
        Vertex () :
            degree(0), vertex_state(undiscovered) {}

        Vertex (int v2, size_t weight) :
            degree(1), vertex_state(undiscovered)
        {
            vertex_list.push_back(make_pair(v2, weight));
        }

        virtual ~Vertex () {}

        void add_neghibor (int v2, size_t weight)
        {
            vertex_list.push_back(make_pair(v2, weight));
            degree++;
        }

        bool find_vertex (int v2) const
        {
            for (auto it = vertex_list.begin();
                      it != vertex_list.end();
                      ++it) {
                if ((*it).first == v2) {
                    return true;
                }
            }
            return false;
        }

        void display_vertex_list () const
        {
            for (auto it = vertex_list.begin();
                      it != vertex_list.end(); ++it) {
                cout << (*it).second << "---->" << (*it).first
                     << "----";
            }
            cout << "|";
        }
};

class Graph {
    private:
        unordered_map<int, Vertex>       adj_list;
        size_t                           nedges;
        size_t                           nvertices;
        bool                             directed;

    public:
        Graph(bool dir) :
            nedges(0), nvertices(0), directed(dir) {}
        virtual ~Graph() {}

        void add_edge (const int v1, const int v2,
                       const size_t w)
        {
            __add_edge(v1, v2, w, directed);
        }


        void display_graph ()
        {
            for (const auto& v: adj_list) {
                cout << v.first << "----";
                v.second.display_vertex_list();
                cout << endl;
            }
        }


    private:
        void __add_edge(const int v1, const int v2,
                        const size_t w, bool dir)
        {
            if (!adj_list.count(v1)) {
                adj_list[v1] = Vertex(v2, w);
                nvertices++;
            } else {
                adj_list[v1].add_neghibor(v2, w);
            }

            if (!dir) {
                __add_edge(v2, v1, w, true);
            } else {
                nedges++;
            }
        }
};
