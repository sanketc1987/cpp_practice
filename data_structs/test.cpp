#include "Llist.h"
#include "SList.h"
#include "BTree.h"
#include "Heap.h"
#include <list>
#include <functional>

using namespace std;
int
main (int argc, char *argv[])
{

    vector<int> inp = {10, 4, 7, 5, 3, 1, 2, 6, 9, 8};
    Heap<int>  h(inp);

    h.display();

    for (auto &x: inp) {
        x = h.get_min();
        h.pop();
    }
    cout << endl;

    for (int &x: inp) {
        cout << x << "\t";
    }
    cout << endl;
#if 0
    Llist<int> l;
    SLlist<int> s;
    Node<int>* n;

    l.insert_to_list(5);
    l.insert_to_list(20);
    l.insert_to_list(10);
    l.insert_to_list(15);

    l.display_list();

    l.delete_from_list(5);
    l.display_list();

    n = l.find_max_in_list();

    if (n)
        cout << "Max = " << n->val << endl;

    n = l.find_min_in_list();

    if (n)
        cout << "Min = " << n->val << endl;

    s.insert_to_list(20);
    s.insert_to_list(10);
    s.insert_to_list(15);
    s.insert_to_list(5);
    s.insert_to_list(20);
    s.display_list();
    n = s.find_max_in_list();

    if (n)
        cout << "Max = " << n->val << endl;

    n = s.find_min_in_list();

    if (n)
        cout << "Min = " << n->val << endl;

    s.delete_from_list(20);
    s.display_list();

    s.insert_to_list(20);
    s.display_list();
    BTree<int> t;

    t.insert(10);
    t.insert(3);
    t.insert(4);
    t.insert(2);
    t.insert(8);
    t.insert(1);
    t.insert(7);
    t.insert(6);
    t.insert(5);
    t.insert(9);

    //t.display();
    t.pop(10);
    //t.display();
    t.insert(10);
    t.display();
    t.pop(8);
    t.display();
    t.insert(8);
    t.display();
    t.pop(1);
    t.display();
    t.pop(7);
    t.display();

    cout << "Before balancing" << endl;
    t.display();
    t.balance_tree();
    cout << "After balancing" << endl;
    t.display();

    t.insert(11);
    t.balance_tree();
#endif
}

