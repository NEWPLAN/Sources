#include <stdio.h>
#include <stdlib.h>

/**
 * @brief the degree of btree
 * key per node: [M-1, 2M-1]
 * child per node: [M, 2M]
 */
#define M 3
/*这里key和pointer的最大个数都比要求的大1，主要是方便在插入时需要先插入再分裂 */
typedef struct btree_node
{
    int k[2*M];
    struct btree_node *p[2*M+1]; // one more for recursive
    int num;
    bool is_leaf;
} btree_node;

/**
 * @brief allocate a new btree node
 * default: is_leaf == true
 * 创建节点接口：
 * @return pointer of new node
 */
btree_node *btree_node_new();


/**
 * @brief create a btree root
 * 创建节点接口：
 * @return pointer of btree root
 */
btree_node *btree_create();


/** 插入节点
 * @brief split child if num of key in child reach 2 * M
 * left M -1 keys, right M keys
 * 
 * @param parent: parent of child
 * @param pos: p[pos] points to child
 * @param child: the node to be splited
 *
 * @return
 */
 // 执行该操作时，说明child上已经有2*M个节点了，已经违反了规则
// 保留左边的M-1个节点，把第M个节点上升到parent中，右边M个节点放入新节点中
int btree_split_child(btree_node *parent, int pos, btree_node *child);


/**插入节点
 * @brief insert a value into btree
 * the num of key in node less than 2M-1
 *
 * @param node: tree root
 * @param target: target to insert
 */
  // node一定是叶子节点，在pos处插入target
void btree_insert_nonfull(btree_node *node, int pos, int target);


/**插入节点
 * @brief insert a value into btree

 *
 * @param root: tree root
 * @param target: target to insert
 *
 * @return: new root of tree
 */
 // 考虑特殊情况，如果最后根节点有2*M个节点，将根进行分裂，产生新的根，树增高一层
btree_node* btree_insert(btree_node *root, int target);


/**删除节点
 * @brief merge y, z and root->k[pos] to left
 * y or z has less than M-1 keys
 * // 执行该操作时，一定是y或者z的key个数少于M-1，将y，root->k[pos]，z合并到y，释放z
 * @param root: parent node
 * @param pos: postion of y
 * @param y: left node to merge
 * @param z: right node to merge
 */
void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z);


/**
 * @brief delete target recursively
 *
 * @param node
 * @param target
 */
 // 递归删除target，在回溯的过程中，如果遇到叶子节点，则将target删除
 // 如果遇到分支节点，则检查子树的节点数是否少于M-1，如果是，则需要从左或
 // 右兄弟借节点，如果左右兄弟都只有M-1个节点，则要执行merge
void btree_delete_recursive(btree_node *node, int target);

/**
 * @brief delete a vlue from btree
 *
 * @param root: btree root
 * @param target: target to delete
 *
 * @return: new root of tree
 */
 // 考虑特殊情况，删除完后，根节点无任何key了，这时释放根，将子树作为新的根
btree_node *btree_delete(btree_node *root, int target);

/**
 * @brief delete a vlue from btree
 * root has at least M keys
 *
 * @param root: btree root
 * @param target: target to delete
 *
 * @return
 */
 // 如果node的第pos个key为target，则删除之，否则说明树中没有待删除的节点
void btree_delete_nonone(btree_node *root, int pos, int target);


/**
 * @brief find the rightmost value
 *
 * @param root: root of tree
 *
 * @return: the rightmost value
 */
  // find rightmost key
int btree_search_predecessor(btree_node *root);


/**
 * @brief find the leftmost value
 *
 * @param root: root of tree
 *
 * @return: the leftmost value
 */
  // find leftmost key 
int btree_search_successor(btree_node *root);


/**
 * @brief shift a value from z to y
 *
 * @param root: btree root
 * @param pos: position of y
 * @param y: left node
 * @param z: right node
 */
   // 从右兄弟借一个节点，z的最小key上升至root，root的看k[pos]下降至y
void btree_shift_to_left_child(btree_node *root, int pos, btree_node *y, btree_node *z);

/**
 * @brief shift a value from z to y
 *
 * @param root: btree root
 * @param pos: position of y
 * @param y: left node
 * @param z: right node
 */
  // 从左兄弟借一个节点，y的最大key上升至root，root的k[pos]下降至z
void btree_shift_to_right_child(btree_node *root, int pos, btree_node *y, btree_node *z);


