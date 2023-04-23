#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

  TreeMap *newTreeMap = (TreeMap*) malloc(sizeof(TreeMap));
  newTreeMap->root = NULL;
  newTreeMap->current = NULL;
  newTreeMap->lower_than = lower_than;
  
  return newTreeMap;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
  if(tree == NULL)
  {
    return;
  }

  TreeNode* actual = tree->root;
  TreeNode* newNode = createTreeNode(key, value);

  if(actual == NULL)
  {
    tree->root = actual;
    tree->current = actual;
  }
  else
  {
    while(actual != NULL)
    {
      if(is_equal(tree, actual->pair->key,key))
      {
        return;
      }
      else if(tree->lower_than(key, actual->pair->key))
      {
        if(actual->left == NULL)
        {
          newNode->parent = actual;
          actual->left = newNode;
          tree->current = newNode;
          return;
        }
        actual = actual->left;
      }
      else
      {
        if(actual->right == NULL)
        {
          newNode->parent = actual;
          actual->right = newNode;
          tree->current = newNode;
          return;
        }
        actual = actual->right;
      }
      
    }
  }
}

TreeNode * minimum(TreeNode * x)
{
  if(x->left == NULL)
  {
    return x;
  }
  else
  {
     return minimum(x->left); 
  }
}


void removeNode(TreeMap * tree, TreeNode* node) 
{
  if(tree == NULL || tree->root == NULL)
  {
    return;
  }

  if(node->left == NULL && node->right == NULL)
  {
    if(node->parent == NULL)
    {
      tree->root = NULL;
    }
    else
    {
      if(node->parent->left == node)
      {
        node->parent->left = NULL;
      }
      else if(node->parent->right == node)
      {
        node->parent->left = NULL;
      }
    }
  }
  else if (node->left ==  NULL)
  {
    if(node->parent == NULL)
    {
      tree->root = node->right;
    }
    else
    {
      if(node->parent->left == node)
      {
         node->parent->left = node->right; 
      }
      else
      {
        node->parent->right = node->right;
      }

      node->right->parent = node->parent;
    }
  }
  else if(node->right == NULL)
  {
    if(node->parent == NULL)
    {
      tree->root = node->left;
    }
    else
    {
      if(node->parent->left == node)
      {
        node->parent->left = node->left;
      }
      else
      {
        node->parent->right = node->left;
      }
      node->left->parent = node->parent;
    }
  }
  else
  {
    TreeNode *minimo = minimum(node->right);

    node->pair->key = minimo->pair->key;
    node->pair->key = minimo->pair->key;

    removeNode(tree, minimo);
    
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}


Pair * searchTreeMap(TreeMap * tree, void* key) {
  if(tree->root == NULL)
  {
    return NULL;
  }

  TreeNode *actual = tree->root;

  while(actual != NULL)
  {
    if(is_equal(tree, key, actual->pair->key))
    {
      tree->current = actual;
      return actual->pair;
    }
    else if(tree->lower_than(key, actual->pair->key))
    {
      actual = actual->left;
      
    }
    else
    {
      actual = actual->right;
    }
  }
  
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) 
{
  if(tree == NULL)
  {
    return NULL;
  }

  TreeNode *aux;
  TreeNode *keyMayor;

  //Caso que existe la key en el arbol
  if(searchTreeMap(tree, key) != NULL)
  {
    return searchTreeMap(tree,key);
  }

  //Caso que no exista la key en el arbol
  aux = tree->root;

  while(aux != NULL)
  {
    if(keyMayor == NULL ||tree->lower_than(aux->pair->key, keyMayor->pair->key) && tree->lower_than(key, aux->pair->key))
    {
      keyMayor = aux;
    }

    if(tree->lower_than(aux->pair->key,key) == 1)
    {
      aux = aux->right;
    }
    else if (tree->lower_than(key,aux->pair->key)==1)
    {
      aux = aux->left;
    }

    if(keyMayor == NULL)
    {
      return NULL;
    }

    tree->current = keyMayor;
    return keyMayor->pair;

  }  
  

  return NULL;
}

Pair * firstTreeMap(TreeMap * tree) 
{
  if(tree == NULL || tree->root == NULL)
  {
    return NULL;
  }

  tree->current = tree->root;
  TreeNode *minimo = minimum(tree->current);
  tree->current= minimo;
  return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) 
{
  if(tree == NULL || tree->root == NULL || tree->current == NULL)
  {
    return NULL;
  }

  if(tree->current->right != NULL)
  {
    tree->current = tree->current->right;
    TreeNode *minimo = minimum(tree->current);

    if(minimo != NULL)
    {
      tree->current = minimo;
      return tree->current->pair;
    }
    else
    {
      tree->current = tree->current;
      return NULL;
    }
  }
  else
  {
    TreeNode *actual = tree->current;
    TreeNode *padre = tree->current->parent;

    while(padre != NULL && actual == padre->right)
    {
      actual = padre;
      padre = padre->parent;
    }
    
    tree->current = padre;
    
    if(tree->current != NULL)
    {
      return tree->current->pair;
    }
    else
    {
      return NULL;
    }
    
  }
  

}
