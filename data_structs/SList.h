/*
 * SLlist.h
 *
 *  Created on: Mar 2, 2018
 *      Author: sanket
 */

#ifndef SLLIST_H_
#define SLLIST_H_
#include "Llist.h"

/* Implementation of  a templated unsorted singly linked list by inheriting from the unsorted
 * singly linked list */

template <typename T>
class SLlist : public Llist<T>
{
public:
    SLlist() {}
    virtual ~SLlist() {}

    /* @descr:  Overrides base class insert fucntion. Find a node which is just greater than data
     *          and insert new element before that
     * @state:  Updates head if data is lesser than current minimum of the list, updates tail if
     *          data is greater than current maximum of the list
     * @params: data - Element to be inserted into the list
     * @return: None
     */
    virtual
    void insert_to_list(T data)
    {
        Node<T>* list = this->get_head();
        Node<T>* new_node(new Node<T>(data));

        if (!list || list->val > data) {
                /* Insertion at the head */
                new_node->next = list;
                this->set_head(new_node);
        } else {
            while (list && list->next) {
                if (list->next->val > data) {
                    break;
                }
                list = list->next;
            }
            new_node->next = list->next;
            list->next = new_node;
        }

        if (!new_node->next) {
            this->set_tail(new_node);
        }
    }

    /* @descr:  Member function to find the node with minimum value in the list. In a sorted this
     *          is always the head of the list
     * @state:  None
     * @params: None
     * @return: min_node - The node whose value is minimum in the list. This will be null if the
     *          list is empty
     */
    virtual
    Node<T>* find_min_in_list (void)
    {
        return this->get_head();
    }

    /* @descr:  Member function to find the node with maximum value in the list. In a sorted this
     *          is always the tail of the list
     * @state:  None
     * @params: None
     * @return: max_node - The node whose value is maximum in the list. This will be null if the
     *          list is empty
     */
    virtual
    Node<T>* find_max_in_list (void)
    {
        return this->get_tail();
    }
};

#endif /* SLLIST_H_ */