/**
 * @brief inorder traverse the btree
 *
 * @param root: root of treee
 */
void btree_inorder_print(btree_node *root);


/**
 * @brief level print the btree
 *
 * @param root: root of tree
 */
void btree_level_display(btree_node *root);

btree_node *btree_node_new()
{
    btree_node *node = (btree_node *)malloc(sizeof(btree_node));
    if(NULL == node)
    {
        return NULL;
    }

    for(int i = 0; i < 2 * M -1; i++)
    {
        node->k[i] = 0;
    }

    for(int i = 0; i < 2 * M; i++)
    {
        node->p[i] = NULL;
    }

    node->num = 0;
    node->is_leaf = true;
}

btree_node *btree_create()
{
    btree_node *node = btree_node_new();
    if(NULL == node)
    {
        return NULL;
    }

    return node;
}


int btree_split_child(btree_node *parent, int pos, btree_node *child)
{
    btree_node *new_child = btree_node_new();
    if(NULL == new_child)
    {
        return -1;
    }

    new_child->is_leaf = child->is_leaf;
    new_child->num = M; // M-1 left, M right

    for(int i = 0; i < M; i++)
    {
        new_child->k[i] = child->k[i+M];
    }

    if(false == new_child->is_leaf)
    {
        for(int i = 0; i < M + 1; i++)
        {
            new_child->p[i] = child->p[i+M];
        }
    }

    child->num = M - 1;

    for(int i = parent->num; i > pos; i--)
    {
        parent->p[i+1] = parent->p[i];
    }
    parent->p[pos+1] = new_child;

    for(int i = parent->num - 1; i >= pos; i--)
    {
        parent->k[i+1] = parent->k[i];
    }
    parent->k[pos] = child->k[M-1];

    parent->num += 1;

}

// 从根节点开始，一直到叶子节点，然后回溯
// 回溯过程中，如果是叶子节点，则将target插入，如果是分支节点，如果子路径上有包含
// 2*M个key的节点，则将其分裂（分裂时必须知道待分裂节点的父节点）
void btree_insert_recursive(btree_node *node, int target)
{
    if(NULL != node)
    {
        int pos = 0;
        while(pos < node->num && target > node->k[pos]) pos++;
        btree_insert_recursive(node->p[pos], target);
        if(NULL == node->p[pos])
        {
            btree_insert_nonfull(node, pos, target);
        }
        else
        {
            if(2 * M == node->p[pos]->num)
            {
                btree_split_child(node, pos, node->p[pos]);
            }
        }
    }
}

void btree_insert_nonfull(btree_node *node, int pos, int target)
{
    for(int j = node->num; j > pos; j--)
    {
        node->k[j] = node->k[j-1];
    }
    node->k[pos] = target;
    node->num += 1;
}

btree_node* btree_insert(btree_node *root, int target)
{
    if(NULL == root)
    {
        return root;
    }

    btree_insert_recursive(root, target);

    if(2 * M  == root->num)
    {
        btree_node *node = btree_node_new();
        if(NULL == node)
        {
            return root;
        }

        node->is_leaf = false;
        node->p[0] = root;
        btree_split_child(node, 0, root);
        return node;
    }

    return root;
}

void btree_merge_child(btree_node *root, int pos, btree_node *y, btree_node *z)
{
    int n = y->num;
    for(int i = 0; i < z->num; i++)
    {
        y->k[n+1+i] = z->k[i];
    }
    y->k[n] = root->k[pos];

    if(false == z->is_leaf)
    {
        for(int i = 0; i <= z->num; i++)
        {
            y->p[n+1+i] = z->p[i];
        }
    }

    y->num += (z->num + 1);

    for(int j = pos + 1; j < root->num; j++)
    {
        root->k[j-1] = root->k[j];
        root->p[j] = root->p[j+1];
    }

    root->num -= 1;
    free(z);
}

btree_node *btree_delete(btree_node *root, int target)
{
    if(NULL == root)
    {
        return NULL;
    }

    btree_delete_recursive(root, target);

    if(0 == root->num && false == root->is_leaf)
    {
        btree_node *newroot = root->p[0];
        free(root);
        return newroot;
    }

    return root;
}


