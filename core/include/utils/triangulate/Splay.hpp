//-------------------------------------------------------------------------/
//Copyright (C) 2003, 2004, 2005, ALL RIGHTS RESERVED.
//Centre for Sys. Eng. & App. Mech.           FEMAGSoft S.A.
//Universite Cathalique de Louvain            4, Avenue Albert Einstein
//Batiment Euler, Avenue Georges Lemaitre, 4  B-1348 Louvain-la-Neuve
//B-1348, Louvain-la-Neuve                    Belgium
//Belgium
//-------------------------------------------------------------------------/
//
//Name:         splay.h (self-adjusting balanced binary search tree)
//Purpose:      declaration and implentation of splay tree class for fast 
//              binary searching
//Author:       Liang, Wu (wu@mema.ucl.ac.be, wuliang@femagsoft.com)
//Created:      03/2002
//Modified:     2003, 2004, 2005
//-------------------------------------------------------------------------/

#ifndef _SPLAY_TREE_H_
#define _SPLAY_TREE_H_

#include <iostream>  
#include <iomanip>
#include <assert.h>

template <class T, class KeyType>
class SplayTree;

template <class T, class KeyType>
class BTreeNode
{
  public:
      friend class SplayTree<T, KeyType>;
      BTreeNode( ) : _left( NULL ), _right( NULL ), _visited(false) { }
      BTreeNode( const T & data, BTreeNode *lt, BTreeNode *rt )
        : _data(data),_left( lt ), _right( rt ), _visited(false) { }
     
      T& data()                     { return _data; }
      BTreeNode* Left()             { return _left; }
      BTreeNode* Right()            { return _right; }   
      void SetVisited(bool visited) { _visited=visited; }
      bool GetVisited()             { return _visited; }
      KeyType keyValue()            { return _data->keyValue(); }

  private:
      T          _data;
      BTreeNode *_left;
      BTreeNode *_right;
      bool      _visited;

};


template <class T, class KeyType>
class SplayTree
{
  public:
      explicit SplayTree( ):root(NULL),size(0) { }
      SplayTree( const SplayTree & rhs );
      ~SplayTree( );

      void MakeEmpty( );
      bool IsEmpty( ) const;
      long int Size() { return size; }
      BTreeNode<T, KeyType>* Root() { return root; }     

      void Find( const KeyType keys, BTreeNode<T, KeyType>* & res);
      void FindMin( BTreeNode<T, KeyType>* &min );
      void FindMax( BTreeNode<T, KeyType>* &max );
      // We only use this function for polygon Triangulation to find the direct
      // left edge of an event vertex;
      void FindMaxSmallerThan( const KeyType& keys, BTreeNode<T, KeyType>* &res);
      
      void Insert( const T & x );
      void Delete( KeyType keys);
      void Delete( KeyType keys, BTreeNode<T, KeyType>* &res);
      void DeleteMin(BTreeNode<T, KeyType>* &min);
      void DeleteMax(BTreeNode<T, KeyType>* &max);

      const SplayTree & operator=( const SplayTree & rhs );
      void PreOrder( void(*Visit)(BTreeNode<T,KeyType> *u) ) 
                { PreOrder(Visit, root); }
      void InOrder( void(*Visit)(BTreeNode<T,KeyType> *u) ) 
                { InOrder(Visit, root); }

      void InOrder( void(*Visit)(BTreeNode<T,KeyType>*u, ostream&), ostream& os)
                { InOrder(Visit, root, os); }
      
      void InOrder( void(*Visit)(BTreeNode<T,KeyType>*u, double y), double y)
	                      { InOrder(Visit, root, y); }

      void PostOrder( void(*Visit)(BTreeNode<T,KeyType> *u) ) 
                { PostOrder(Visit, root); }

      int Height( ) const { return Height(root); }  //height of root
      int Height(BTreeNode<T, KeyType> *t) const;    //Height of subtree t;
      BTreeNode<T, KeyType>* Left(BTreeNode<T, KeyType> *node) { return node->_left; }
      BTreeNode<T, KeyType>* Right(BTreeNode<T, KeyType> *node) { return node->_right; }     

  private:
      BTreeNode<T, KeyType> *root;
      long int              size;          

      void reclaimMemory( BTreeNode<T, KeyType> * t ) const;
      BTreeNode<T, KeyType> * clone( BTreeNode<T, KeyType> *t ) const;

