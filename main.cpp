/*
    Red Black Tree Part 1 (Insertion) by Nikhil Nayak
    Finished May 7, 2022

    Allows used to add to a binary search tree which rebalances itself to ensure searching runs in optimal time
*/
#include <cstring>
#include <fstream>
#include <iostream>

enum Color
{
  Red,
  Black
}; // stores metadata in node for color

class RBNode
{ // node of RB tree, holds value as well as its place in the tree using pointers to family
public:
  Color color;
  unsigned short data;

  RBNode *parent;
  RBNode *left;
  RBNode *right;

  RBNode()
  {
    parent = NULL;
    left = NULL;
    right = NULL;
    data = 0;
    color = Red;
  }

};

void rl(RBNode *&head, RBNode *&current)
{ // Rotate Left
  // this occurs when 3 nodes (parent, left, right) need to be rotated to (right, parent, left)
  RBNode *rightPointer = current->right;
  current->right = rightPointer->left;
  if (current->right != NULL)
  {
    current->right->parent = current;
  }
  rightPointer->parent = current->parent;
  // we need to do things different if current is the head
  if (current->parent == NULL)
  {
    head = rightPointer;
  }
  else if (current == current->parent->left)
  {
    current->parent->left = rightPointer;
  }
  else
  {
    current->parent->right = rightPointer;
  }
  rightPointer->left = current;
  current->parent = rightPointer;
}

void rr(RBNode *&head, RBNode *&current)
{ // Rotate Right
  // this occurs when 3 nodes (parent, left, right) need to be rotated to (left, right, parent)
  RBNode *leftPointer = current->left;
  current->left = leftPointer->right;
  if (current->left != NULL)
  {
    current->left->parent = current;
  }
  leftPointer->parent = current->parent;
  // we need to do things different if current is the head
  if (current->parent == NULL)
  {
    head = leftPointer;
  }
  else if (current == current->parent->left)
  {
    current->parent->left = leftPointer;
  }
  else
  {
    current->parent->right = leftPointer;
  }
  leftPointer->right = current;
  current->parent = leftPointer;
}

void balance(RBNode *&head, RBNode *&curr)
{
  RBNode *parent = NULL;
  RBNode *grandParent = NULL;
  while ((curr != head) && (curr->color != Black) && (curr->parent->color == Red))
  {
    parent = curr->parent;
    grandParent = parent->parent;
    // left of granparent
    if (parent == grandParent->left)
    {
      RBNode *uncle = grandParent->right;
      // uncle is red (recolor)
      if (uncle != NULL && uncle->color != Black)
      {
        grandParent->color = Red;
        parent->color = Black;
        uncle->color = Black;
        curr = grandParent;
      }
      else
      {
        // left rotation, current is right
        if (curr == parent->right)
        {
          rl(head, parent);
          curr = parent;
          parent = curr->parent;
        }
        // right rotation, current is left
        rr(head, grandParent);
        // swap colors

        Color parentOldColor = parent->color;
        parent->color = grandParent->color;
        grandParent->color = parentOldColor;
        curr = parent;
      }
    }
    // right of grandparent
    else
    {
      RBNode *uncle = grandParent->left;
      // recolor, uncle is red
      if (uncle != NULL && uncle->color == Red)
      {
        grandParent->color = Red;
        parent->color = Black;
        uncle->color = Black;
        curr = grandParent;
      }
      else
      {
        // left of parent -> right rotation
        if (curr == parent->left)
        {
          rr(head, parent);
          curr = parent;
          parent = curr->parent;
        }
        // right of parent -> left rotation
        rl(head, grandParent);
        // swap colors
        Color parentOldColor = parent->color;
        parent->color = grandParent->color;
        grandParent->color = parentOldColor;
        curr = parent;
      }
    }
  }
  head->color = Black;
}

