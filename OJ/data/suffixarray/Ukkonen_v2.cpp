/////////////////////////////////////////////////////////////////
//Suffix Tree and Suffix Array with Ukkonen Algorithm.
//Store child nodes in linked list.
/////////////////////////////////////////////////////////////////
#include <cstring>//strlen.
#include <list>//used by suffix tree node.
#include <vector>//used by suffix tree.using namespace std;struct Suffix { const char* str;  int from; };struct SfxNode {
    const char *l, *r;//[l, r): edge label from parent to this node.
    //for character X and string A, if this node's label is XA,
    //then the suffix link is the node with label A, if any.
    SfxNode* sfxLink;//suffix link
    list<int> from;//which string does this suffix belong to.
    SfxNode* prnt;//parent
    SfxNode* ch;//the alphabetically first child
    SfxNode* pre;//the alphabetically previous cousin
    SfxNode* csn;//the alphabetically next cousin    SfxNode* child(char c) const
    {   SfxNode* p = NULL;
        if (ch && *ch->l <= c) { p = ch; } else { return p; }
        while (p->csn && *p->csn->l <= c) { p = p->csn; }
        return p; }    void addChild(SfxNode* p, SfxNode* add) {//add a child after p.
        if (p) { add->csn = p->csn;  add->pre = p;  p->csn = add;
                 if (add->csn) { add->csn->pre = add; } }
        else { add->pre = NULL;  add->csn = ch;  ch = add;
               if (add->csn) { add->csn->pre = add; } }
        add->prnt = this;
    }
};SfxNode g_sfxNode[400000 + 100]; //Ask for memory once and allocate
int g_sfxNodeCnt = 0;            //them myself, to make the tree
                                 //destruction fast.
SfxNode* newNode(const char* l = NULL, const char* r = NULL)
{   SfxNode* p = &g_sfxNode[g_sfxNodeCnt++];
    p->l = l;  p->r = r;  p->from.clear();
    p->sfxLink = p->prnt = p->ch = p->pre = p->csn = NULL;
    return p; }class SuffixTree {
public:
    SuffixTree(): m_root( newNode() ), m_texts(), m_lens() {}
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
    }    void clear() { g_sfxNodeCnt = 0;  m_root = newNode();
                   m_texts.clear();  m_lens.clear(); }    //Write the two arrays to construct a suffix array:
    //sfx: the suffixes in lexigraphical order.
    //lcp[i]: longest common prefix of sfx[i - 1] and g_sfx[i].
    void toSuffixArray(Suffix* sfx, int* lcp) const {
        Node* p = m_root;
        int depth = 0, sfxI = 0, cp = 0;  bool up = false;
        while (p) {
            if ( !p->from.empty() && depth > 1 ) {//meet a leaf
                for (list<int>::iterator it = p->from.begin();
                     p->from.end() != it;  it++)
                {   sfx[sfxI].from = *it;
                    sfx[sfxI].str = m_texts[*it]+m_lens[*it]-depth+1;
                    lcp[sfxI++] = cp;  cp = depth - 1; }
            }
            if (!up && p->ch) { p=p->ch; depth+=p->r-p->l; continue; }
            depth -= p->r - p->l;  cp = depth;
            if (p->csn) { p=p->csn; depth += p->r-p->l; up=false; }
            else { p = p->prnt;  up = true; }
        }
    }private:
    typedef SfxNode Node;    //Go along string m_text[l, r) starting from node p.
    void goStr(const char* l, const char* r) {
        m_i = m_p->r;
        while (l < r)
        {   m_p = m_p->child(*l);//There must be a child.
            if (r-l <= m_p->r - m_p->l) { m_i = m_p->l + (r-l);  l=r; }
            else { m_i = m_p->r;  l += m_p->r - m_p->l; } }
    }    //Return true if new leaf is added.
    bool extend(const char* i, const char* r) {
        if (m_i < m_p->r) {
            const char* l;
            if (*m_i == *r) //implicit extension, no new leaf added.
            {   if (*r) { m_i++;  return false; }
                m_p->from.push_back( m_texts.size() );
                l = r - (m_p->r - m_p->l - 1); }
            else {
                //Insert a new internal node and add a new leaf.
                Node* in = newNode(m_p->l, m_i);
                in->pre=m_p->pre; in->csn=m_p->csn; in->prnt=m_p->prnt;
                if (m_p->pre) { m_p->pre->csn = in; }
                if (m_p->csn) { m_p->csn->pre = in; }
                if (m_p->prnt->ch == m_p) { m_p->prnt->ch = in; }
                in->addChild(NULL, m_p);  m_p->l = m_i;
                Node* leaf = newNode(r, m_text + m_len + 1);
                in->addChild( in->child(*r), leaf );  m_leafCnt++;
                leaf->from.push_back( m_texts.size() );
                //This new internal node may be suffix link of others.
                if (m_newIn) { m_newIn->sfxLink = in; }
                m_p = m_newIn = in;  l = r - (m_p->r - m_p->l);
            }
            //Find the position of next extension.
            m_p = m_p->prnt;
            if (m_p->sfxLink) { m_p = m_p->sfxLink; } else { l++; }
            goStr(l, r);
        }
        else {//in condition that m_i == m_p->r
            if (m_newIn) { m_newIn->sfxLink = m_p;  m_newIn = NULL; }
            Node* ch = m_p->child(*r);
            if (ch && *ch->l == *r)
            {   if (*r) { m_p=ch;  m_i = m_p->l + 1;  return false; }
                ch->from.push_back( m_texts.size() ); }
            else { Node* leaf = newNode(r, m_text + m_len + 1);
                   m_p->addChild(ch, leaf);  m_leafCnt++;
                   leaf->from.push_back( m_texts.size() ); }
            if (i < r) { m_p = m_p->sfxLink;  goStr(NULL, NULL); }
        }
        return true;
    }    Node* m_root;
    vector<const char*> m_texts;  vector<int> m_lens;
    //the following members are to help the extensions.
    Node *m_p,*m_newIn;
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