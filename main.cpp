/*
    Red Black Tree Part 1 (Insertion) by Nikhil Nayak
    Finished May 7, 2022

    Allows used to add to a binary search tree which rebalances itself to ensure searching runs in optimal time
*/
#include <cstring>
#include <fstream>
#include <iostream>

enum Color {Red, Black}; // stores metadata in node for color

class RBNode{ // node of RB tree, holds value as well as its place in the tree using pointers to family
public:
	Color color;
	int data;

	RBNode* parent;
	RBNode* left;
	RBNode* right;


	RBNode(){
		parent = NULL;
		left = NULL;
		right = NULL;
		data = 0;
		color = Red;
	}

};


void rl(RBNode*& head, RBNode*& current) { //Rotate Left
    // this occurs when 3 nodes (parent, left, right) need to be rotated to (right, parent, left)
    RBNode* rightPointer = current->right;
    current->right = rightPointer->left;
    if (current->right != NULL) {
        current->right->parent = current;
    }
    rightPointer->parent = current->parent;
    // we need to do things different if current is the head
    if (current->parent == NULL) {
        head = rightPointer;
    }
    else if (current == current->parent->left) {
        current->parent->left = rightPointer;
    }
    else {
        current->parent->right = rightPointer;
    }
    rightPointer->left = current;
    current->parent = rightPointer;
}

void rr(RBNode*& head, RBNode*& current) { //Rotate Right
    // this occurs when 3 nodes (parent, left, right) need to be rotated to (left, right, parent)
    RBNode* leftPointer = current->left;
    current->left = leftPointer->right;
    if (current->left != NULL) {
        current->left->parent = current;
    }
    leftPointer->parent = current->parent;
    // we need to do things different if current is the head
    if(current->parent == NULL){
        head = leftPointer;
    }
    else if(current == current->parent->left){
        current->parent->left = leftPointer;
    }
    else {
        current->parent->right = leftPointer;
    }
    leftPointer->right = current;
    current->parent = leftPointer;
}



void balance(RBNode*& head, RBNode*& curr){
    RBNode* parent = NULL;
    RBNode* grandParent = NULL;
    while ((curr != head) && (curr->color != Black) && (curr->parent->color == Red)) {
        parent = curr->parent;
        grandParent = parent->parent;
        // left of granparent
        if (parent == grandParent->left) {
            RBNode* uncle = grandParent->right;
            // uncle is red (recolor)
            if (uncle != NULL && uncle->color != Black) {
                grandParent->color = Red; 
                parent->color = Black; 
                uncle->color = Black; 
                curr = grandParent;
            }
            else {
                // left rotation, current is right
                if (curr == parent->right) {
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
        else {
            RBNode* uncle = grandParent->left;
            // recolor, uncle is red
            if (uncle != NULL && uncle->color == Red) {
                grandParent->color = Red;
                parent->color = Black;
                uncle->color = Black;
                curr = grandParent;
            }
            else {
                // left of parent -> right rotation
                if (curr == parent->left) {
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


void add(RBNode*& head, RBNode*& curr, RBNode*& prev, int value){ // inserts node with value "value" into head
	if(head == NULL){
		head = new RBNode();
		head->data = value;
		head->color = Black;
		curr = head;
		return;
	}
	if(value < curr->data){
		prev = curr;
		curr = curr->left;
		if(curr == NULL){
			curr = new RBNode();
			curr->data = value;
			prev->left = curr;
			curr->parent = prev;
			balance(head, curr);
			return;
		}
		else{
			add(head, curr, prev,value);
		}
	}
	else{
		prev = curr;
		curr = curr->right;
		if(curr == NULL){
			curr = new RBNode();
			curr->data = value;
			prev->right = curr;
			curr->parent = prev;
			balance(head, curr);
			return;
		}
		else{
			add(head, curr, prev, value);
		}
	}
}


void nprint(RBNode* head, int space){
    // https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
    // ^ used that to make formatting for the tree in stdout look nicer

    const int NUM_SPACES = 5; // increase for more spaced out tree print
    if(head == NULL) return;
    space += NUM_SPACES; // number of spaces per new line
    nprint(head->left, space);
    std::cout << "\n";
    for(int i = NUM_SPACES; i < space; i++) std::cout << " ";
    std::cout << head->data << (head->color == Red ? 'R' : 'B') << "\n";

    nprint(head->right, space);
}


void addExt(RBNode*& head, int value){ // calls the add function and then balances
    RBNode* curr = head;
    RBNode* prev = NULL;
    add(head, curr, prev, value);
    if(curr != head){
        balance(head, curr);
    }
}


int pop(std::ifstream& file){
    char input[10];
    file.getline(input, 10, ' ');
    return atoi(input);
}

int random(int high){
    return (rand() % high) + 1;
}

void upload(RBNode *&root){ // gets numbers from numbers.txt and adds them to RBTree
    
    // (mostly) same code from BST project
	std::cout << "How many numbers: ";
	int numnums;
	std::cin >> numnums;

    std::ifstream file("numbers.txt");

    for(int i = 0, l = random(25); i < l; i++) pop(file); // randomly skip the first 0-25 numbers
    for(int i = 0; i < numnums; i++){
        for(int i = 0, l = random(25); i < l; i++) pop(file); // randomly skip 0-25 numbers
        int num = pop(file);
        addExt(root, num);
    }
}

int main(){
    RBNode* head = NULL;
    while(true){
        std::cout << "Enter Character For Command - A(dd), R(ead), P(rint): ";
        char inp[256];
        std::cin >> inp;
        switch(inp[0]){
            case 'A': {
                std::cout << "What number should I add: ";
                int num;
                std::cin >> num;
                addExt(head, num);
                break;
            };
            case 'R': {
                upload(head);
                break;
            };
            case 'P': {
                nprint(head, 0);
                break;
            };
        }
    }
}
