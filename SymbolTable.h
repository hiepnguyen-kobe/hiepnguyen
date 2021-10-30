#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include "main.h"

class SymbolTable {
public:
    SymbolTable() {}
    void run(string filename);
};

struct Node {
    string id;
    string type;
    string value;
	string funcFormat="";
	
    int level;
	int key ; 
	int func = 0 ;
    Node *lChild;
    Node*rChild;
    Node*parent; 
     bool isLeafNode()
	{
		return (!lChild && !rChild);
	}
    int compNode(Node*node){
        if(this->level > node->level) return 1 ; //1 means bigger than // 
         else if(this->level == node->level) {
             if(this->id.compare(node->id)==0) return 0 ; //means equal 
             else if(this->id.compare(node->id)>0)  return 1 ;
             else return -1 ; //means smaller
         }
         else return -1 ;
    }


    Node(): id(""), type(""), value(""), level(0), lChild(NULL),rChild(NULL),parent(NULL) {}
    
    Node(string id, string type,int level,int key): id(id), type(type), value(""), level(level),key(key), lChild(NULL),rChild(NULL),parent(NULL){}
    };

struct SplayTree{
  unsigned  int numofNodes;
    int level;
    Node*root;   

    void deleteAll(Node*node,string str[]){
		
        if (node == nullptr)
			return;

		deleteAll(node->lChild,str);
		deleteAll(node->rChild,str);
		delete node;
		
    }
    void insert(string line, string str[]);
    void assign(string line, string str[]);     
	void endDel(string str[]);
    void lookup(string line, string str[]);
	void deleteNode(Node*node);
    bool print(Node*root);
    SplayTree(): level(0), root(NULL) {}
	SplayTree(Node*root):level(0),root(root){}
	
   
   
   void LeftRotate(Node* y) {
		Node* curr = y->rChild;
		y->rChild = curr->lChild;
		if (curr->lChild != nullptr) {
			curr->lChild->parent = y;
		}
		curr->parent = y->parent;
		if (y->parent == nullptr) {
			this->root = curr;
		} else if (y == y->parent->lChild) {
			y->parent->lChild = curr;
		} else {
			y->parent->rChild = curr;
		}
		curr->lChild = y;
		y->parent = curr;
	}

    void RightRotate(Node* y) {
		Node* curr = y->lChild;
		y->lChild = curr->rChild;
		if (curr->rChild != nullptr) {
			curr->rChild->parent = y;
		}
		curr->parent = y->parent;
		if (y->parent == nullptr) {
			this->root = curr;
		} else if (y == y->parent->rChild) {
			y->parent->rChild = curr;
		} else {
			y->parent->lChild = curr;
		}
		curr->rChild = y;
		y->parent = curr;
	}
    
    void insertTree(Node*newNode, int &nComp){
       Node*parentNode = NULL ; 
	   Node* curr= this->root ; 
	    while(curr!=nullptr ){
			parentNode = curr ; 
			// bigger node go rChild

			if(newNode->compNode(curr)==1||newNode->compNode(curr)==0){
				curr= curr->rChild ; 
                nComp++ ;
			}
			else {
                curr = curr->lChild; 
				nComp++ ;
			}
			}
			newNode->parent = parentNode ; 
			if (parentNode == NULL) this->root = newNode;
	    else if (newNode->compNode(parentNode)==-1) parentNode->lChild = newNode; 
        else parentNode->rChild = newNode;    
		
		

    }
    void splay(Node*curr,int &numSplay){
		if(curr->parent != nullptr) {numSplay++;}
		while(curr->parent != NULL) { 
					if(curr->parent->parent == NULL){
				if (curr->parent->lChild == curr){
					//zig rotation 
					RightRotate(curr->parent);
				}
				//zag rotation
				else LeftRotate(curr->parent);
			}
			else {
				if (curr->parent->lChild == curr && curr->parent->parent->lChild ==curr->parent)
			      { 
	                        // zig-zig case 
							RightRotate(curr->parent->parent);
							 RightRotate(curr->parent);	  		
			      }
				else if (curr->parent->rChild == curr && curr->parent->parent->rChild ==curr->parent)
			      { 
	                        // zag-zag case 
							LeftRotate(curr->parent->parent) ; LeftRotate(curr->parent)  ;
			      }
			    else if (curr->parent->rChild== curr && curr->parent->parent->lChild == curr->parent){
					//zag-zig case 
					LeftRotate(curr->parent); RightRotate(curr->parent); 


				}
				else { 
					//zig-zag case 
					RightRotate(curr->parent);LeftRotate(curr->parent) ; 
				}

		}

    }
	}
	Node *searchBST( Node *root, Node *curr, int &numcomp){
    while(root!=NULL && root->compNode(curr)!=0){
        numcomp++;
        if (root->compNode(curr) == 1) root = root->lChild;
        else if(root->compNode(curr) == -1) root = root->rChild;
    }
    if (root->compNode(curr)==0) numcomp++;
    return root;
}
};
#endif
