/////////////////////////////////////////////////////////////////
//Suffix Tree and Suffix Array with Ukkonen Algorithm.
//Store child nodes in array.
/////////////////////////////////////////////////////////////////
#include <cstring>//strlen, memset.
#include <list>//used by suffix tree node.
#include <vector>//used by suffix tree.using namespace std;struct Suffix { const char* str;  int from; };const int ALPHABET_SIZE = 26 * 2 + 1;struct InNode;struct SfxNode //Suffix tree node
{   const char *l, *r;//[l, r): edge label from parent to this node.
    InNode* prnt;//parent
    virtual bool isLeaf() = 0; };struct InNode: public SfxNode {//Internal node (non-leaf node)
    //for character X and string A, if this node's label is XA,
    //then the suffix link is the node with label A, if any.
    InNode* sfxLink;//suffix link
    SfxNode* ch[ALPHABET_SIZE];//children
    SfxNode*& child(char c)
        { if ('\0' == c) { return ch[0]; }
          return c < 'a'? ch[c - 'A' + 1]: ch[c - 'a' + 27]; }
    bool isLeaf() { return false; }
};struct Leaf: public SfxNode//Suffix tree leaf
{   list<int> from;//which string does this suffix belong to.
    bool isLeaf() { return true; } };InNode g_internal[200000 + 100]; //Ask for memory once and allocate
Leaf g_leaf[200000 + 100];       //them my self, to make the
int g_inCnt = 0, g_leafCnt = 0;  //tree destruction fast.InNode* newInNode(const char* l = NULL, const char* r = NULL)
{   InNode* p = &g_internal[g_inCnt++];
    p->l = l;  p->r = r;  p->sfxLink = p->prnt = NULL;
    memset( p->ch, 0, sizeof(p->ch) );
    return p; }Leaf* newLeaf(const char* l = NULL, const char* r = NULL)
{   Leaf* p = &g_leaf[g_leafCnt++];
    p->l = l;  p->r = r;  p->from.clear();
    return p; }list<int> g_stack;//A stack for the DFS of the tree.class SuffixTree {