      //Transverse
      void PreOrder( void(*Visit)(BTreeNode<T, KeyType> *u), BTreeNode<T, KeyType> *t);
      void InOrder( void(*Visit)(BTreeNode<T, KeyType> *u), BTreeNode<T, KeyType> *t);
      void PostOrder( void(*Visit)(BTreeNode<T, KeyType> *u), BTreeNode<T, KeyType> *t);
      void InOrder( void(*Visit)(BTreeNode<T, KeyType>*, ostream&), 
		BTreeNode<T, KeyType> *t, ostream& os);
      
      void InOrder( void(*Visit)(BTreeNode<T, KeyType>*, double y),
		                      BTreeNode<T, KeyType> *t, double y);
      


      // Tree manipulations
      void rotateWithLeftChild( BTreeNode<T, KeyType> * & k2 ) const;
      void rotateWithRightChild( BTreeNode<T, KeyType> * & k1 ) const;
      void splay( KeyType keys, BTreeNode<T, KeyType> * & t ) const;
};

//----------------------------------------------------------------------
//Constructor;
//----------------------------------------------------------------------
template <class T, class KeyType>
SplayTree<T, KeyType>::SplayTree( const SplayTree<T, KeyType> & rhs )
{
      *this = rhs;
}

//-----------------------------------------------------------------------
//Destructor.
//-----------------------------------------------------------------------
template <class T, class KeyType>
SplayTree<T, KeyType>::~SplayTree( )
{
      MakeEmpty( );
}

//------------------------------------------------------------------------
//Insert x into the tree.
//------------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::Insert( const T & x )
{

     BTreeNode<T, KeyType> *newNode= new BTreeNode<T, KeyType>;
     newNode->_data=x;
    
     if( root == NULL )
      {
          newNode->_left = newNode->_right = NULL;
          root = newNode; size++; 
      } 
     else
      {
         KeyType keys=x->keyValue();
         splay( keys, root );
         KeyType rootk=root->keyValue();
         if( keys < rootk )
           {
              newNode->_left = root->_left;
              newNode->_right = root;
              root->_left = NULL;
              root = newNode; 
              size++;
           }
        else if( keys > rootk )
           {
              
              newNode->_right = root->_right;
              newNode->_left = root;
              root->_right = NULL;
              root = newNode;
              size++;
           }
        else 
           {
             //slight incresed the keyvalue to avoid duplicated keys		   
             x->increaseKeyValue(1.0e-10);
             Insert(x);
	     
           }         
      }
}

//---------------------------------------------------------------------
//Remove the node with the keys from the tree, and retrieve the result
//---------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::Delete( KeyType keys, BTreeNode<T, KeyType>* &res)
{
      BTreeNode<T, KeyType> *newTree;

      splay( keys, root );
      if( root->keyValue() != keys ) { res=NULL; return; } // Item not found; do nothing
 
      res = root;

      if( root->_left == NULL )
           newTree = root->_right;
      else
         {
            // Find the maximum in the _left subtree
            // Splay it to the root; and then attach _right child
            newTree = root->_left;
            splay( keys, newTree );
            newTree->_right = root->_right;
         }

      root = newTree;
      size--; 
}

//---------------------------------------------------------------------
//Remove the node with the keys from the tree.
//---------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::Delete( KeyType keys)
{
      BTreeNode<T, KeyType> *newTree;

      splay( keys, root );
      KeyType rootk=root->keyValue();
      if( rootk != keys ) { return; } // Item not found; do nothing

      if( root->_left == NULL ) newTree = root->_right;
      else
         {
            // Find the maximum in the _left subtree
            // Splay it to the root; and then attach _right child
            newTree = root->_left;
            splay( keys, newTree );
            newTree->_right = root->_right;
         }

      delete root;
      root = newTree;
      size--;
}



//---------------------------------------------------------------------
//Find and Delete the node with min keys from the tree;
//---------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::DeleteMin(BTreeNode<T, KeyType>* &min)
{
  if( IsEmpty( ) )  { min=NULL; return; }

  double keys=-1.0e30;
  splay( keys, root );
 
  min = root;

  BTreeNode<T, KeyType> *newTree;
  if( root->_left == NULL ) newTree = root->_right;
  else
     {
            newTree = root->_left;
            splay( keys, newTree );
            newTree->_right = root->_right;
      }

   size--;
   root = newTree;

} 