void add(RBNode *&head, RBNode *&curr, RBNode *&prev, int value)
{ // inserts node with value "value" into head
  if (head == NULL)
  {
    head = new RBNode();
    head->data = value;
    head->color = Black;
    curr = head;
    return;
  }
  if (value < curr->data)
  {
    prev = curr;
    curr = curr->left;
    if (curr == NULL)
    {
      curr = new RBNode();
      curr->data = value;
      prev->left = curr;
      curr->parent = prev;
      balance(head, curr);
      return;
    }
    else
    {
      add(head, curr, prev, value);
    }
  }
  else
  {
    prev = curr;
    curr = curr->right;
    if (curr == NULL)
    {
      curr = new RBNode();
      curr->data = value;
      prev->right = curr;
      curr->parent = prev;
      balance(head, curr);
      return;
    }
    else
    {
      add(head, curr, prev, value);
    }
  }
}

void nprint(RBNode *head, int space)
{
  // https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
  // ^ used that to make formatting for the tree in stdout look nicer

  const int NUM_SPACES = 8; // increase for more spaced out tree print
  if (head == NULL)
    return;
  space += NUM_SPACES; // number of spaces per new line
  nprint(head->left, space);
  std::cout << "\n";
  for (int i = NUM_SPACES; i < space; i++)
    std::cout << " ";

  std::cout << head->data << "(" << (head->color == Red ? 'R' : 'B') << ")" << "(p=";

  if(head->parent == NULL) std::cout << "NULL";
  else std::cout << head->parent->data;
  std::cout << ")\n";


  nprint(head->right, space);
}

void addExt(RBNode *&head, int value)
{ // calls the add function and then balances
  RBNode *curr = head;
  RBNode *prev = NULL;
  add(head, curr, prev, value);
  if (curr != head)
  {
    balance(head, curr);
  }
}

int pop(std::ifstream &file)
{
  char input[10];
  file.getline(input, 10, ' ');
  return atoi(input);
}

int random(int high)
{
  return (rand() % high) + 1;
}

void upload(RBNode *&root)
{ // gets numbers from numbers.txt and adds them to RBTree

  // (mostly) same code from BST project
  std::cout << "How many numbers: ";
  int numnums;
  std::cin >> numnums;

  std::ifstream file("numbers.txt");

  for (int i = 0, l = random(25); i < l; i++)
    pop(file); // randomly skip the first 0-25 numbers
  for (int i = 0; i < numnums; i++)
  {
    for (int i = 0, l = random(25); i < l; i++)
      pop(file); // randomly skip 0-25 numbers
    int num = pop(file);
    addExt(root, num);
  }
}

bool exists(RBNode *head, int n)
{
  if (head == NULL)
    return false;
  if (n == head->data)
    return true;

  if (n < head->data)
    return exists(head->left, n);
  else
    return exists(head->right, n);

  return false;
}

RBNode *successor(RBNode *&x)
{
  // Get the left most value of right subtree
  RBNode *a = x;
  while (a->left != NULL)
  {
    a = a->left;
  }
  return a;
}

RBNode *replaceNode(RBNode *&x)
{
  // If node has 2 children
  if (x->left != NULL && x->right != NULL)
  {
    RBNode *right = x->right;
    return successor(right);
  }
  // If node has no children
  else if (x->left == NULL && x->right == NULL)
  {
    return NULL;
  }
  // If node has 1 child
  else
  {
    if (x->left != NULL)
    {
      return x->left;
    }
    else
    {
      return x->right;
    }
  }
}

RBNode *getSibling(RBNode *&x)
{ // gets node's parent's other child
  if(x->parent == NULL) return NULL;
  return (x == x->parent->left) ? x->parent->right : x->parent->left;
}

bool isRedChild(RBNode *&x)
{ // determines if node's chlid is red
  if ((x->left != NULL && x->left->color == Red) || (x->right != NULL && x->right->color == Red))
  {
    return true;
  }
  return false;
}