public:
    SuffixTree(): m_root( newInNode() ), m_texts(), m_lens() {}
    ~SuffixTree() { clear(); }    //Don't free the space of the added string
    //before the last string is added.
    void addText(const char* text) {
        m_text = m_i = text;  m_leafCnt = 0;  m_p = m_root;
        m_root->l = m_root->r = m_text;
        m_len = strlen(text);
        for (int i = 0; i <= m_len; i++) {
            m_newIn = NULL;
            for (int j = m_leafCnt; j <= i; j++)
                { if ( !extend(m_text + j, m_text + i) ) { break; } }
        }
        m_texts.push_back(m_text);  m_lens.push_back(m_len);
    }    void clear() { g_inCnt = g_leafCnt = 0;  m_root = newInNode();
                   m_texts.clear();  m_lens.clear(); }    //Write the two arrays to construct a suffix array:
    //sfx: the suffixes in lexigraphical order.
    //lcp[i]: longest common prefix of sfx[i - 1] and sfx[i].
    void toSuffixArray(Suffix* sfx, int* lcp) const {
        Node* p = m_root;
        int i = 0, depth = 0, sfxI = 0, cp = 0;
        g_stack.clear();  g_stack.push_back(0);
        while ( !g_stack.empty() ) {
            if ( p->isLeaf() ) {
                Leaf* leaf = (Leaf*)p;
                if (depth > 1) {
                    for (list<int>::iterator it = leaf->from.begin();
                         leaf->from.end() != it;  it++)
                    {   sfx[sfxI].from = *it;
                        sfx[sfxI].str = m_texts[*it]+m_lens[*it]-depth+1;
                        lcp[sfxI++] = cp;  cp = depth - 1; }
                }
                i = g_stack.back();  i++;  g_stack.pop_back();
                depth -= p->r - p->l;  p = p->prnt;  cp = depth;
            }
            else {
                InNode* in = (InNode*)p;
                while ( i < ALPHABET_SIZE && !in->ch[i] ) { i++; }
                if (ALPHABET_SIZE == i)//All children are visited.
                {   i = g_stack.back();  i++;  g_stack.pop_back();
                    depth -= p->r - p->l;  p = p->prnt;  cp = depth; }
                else { p = in->ch[i];  depth += p->r - p->l;
                       g_stack.push_back(i);  i = 0; }
            }
        }
    }private:
    typedef SfxNode Node;
   
    //Go along string m_text[l, r) starting from node p.
    void goStr(const char* l, const char* r) {
        m_i = m_p->r;
        while (l < r)
        {   m_p = ( (InNode*)m_p )->child(*l);//There must be a child.
            if (r-l <= m_p->r - m_p->l) { m_i = m_p->l + (r-l);  l=r; }
            else { m_i = m_p->r;  l += m_p->r - m_p->l; } }
    }    //Return true if new leaf is added.
    bool extend(const char* i, const char* r) {
        if (m_i < m_p->r) {
            const char* l;
            if (*m_i == *r) {//implicit extension, no new leaf added.
                if (*r) { m_i++;  return false; }
                ( (Leaf*)m_p )->from.push_back( m_texts.size() );
                l = r - (m_p->r - m_p->l - 1);
            }
            else {
                //Insert a new internal node and add a new leaf.
                InNode* in = newInNode(m_p->l, m_i);
                m_p->prnt->child(*m_p->l) = in;  in->prnt = m_p->prnt;
                in->child(*m_i) = m_p;  m_p->prnt = in;  m_p->l = m_i;
                Leaf* leaf = newLeaf(r, m_text + m_len + 1);
                in->child(*r) = leaf;  leaf->prnt = in;
                leaf->from.push_back( m_texts.size() );  m_leafCnt++;
                //This new internal node may be suffix link of others.
                if (m_newIn) { m_newIn->sfxLink = in; }
                m_p = m_newIn = in;
                l = r - (m_p->r - m_p->l);
            }
            //Find the position of next extension.
            InNode* p = m_p->prnt;  m_p = p;
            if (p->sfxLink) { m_p = p->sfxLink; } else { l++; }
            goStr(l, r);
        }
        else {//in condition that m_i == m_p->r
            InNode* p = (InNode*)m_p;//now m_p must be internal.
            if (m_newIn) { m_newIn->sfxLink = p;  m_newIn = NULL; }
            Node* ch = p->child(*r);
            if (ch)
            {   if (*r) { m_p = ch;  m_i = m_p->l + 1;  return false; }
                ( (Leaf*)ch )->from.push_back( m_texts.size() ); }
            else { Leaf* leaf = newLeaf(r, m_text + m_len + 1);
                   p->child(*r) = leaf;  leaf->prnt = p;  m_leafCnt++;
                   leaf->from.push_back( m_texts.size() ); }
            if (i < r) { m_p = p->sfxLink;  goStr(NULL, NULL); }
        }
        return true;
    }    InNode* m_root;
    vector<const char*> m_texts;  vector<int> m_lens;
    //the following members are to help the extensions.
    Node* m_p;  InNode* m_newIn;
    const char *m_i, *m_text;
    int m_leafCnt, m_len;
};//Test suite and usage example
#include <iostream>
int main() {
    Suffix sa[100];  int lcp[100];
    char a[] = "xabxa", b[] = "babxba";
    SuffixTree t;  t.addText(a);  t.addText(b);
    t.toSuffixArray(sa, lcp);
    int cnt = strlen(a) + strlen(b);
    for (int i = 0; i < cnt; i++)
    {   cout << sa[i].from <<" "<< sa[i].str <<" "<<lcp[i] << endl; }
    return 0; //output: 0 a 0
              //        1 a 1
              //        0 abxa 1
              //        1 abxba 3
              //        1 ba 0
              //        1 babxba 2
              //        0 bxa 1
              //        1 bxba 2
              //        0 xa 0
              //        0 xabxa 2
              //        1 xba 1
}