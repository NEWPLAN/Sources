#ifndef TWO_THREE_FOUR_NODE_H_
#define TWO_THREE_FOUR_NODE_H_

#include <cstddef>
#include <cstring>

class TwoThreeFourNode {
public:
    TwoThreeFourNode():key_cnt(0), parent(NULL) { 
        memset(child, NULL, sizeof(child));
    };    
   ~TwoThreeFourNode() {
       for (int i = 0; i < MAX_CHILD_CNT; ++i) {
           delete child[i];
       }
   };
    
    void insert(int key);
    
private:
    enum {
        MAX_DATA_CNT = 3,
        MAX_CHILD_CNT,
    };
    bool need_split(void) const { return key_cnt == MAX_DATA_CNT;};
    void split_node(void);
    bool is_root_node(void) const { return parent == NULL;};
    bool is_leaf_node(void) const {
        for (int i = 0; i < MAX_CHILD_CNT; ++i) {
            if (child[i]) {
                return false;
            }
        }
        return true;
    }
    int get_child_node_index(const TwoThreeFourNode *child_node);
    void add_key(int new_key);
    void add_child(TwoThreeFourNode *child_node, int index);
    void add_key_to_child(int new_key);

    int key[MAX_DATA_CNT];
    int key_cnt;
    TwoThreeFourNode *child[MAX_CHILD_CNT];
    TwoThreeFourNode *parent;
};

#endif