//----------------------------------------------------------------------
//Find and Delete the node with max keys from the tree;
//----------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::DeleteMax(BTreeNode<T, KeyType>* &max)
{
  if( IsEmpty( ) )  { max=NULL; return; }
 
  double keys=1.0e30;
  splay( keys, root );
 
  max = root;
  
  BTreeNode<T, KeyType> *newTree;
  if( root->_left == NULL ) newTree = root->_right;
  else
     {
            newTree = root->_left;
            splay( keys, newTree );
            newTree->_right = root->_right;
      }
   size--;
   root = newTree;
}


//----------------------------------------------------------------------
//Find the smallest item in the tree, but won't delete it from the tree.
//----------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::FindMin(BTreeNode<T, KeyType>* & min )
{
      if( IsEmpty( ) )  { min=NULL; return; }
      BTreeNode<T, KeyType> *ptr = root;

      while( ptr->_left != NULL ) ptr = ptr->_left;
      splay( ptr->keyValue(), root );
      min = ptr;
}

//----------------------------------------------------------------------
//Find the largest item in the tree. but won't delete it from the tree.
//----------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::FindMax(BTreeNode<T, KeyType>* & max)
{
      if( IsEmpty( ) )   { max=NULL; return; }  

      BTreeNode<T, KeyType> *ptr = root;
      while( ptr->_right != NULL ) ptr = ptr->_right;
      splay( ptr->keyValue(), root );
      max =  ptr;
}

//--------------------------------------------------------------------
//Find the node with the keys in the tree.
//res==NULL if it donesn't exist in the tree;
//--------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::Find( const KeyType keys, BTreeNode<T, KeyType>* & res) 
{
      if( IsEmpty( ) ) { res=NULL; return; }
      splay( keys, root );
      if( root->keyValue() != keys ) { res=NULL; return; }
      else res = root;
}

//--------------------------------------------------------------------
//Find the maximum node smaller than or equal to the given key.
//This function specially designed for polygon Triangulation to
//find the direct left edge at event vertex;
//--------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::FindMaxSmallerThan( const KeyType& keys, BTreeNode<T, KeyType>* &res)
{
      if( IsEmpty( ) ) { res=NULL; return; }
      splay( keys, root );
      
      if( root->data()->keyValue() < keys) res=root; 
      else if(root->_left) 
      {       
	      res=root->_left;
	      while(res->_right) res=res->_right;  
      }
      else 
      {
	      assert(false);
      }
}

//--------------------------------------------------------------------
//Make the tree logically empty.
//--------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::MakeEmpty( )
{
      BTreeNode<T, KeyType>* ptr;
      while( !IsEmpty( ) ) 
         {
            DeleteMax(ptr);
            delete ptr;
          } 
}

//---------------------------------------------------------------------
//Test if the tree is logically empty.
//---------------------------------------------------------------------
template <class T, class KeyType>
bool SplayTree<T, KeyType>::IsEmpty( ) const
{
      return root == NULL;
}

//----------------------------------------------------------------------
//copy overload operator.
//----------------------------------------------------------------------
template <class T, class KeyType>
const SplayTree<T, KeyType> & SplayTree<T, KeyType>::operator=( const SplayTree<T, KeyType> & rhs )
{
      if( this != &rhs )
       {
          MakeEmpty( );
          root = clone( rhs.root );
       }

      return *this;
}

//-----------------------------------------------------------------------
//Internal method to perform a top-down splay.
//x is the key of target node to splay around.
//t is the root of the subtree to splay.
//-----------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::splay( KeyType keys, BTreeNode<T, KeyType> * & t ) const
{
      BTreeNode<T, KeyType> *_leftTreeMax, *_rightTreeMin;
      static BTreeNode<T, KeyType> header;

      header._left = header._right = NULL;
      _leftTreeMax = _rightTreeMin = &header;

      for( ; ; )
        {
          KeyType rkey=t->keyValue();
          if( keys < rkey )
            {
               if(t->_left == NULL) break;
               if( keys < t->_left->keyValue() ) rotateWithLeftChild( t );
                if( t->_left == NULL ) break;
               
               // Link Right
               _rightTreeMin->_left = t;
               _rightTreeMin = t;
               t = t->_left;
            }
          else if( keys > rkey )
            {
              if( t->_right == NULL ) break;
              if( keys > t->_right->keyValue() ) rotateWithRightChild( t );
              if( t->_right == NULL ) break;    
 
              // Link Left
              _leftTreeMax->_right = t;
              _leftTreeMax = t;
              t = t->_right;
             }
         else  break;
        }

      _leftTreeMax->_right = t->_left;
      _rightTreeMin->_left = t->_right;
      t->_left = header._right;
      t->_right = header._left;
 
}