void fixBlackChild(RBNode *&head, RBNode *&n)
{ // fixes black black
  if (n == head)
    return;

  RBNode *sibling = getSibling(n);
  RBNode *parent = n->parent;

  if (sibling == NULL)
  {
    fixBlackChild(head, parent);
  }
  else
  {
    if (sibling->color == Red)
    { // if red sibling
      parent->color = Red;
      sibling->color = Black;
      if (sibling == parent->left)
      {
        rr(head, parent);
      }
      else
      {
        rl(head, parent);
      }
      fixBlackChild(head, n);
    }
    else
    {
      if (isRedChild(sibling))
      { // if black sibling
        if (sibling->left != NULL && sibling->left->color == Red)
        { // >=1 red child (left)
          if (sibling == parent->left)
          {
            // left left
            sibling->left->color = sibling->color;
            sibling->color = parent->color;
            rr(head, parent);
          }
          else
          {
            // right left
            sibling->left->color = parent->color;
            rr(head, sibling);
            rl(head, parent);
          }
        }
        else
        {
          // sibling right chlid is red
          if (sibling == parent->left)
          {
            // left right
            sibling->right->color = parent->color;
            rl(head, sibling);
            rr(head, parent);
          }
          else
          {
            // right right
            sibling->right->color = sibling->color;
            sibling->color = parent->color;
            rl(head, parent);
          }
        }
        parent->color = Black;
      }
      else
      {
        // both children are black
        sibling->color = Red;
        if (parent->color == Black)
        {
          fixBlackChild(head, parent);  // fix recursively
        }
        else
        {
          parent->color = Black;
        }
      }
    }
  }
}
void delete_(RBNode *&head, RBNode *&toDelete)
{ // main delete function for Node in RB Tree
  RBNode *rep = replaceNode(toDelete);
  RBNode *parent = toDelete->parent;
  // Bool to track if both are black
  bool bothBlack = ((rep == NULL || rep->color == Black) && (toDelete == NULL || toDelete->color == Black));

  // If x has no children
  if (rep == NULL)
  {
    if (toDelete == head)
    {
      head = NULL;
    }
    else
    {
      if (bothBlack)
      {
        fixBlackChild(head, toDelete);
      }
      else
      {
        // One is red -> make sibling red
        if (getSibling(toDelete) != NULL)
        {
          getSibling(toDelete)->color = Red;
        }
      }
      // remove from tree
      if (toDelete == parent->left)
      {
        parent->left = NULL;
      }
      else
      {
        parent->right = NULL;
      }
    }
    toDelete->~RBNode();
    return;
  }

  // 1 child
  if (toDelete->right == NULL || toDelete->left == NULL)
  {
    if (toDelete == head)
    {
      // rep -> toDelete
      toDelete->data = rep->data;
      toDelete->left = NULL;
      toDelete->right = NULL;
    }
    else
    {
      // move rep up
      if (toDelete == parent->left)
      {
        parent->left = rep;
      }
      else
      {
        parent->right = rep;
      }
      rep->parent = parent;
      if (bothBlack)
      {
        fixBlackChild(head, toDelete);
      }
      else
      {
        rep->color = Black;
      }
    }
    return;
  }

  // 2 children
  // swap data
  unsigned short t = rep->data;
  toDelete->data = rep->data;
  rep->data = t;

  delete_(head, rep);
}

int main()
{
  RBNode *head = NULL;
  while (true)
  {
    std::cout << "Enter Character For Command - A(dd), R(ead), P(rint) E(xists), D(elete): ";
    char inp[256];
    std::cin >> inp;
    switch (inp[0])
    {
    case 'A':
    {
      std::cout << "What number should I add: ";
      int num;
      std::cin >> num;
      addExt(head, num);
      break;
    };
    case 'R':
    {
      upload(head);
      break;
    };
    case 'P':
    {
      nprint(head, 0);
      break;
    };
    case 'E':
    {
      std::cout << "What number would you like to check the presence of: ";
      int n;
      std::cin >> n;
      std::cout << (exists(head, n) ? "Exists" : "Doesn't Exist") << "\n";
      break;
    };
    case 'D':
    {
      if (head == NULL)
        break;
      std::cout << "What do you want to delete: ";
      int value;
      std::cin >> value;
      bool nodeExists = exists(head, value);
      if (!nodeExists)
        std::cout << "Node Doesn't Exist!\n";

      RBNode *v = head;
      while (v->data != value)
      { // find the right node
        if (value < v->data)
        {
          v = v->left;
        }
        else if (value > v->data)
        {
          v = v->right;
        }
      }
      delete_(head, v);
      break;
    };
    }
  }
}
