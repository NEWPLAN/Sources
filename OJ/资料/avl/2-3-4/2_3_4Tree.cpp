#include <cassert>
#include "two_three_four_node.h"

void TwoThreeFourNode::insert(int new_key)
{
    /* check itself */
    if(need_split()) {
        split_node();
        /* Because split this node, so need recheck the parent node */
        if (parent) {
            parent->insert(new_key);
            return;
        }
    }

    if (is_leaf_node()) {
        add_key(new_key);
    }
    else {
        add_key_to_child(new_key);
    }

    return; 
}

void TwoThreeFourNode::add_key(int new_key)
{
    int index = 0;
    while (index < key_cnt && new_key >= key[index]) {
        ++index;
    }
    if (index < key_cnt) {
        memmove(key+index+1, key+index, (key_cnt-index)*sizeof(int));
    }
    key[index] = new_key;
    ++key_cnt;
}

void TwoThreeFourNode::add_child(TwoThreeFourNode * child_node, int index)
{
    child[index] = child_node;
    if (child_node) {
        child_node->parent = this;
    }
}

void TwoThreeFourNode::add_key_to_child(int new_key)
{
    for (int i = 0; i < key_cnt; ++i) {
        if (new_key <= key[i]) {
            if (NULL == child[i]) {
                TwoThreeFourNode *new_child = new TwoThreeFourNode();
                new_child->add_key(new_key);
                add_child(new_child, i);
                return;
            }
            else {
                child[i]->insert(new_key);
                return;
            }
        }
    }

     //这块代码有些冗余，应该可以和上面的代码合并。单独去判断最右侧的子节点，显得有些奇怪。
     //不过由于这只是一个练手代码，我也就接受了。
    /* check the rightmost child */
    if (NULL == child[key_cnt]) {
        TwoThreeFourNode *new_child = new TwoThreeFourNode();
        new_child->add_key(new_key);
        add_child(new_child, key_cnt);
    }
    else {
        child[key_cnt]->insert(new_key);
    }
}

int TwoThreeFourNode::get_child_node_index(const TwoThreeFourNode * child_node)
{
    for (int i = 0; i < key_cnt+1; ++i) {
        if (child_node == child[i]) {
            return i;
        }
    }

    return -1;
}

 /*
 拆分节点的时候。根节点的拆分方法和普通节点不同。这两部分处理要合并有点困难。
 但是目前这个函数有点长。在真正写代码时，如果真的不能合并处理，我会将其分为两个子函数。
 */
void TwoThreeFourNode::split_node(void)
{
    if (is_root_node()) {
        TwoThreeFourNode *new_left_child = new TwoThreeFourNode();
        TwoThreeFourNode *new_right_child = new TwoThreeFourNode();

        new_left_child->key[0] = key[0];
        new_left_child->key_cnt = 1;
        new_left_child->add_child(child[0], 0);
        new_left_child->add_child(child[1], 1);
        add_child(new_left_child, 0);

        new_right_child->key[0] = key[2];
        new_right_child->key_cnt = 1;
        new_right_child->add_child(child[2], 0);
        new_right_child->add_child(child[3], 1);
        add_child(new_right_child, 1);

        key[0] = key[1];
        key_cnt = 1;
        child[2] = NULL;
        child[3] = NULL;
    }
    else {
        TwoThreeFourNode *new_node = new TwoThreeFourNode();

        new_node->key[0] = key[0];
        new_node->key_cnt = 1;
        new_node->add_child(child[0], 0);
        new_node->add_child(child[1], 1);

        int new_index = parent->get_child_node_index(this);
        assert(new_index != -1);
        memmove(parent->child+new_index+1, parent->child+new_index, (parent->key_cnt-new_index+1)*sizeof(TwoThreeFourNode *));
        if (new_index < parent->key_cnt) {
            memmove(parent->key+new_index+1, parent->key+new_index, (parent->key_cnt-new_index)*sizeof(int));
        }
        parent->key[new_index] = key[1];
        ++parent->key_cnt;
        parent->add_child(new_node, new_index);


        key[0] = key[2];
        key_cnt = 1;
        add_child(child[2], 0);
        add_child(child[3], 0);
        child[2] = NULL;
        child[3] = NULL;
    }
}