//--------------------------------------------------------------------
//Rotate binary tree node with _left child.
//--------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::rotateWithLeftChild( BTreeNode<T, KeyType> * & k2 ) const
{
      BTreeNode<T, KeyType> *k1 = k2->_left;
      k2->_left = k1->_right;
      k1->_right = k2;
      k2 = k1;
}

//---------------------------------------------------------------------
//Rotate binary tree node with _right child.
//---------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::rotateWithRightChild( BTreeNode<T, KeyType> * & k1 ) const
{
      BTreeNode<T, KeyType> *k2 = k1->_right;
      k1->_right = k2->_left;
      k2->_left = k1;
      k1 = k2;
}

//----------------------------------------------------------------------
//Internal method to reclaim internal nodes in subtree t.
//WARNING: This is prone to running out of stack space.
//----------------------------------------------------------------------
template <class T, class KeyType>
void SplayTree<T, KeyType>::reclaimMemory( BTreeNode<T, KeyType> * t ) const
{
      if( t != t->_left )
      {
          reclaimMemory( t->_left );
          reclaimMemory( t->_right );
          delete t;
      }
}

//-----------------------------------------------------------------------
//Internal method to clone subtree.
//WARNING: This is prone to running out of stack space.
//-----------------------------------------------------------------------
template <class T, class KeyType>
BTreeNode<T, KeyType> * SplayTree<T, KeyType>::clone( BTreeNode<T, KeyType> * t ) const
{
      if( t == t->_left )  // Cannot test against NULLNode!!!
             return NULL;
      else   
             return new BTreeNode<T, KeyType>( t->_data, clone( t->_left ), clone( t->_right ) ); 
}

//-----------------------------------------------------------------------
//Tranverse the tree by pre-order method;
//-----------------------------------------------------------------------
template<class T, class KeyType>
void SplayTree<T, KeyType>::PreOrder( void(*Visit)(BTreeNode<T, KeyType> *u), BTreeNode<T, KeyType> *t)
{
   if(t!=NULL)
    {
       Visit(t);
       PreOrder(Visit,t->_left);
       PreOrder(Visit,t->_right);
    } 
     
}

//-----------------------------------------------------------------------
//Tranverse the tree by in-order method;
//-----------------------------------------------------------------------
template<class T, class KeyType>
void SplayTree<T, KeyType>::InOrder( void(*Visit)(BTreeNode<T, KeyType> *u), BTreeNode<T, KeyType> *t)
{
   if(t!=NULL)
     {
        InOrder(Visit,t->_left);
        Visit(t);
        InOrder(Visit,t->_right);
      }
}


//-----------------------------------------------------------------------
//Tranverse the tree by in-order method;
//-----------------------------------------------------------------------
template<class T, class KeyType>
void SplayTree<T, KeyType>::InOrder( void(*Visit)(BTreeNode<T, KeyType>*u, ostream&)
, BTreeNode<T, KeyType> *t, ostream& os)
{
   if(t!=NULL)
     {
        InOrder(Visit,t->_left, os);
        Visit(t, os);
        InOrder(Visit,t->_right, os);
      }
}

//-----------------------------------------------------------------------
//Tranverse the tree by in-order method;
//-----------------------------------------------------------------------
template<class T, class KeyType>
void SplayTree<T, KeyType>::InOrder( void(*Visit)(BTreeNode<T, KeyType>*u, double y)
, BTreeNode<T, KeyType> *t, double y)
{
   if(t!=NULL)
     {
        InOrder(Visit,t->_left, y);
        Visit(t, y);
        InOrder(Visit,t->_right, y);
      }
}



//-----------------------------------------------------------------------
//Tranverse the tree by post-order method;
//-----------------------------------------------------------------------
template<class T, class KeyType>
void SplayTree<T, KeyType>::PostOrder( void(*Visit)(BTreeNode<T, KeyType> *u), BTreeNode<T, KeyType> *t)
{
   if(t!=NULL)
     {
        PostOrder(Visit,t->_left);
        PostOrder(Visit,t->_right);
        Visit(t);
     }
}

//-----------------------------------------------------------------------
//return the height of subtree
//-----------------------------------------------------------------------
template<class T, class KeyType>
int SplayTree<T, KeyType>::Height(BTreeNode<T, KeyType> *subtree) const
{
    if(subtree==NULL) return 0;
    int lh=Height(subtree->_left);
    int rh=Height(subtree->_right);
   
    return (lh>rh)?(++lh):(++rh);
}   

#endif

