 #include <stdio.h>

 #include <string>

 #include <vector>

 #include <queue>

 #include <fstream>  

 #include <iostream>  

 #include <sstream>

 #include <iterator>

 #include <streambuf>

 

 class BkTree {

 public:

		13:     BkTree();

     ~BkTree();

     void insert(std::string m_item);

     int getWithinDistance(std::string center, size_t k, std::vector<std::string> *result = NULL);

 private:

		18:     size_t EditDistance( const std::string &s, const std::string &t );

     struct Node {

         std::string m_item;

         size_t m_distToParent;

         Node *m_firstChild;

         Node *m_nextSibling;

         Node(std::string x, size_t dist);        

         ~Node();

   };

   Node *m_root;

   int   m_size;

 protected:

		30: };

 

 BkTree::BkTree() {

     m_root = NULL; 

     m_size = 0;

 }

 

 BkTree::~BkTree() { 

     if( m_root ) 

			39:         delete m_root; 

 }

 

 BkTree::Node::Node(std::string x, size_t dist) {

     m_item         = x;

     m_distToParent = dist;

     m_firstChild   = m_nextSibling = NULL;

 }

 

 BkTree::Node::~Node() {

     if( m_firstChild ) 

			50:         delete m_firstChild;

     if( m_nextSibling ) 

			52:         delete m_nextSibling;

 }

 

 void BkTree::insert(std::string m_item) {

     if( !m_root ){

         m_size = 1;

         m_root = new Node(m_item, -1);

         return;

     }

     Node *t = m_root;

     while( true ) {

         size_t d = EditDistance( t->m_item, m_item );

         if( !d ) 

				65:             return;

         Node *ch = t->m_firstChild;

         while( ch ) {

             if( ch->m_distToParent == d ) { 

                 t = ch; 

                 break; 

             }

             ch = ch->m_nextSibling;

         }

         if( !ch ) {

             Node *newChild = new Node(m_item, d);

             newChild->m_nextSibling = t->m_firstChild;

             t->m_firstChild = newChild;

             m_size++;

             break;

         }

     }

 }

 

 size_t BkTree::EditDistance( const std::string &left, const std::string &right ) {

     size_t asize = left.size();

     size_t bsize = right.size();

     std::vector<size_t> prevrow(bsize+1);

     std::vector<size_t> thisrow(bsize+1);

 

     for(size_t i = 0; i <= bsize; i++)

			91:         prevrow[i] = i;

 

     for(size_t i = 1; i <= asize; i ++) {

         thisrow[0] = i;

         for(size_t j = 1; j <= bsize; j++) {

             thisrow[j] = std::min(prevrow[j-1] + size_t(left[i-1] != right[j-1]), 

						97:                                   1 + std::min(prevrow[j],thisrow[j-1]) );

         }

         std::swap(thisrow,prevrow);

     }

     return prevrow[bsize];

 }

 

 int BkTree::getWithinDistance(std::string center, size_t k, std::vector<std::string> *result) {

     if( !m_root ) return 0;

 

     int found = 0;

     std::queue< Node* > q;

     q.push( m_root );

 

     while( !q.empty() ) {

         Node *t = q.front(); 

         q.pop();

         size_t d = EditDistance( t->m_item, center );

         if( d <= k ) { 

             result->push_back( t->m_item );

             found++; 

         }

         Node *ch = t->m_firstChild;

         while( ch ) {

             if( d - k <= ch->m_distToParent && ch->m_distToParent <= d + k )

					 122:             q.push(ch);

             ch = ch->m_nextSibling;

         }

     }

     return found;

 }