void btree_delete_recursive(btree_node *node, int target)
{
    if(NULL != node)
    {
        int i = 0;
        while(i < node->num && target > node->k[i]) i++;

        if(i < node->num && target == node->k[i] && false == node->is_leaf)   // found
        {
            btree_node *y = node->p[i];
            int pre = btree_search_predecessor(y);
            node->k[i] = pre;
            target = pre;
        }
        btree_delete_recursive(node->p[i], target);

        if(NULL == node->p[i])
        {
            btree_delete_nonone(node, i, target);
        }
        else
        {
            btree_node *y = node->p[i];
            if(M - 2 == y->num)
            {
                btree_node *p = NULL, *z = NULL;
                if(i > 0)
                {
                    p = node->p[i-1];
                }
                if(i < node->num)
                {
                    z = node->p[i+1];
                }

                if(i > 0 && p->num > M - 1)
                {
                    btree_shift_to_right_child(node, i-1, p, y);
                }
                else if(i < node->num && z->num > M - 1)
                {
                    btree_shift_to_left_child(node, i, y, z);
                }
                else if(i > 0)
                {
                    btree_merge_child(node, i-1, p, y); // note
                    y = p;
                }
                else
                {
                    btree_merge_child(node, i, y, z);
                }
            }
        }
    }
}

void btree_delete_nonone(btree_node *node, int pos, int target)
{
    if(node->k[pos] != target)
    {
        printf("target not found\n");
    }
    else
    {
        for(int j = pos; j < node->num - 1; j++)
        {
            node->k[j] = node->k[j+1];
        }

        (void)target;
        node->num -= 1;
    }
}

int btree_search_predecessor(btree_node *root)
{
    btree_node *y = root;
    while(false == y->is_leaf)
    {
        y = y->p[y->num];
    }
    return y->k[y->num-1];
}

int btree_search_successor(btree_node *root)
{
    btree_node *z = root;
    while(false == z->is_leaf)
    {
        z = z->p[0];
    }
    return z->k[0];
}


void btree_shift_to_right_child(btree_node *root, int pos,
                                btree_node *y, btree_node *z)
{
    z->num += 1;
    for(int i = z->num -1; i > 0; i--)
    {
        z->k[i] = z->k[i-1];
    }
    z->k[0]= root->k[pos];
    root->k[pos] = y->k[y->num-1];

    if(false == z->is_leaf)
    {
        for(int i = z->num; i > 0; i--)
        {
            z->p[i] = z->p[i-1];
        }
        z->p[0] = y->p[y->num];
    }

    y->num -= 1;
}

void btree_shift_to_left_child(btree_node *root, int pos,
                               btree_node *y, btree_node *z)
{
    y->num += 1;
    y->k[y->num-1] = root->k[pos];
    root->k[pos] = z->k[0];

    for(int j = 1; j < z->num; j++)
    {
        z->k[j-1] = z->k[j];
    }

    if(false == z->is_leaf)
    {
        y->p[y->num] = z->p[0];
        for(int j = 1; j <= z->num; j++)
        {
            z->p[j-1] = z->p[j];
        }
    }

    z->num -= 1;
}

void btree_inorder_print(btree_node *root)
{
    if(NULL != root)
    {
        btree_inorder_print(root->p[0]);
        for(int i = 0; i < root->num; i++)
        {
            printf("%d ", root->k[i]);
            btree_inorder_print(root->p[i+1]);
        }
    }
}

void btree_level_display(btree_node *root)
{
    // just for simplicty, can't exceed 200 nodes in the tree
    btree_node *queue[200] = {NULL};
    int front = 0;
    int rear = 0;

    queue[rear++] = root;

    while(front < rear)
    {
        btree_node *node = queue[front++];

        printf("[");
        for(int i = 0; i < node->num; i++)
        {
            printf("%d ", node->k[i]);
        }
        printf("]");

        for(int i = 0; i <= node->num; i++)
        {
            if(NULL != node->p[i])
            {
                queue[rear++] = node->p[i];
            }
        }
    }
    printf("\n");
}

int main()
{
    int arr[] = {18, 31, 12, 10, 15, 48, 45, 47, 50, 52, 23, 30, 20};

    btree_node *root = btree_create();

    for(int i = 0; i < sizeof(arr) / sizeof(int); i++)
    {
        root = btree_insert(root, arr[i]);
        btree_level_display(root);
    }
	printf("\n\tdelele now....\n");
    int todel[] = {15, 18, 23, 30, 31, 52, 50, 48, 47, 45, 20, 12, 10};
    //int todel[] = {36};
    for(int i = 0; i < sizeof(todel) / sizeof(int); i++)
    {
        root = btree_delete(root, todel[i]);
        btree_level_display(root);
    }

    return 0;
}
