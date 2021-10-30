#include "SymbolTable.h"
void deleteTree(Node *root){
    if (root==NULL) return;
    deleteTree(root->lChild);
    deleteTree(root->rChild);
    delete root;
}
void clearProgram(Node *root,string str[]){
    delete[] str;
    deleteTree(root);
}

void readline(string str[], string tmp){
    str[0] = ""; str[1]="";str[2]=""; str[3]="";
    int idx = 0; bool inString = false;
    for (unsigned int i=0; i< tmp.length(); i++){
        if (tmp[i]=='\'' && inString == false) {
            str[idx] += tmp[i];
            inString = true;
        }
        else if (tmp[i]== ' ' && inString!=true) idx++;  
        else {
            str[idx] += tmp[i];
            if (tmp[i]=='\'') inString = false;
        }
    }
}
bool checkInsert(string line) {
    regex r("INSERT [a-z][a-zA-Z0-9_]* ((number|string)|(([(](()|((number)|(string))((,number)|(,string))*)[)])->(string|number))) (true|false)");

    bool matched = regex_match(line, r);
    return matched;
}
bool checkString(string str){
       regex r("'[a-zA-Z0-9 ]+'");

    bool matched = regex_match(str, r);
    return matched;
}
bool checkNumber(string str){
    regex r("[0-9]+"); 
    bool matched = regex_match(str, r);
    return matched;

}
bool checkVar(string str){
    regex r("[a-z][a-zA-Z0-9_]*"); 
    bool matched = regex_match(str, r);
    return matched;
}

Node* findId(Node*root,string id ){
     if(root != nullptr){
        if(root->id==id){
           return root;
        } else {
            Node *foundNode = findId(root->lChild,id);
            if(foundNode == nullptr) {
                foundNode = findId(root->rChild,id);
            }
            return foundNode;
         }
    } 
    else {
        return nullptr;
    }
    
}
Node*findKey(Node*root,int key){
     if(root != nullptr){
        if(root->key==key){
           return root;
        } 
        else {
            Node *foundNode = findKey(root->lChild,key);
            if(foundNode == nullptr) {
                foundNode = findKey(root->rChild,key);
            }
            return foundNode;
         }
    }
     else {
        return nullptr;
    }

}

void SplayTree::insert(string line, string str[]) {
 if(!regex_match(str[1], regex("[a-z][a-zA-Z0-9_]*")) || !regex_match(str[3],regex("(true|false)")) || 
       !regex_match(str[2],regex("((string|number)|(([(](()|((number)|(string))((,number)|(,string))*)[)])->(string|number)))"))){
         clearProgram( this->root,str);
           
        throw InvalidInstruction(line);
    }
    int numcomp = 0 ; 
    int numsplay=0;
    Node*temp=findId(this->root,str[1]) ; 
    if(temp !=nullptr && temp->level==this->level){
       clearProgram(this->root,str);
      
       throw Redeclared(line);
    }
     Node*insertNode = new Node(str[1],str[2],str[3]=="false"?this->level:0,this->numofNodes+1);
    if(str[3]=="true"){
        insertNode->key=-1;
     }
    if(str[2]!="number"&& str[2]!="string") {
      //func type
         if(insertNode->level != 0){
             // func phai khai bao global scope 
           clearProgram(root,str);
               
             throw InvalidDeclaration(line);
             
         }
        else {
               insertNode->func=1;
               
                unsigned int len=str[2].length() ;
             //tach chuoi dang cua func ns-> (num,string)
        for(unsigned int i = 0 ; i < len-1 ;i++)
        {
             if (str[2][i]==','|| str[2][i] =='(' )
              {
                 
               insertNode->funcFormat+=str[2][i+1];
             }
               if(str[2][i] =='(' && str[2][i+1]==')'){insertNode->funcFormat="";}
               

        }
        
               if(str[2][len-1]=='g'){
                   insertNode->type="string";
               }
               else {
                   insertNode->type="number";
               }
        }
    } 
  this->insertTree(insertNode,numcomp);
  this->splay(insertNode,numsplay);
  cout << numcomp <<" "<<numsplay <<endl;
  this->numofNodes++ ;
     

}
void SplayTree::assign(string line, string str[]){
     int numcomp= 0; int  numsplay=0;
      if (!regex_match(str[1], regex("[a-z][a-zA-Z0-9_]*")) ||
        !regex_match(str[2], regex("((([a-z][a-zA-Z0-9_]*(([(][)])|([(]((([0-9]+,)|('[0-9a-zA-Z ]+',)|(([a-z][a-zA-Z0-9_]*),))*)(([0-9]+)|('[0-9a-zA-Z ]+')|([a-z][a-zA-Z0-9_]*))[)]))))|(('[0-9a-zA-Z ]+')|([0-9]+))|([a-z][a-zA-Z0-9_]*))"))){   
        clearProgram( this->root,str);  
        throw InvalidInstruction(line);
    }
    if (checkString(str[2])==1){   
        // return value la string
         
      Node*foundNode = findId(this->root,str[1]);   
         if (foundNode->func==1){
                clearProgram(this->root,str);   
                throw TypeMismatch(line);
          }
         else
          { 

              if (foundNode==NULL) {
              clearProgram( this->root,str);
              throw Undeclared(line);
                }
              else 
             {
                this->splay(searchBST(this->root,foundNode,numcomp), numsplay);
                if (foundNode->type=="number"){
                clearProgram(this->root,str);   
                throw TypeMismatch(line);
               }
                else if (foundNode->type=="string"){
            // Biến string
            foundNode->value = str[2];
            cout << numcomp << " " << numsplay << endl;
               }
             }
             
          }
   }
   else if (checkNumber(str[2])==1){
       // return value la number
       Node*foundNode = findId(this->root,str[1]);   
       if (foundNode->func==1){
                clearProgram(this->root,str);  
                throw TypeMismatch(line);
    }
       else
    { 

           if (foundNode==NULL) {
            clearProgram( this->root,str);  
            throw Undeclared(line);
        }
        else 
        {
            this->splay(searchBST(this->root,foundNode,numcomp), numsplay);
        if (foundNode->type=="string"){
                clearProgram(this->root,str);  
                throw TypeMismatch(line);
             }
        else if (foundNode->type=="number"){
            // Biến string
            foundNode->value = str[2];
            cout << numcomp << " " << numsplay << endl;
             }
        }
        
    }
   }
   else if(checkVar(str[2])==1){

       Node*foundNode = findId(this->root,str[1]);   
       Node*foundValue=findId(this->root,str[2]);
       if(foundValue==nullptr){
        clearProgram( this->root,str);
     
            throw Undeclared(line); 
        }
        
            this->splay(searchBST(this->root,foundValue,numcomp), numsplay);
      
        
         if (foundNode!=NULL && foundNode->func==1){
                clearProgram(this->root,str);
                throw TypeMismatch(line);
            }
       else
       { 

           if (foundNode==NULL) {
            clearProgram( this->root,str);  
            throw Undeclared(line); 
           }
          else 
           {
            this->splay(searchBST(this->root,foundNode,numcomp), numsplay);
           if(foundNode->type==foundValue->type){
               cout<<numcomp<<" "<<numsplay<<endl;
               foundNode->value=foundValue->value;
                } 
           else {
               clearProgram(this->root,str); 
               throw TypeMismatch(line);  
                }
           }
        
       }

   }
   else {
     
       // gan gia tri cho function
       string funcID =""; 
       for(unsigned int i =0 ; i<str[2].length();i++){
            if(str[2][i]=='('){ break;}
           funcID+=str[2][i];
          }
       Node*foundFunc=findId(this->root,funcID);
         if(foundFunc==nullptr){
           clearProgram( this->root,str);
            throw Undeclared(line);   
             }
      else 
      {
            this->splay(searchBST(this->root,foundFunc,numcomp), numsplay);
             string format = ""; string para = "";
          for (unsigned int i = str[2].find('(')+1; i< str[2].length(); i++){
            if (str[2][i]==',' || str[2][i] ==')') {
                if(checkNumber(para)) format = format+'n';
                else if(checkString(para)) format = format+'s';
                else if (checkVar(para)){
                    Node *foundparaID = findId(this->root,para);
                   
                    if (foundparaID == NULL) {
                        clearProgram(this->root,str); 
                        throw Undeclared(line);  
                    }
                    else if (foundparaID->func==1){
                        clearProgram(this->root,str);  
                        throw TypeMismatch(line);   
                    }
                    else {
                        this->splay(searchBST(this->root, foundparaID, numcomp), numsplay);
                        if( foundparaID->type=="string") format+='s';
                         else { format+='n';}
                    }

                  
                }
                para = "";
            }
            else para += str[2][i];
        }
        if(foundFunc->funcFormat!=format){
        clearProgram(this->root,str); 
        throw TypeMismatch(line); 
        }
         Node*foundNode=findId(this->root,str[1]);
         
         if(foundNode==NULL){
            clearProgram(this->root,str); 
            throw Undeclared(line);  
        }
        this->splay(searchBST(this->root, foundNode, numcomp), numsplay);
        if(foundNode->func==1){
            clearProgram(this->root,str);
            throw TypeMismatch(line);   
        } 
       
        if(foundNode->type!=foundFunc->type){
            clearProgram(this->root,str); 
            throw TypeMismatch(line);  
        }
        else{
           foundNode->value=str[2];
           cout<<numcomp<<" "<<numsplay<<endl;
        }
    }
 
   }
}
void SplayTree::lookup(string line,string str[]){
   Node*foundNode=findId(this->root,str[1]);
        if(foundNode==NULL){
            clearProgram(this->root,str);
            throw Undeclared(line);   
        }
    else {
        int numsplay=0;
        this->splay(foundNode, numsplay);
        cout << foundNode->level<< endl;
    }

}
void SplayTree::deleteNode(Node*node){
    int numsplay = 0 ;
    this->splay(node,numsplay);// dua nut can xoa len lam root 
      
    //ko co child 
    if(root->rChild==nullptr && root->lChild==nullptr){
        this->root =nullptr;
    }
    // co 1 child
    else if(root->lChild==nullptr && root->rChild != nullptr){
        this->root->rChild->parent=nullptr;
        this->root=root->rChild;
    }
    else if(root->lChild!=nullptr && root->rChild == nullptr){
        this->root->lChild->parent =nullptr;
        this->root=root->lChild;
    }
    // co 2 child : dua nut lon nhat phai nhat cua lchild len dau
    else {
        SplayTree Ltree = SplayTree(this->root->lChild);
             Ltree.root->parent = NULL;
        SplayTree Rtree = SplayTree(this->root->rChild);
             Rtree.root->parent = NULL;
        
            Node* mostRchild = Ltree.root;
            while (mostRchild->rChild != NULL) mostRchild = mostRchild->rChild;
            Ltree.splay(mostRchild, numsplay);
            Ltree.root->rChild = Rtree.root;
            if (Rtree.root != NULL) Rtree.root->parent = Ltree.root;
            this->root = Ltree.root;
        
    }
    delete node;
    return;
}
void SplayTree::endDel(string str[]){
  // delete theo thu tu key 
  if (this->level==0) {
      clearProgram(this->root,str);
      throw UnknownBlock();
  }
  int count =0 ;
  for(unsigned int i = 1; i<=this->numofNodes;i++){
      Node*delNode = findKey(this->root,i);
     if(delNode!=nullptr) {
         if(delNode->level==this->level){
          deleteNode(delNode);
          count++;
      
      }
      }
      delNode=nullptr;
  }
  this->numofNodes-=count ;
  this->level--;

}


void inorder(Node *root , string &res){
    if (root == NULL) return;
    else {
        res = res + root->id + "//" + to_string(root->level) + " ";
        inorder(root->lChild, res);
        inorder(root->rChild, res);
    }
    
}


void SymbolTable::run(string filename) {
    ifstream infile(filename);
    string line = "";
 SplayTree tree = SplayTree();
  string *str = new string[4];
        str[1]="";str[2]="";str[3]="";str[0]="";
    while (!infile.eof()) {
        getline(infile, line);
         readline(str,line);
        int ind = 0;
      
        

        if (str[0] == "INSERT") {
            if (checkInsert(line) == 0)
            {
                clearProgram(tree.root,str);
                throw InvalidInstruction(line);
            }
            tree.insert(line, str);
        }
        else if(str[0]=="ASSIGN"){
            tree.assign(line,str);
        }
        else if(str[0]=="LOOKUP"&&str[2]==""&&str[3]==""){
            tree.lookup(line,str);
        }
        else if(str[0]=="BEGIN"&&str[1]==""&&str[2]==""&&str[3]==""){
            tree.level++;
        }
        else if(str[0]=="END"&&str[1]==""&&str[2]==""&&str[3]==""){
            tree.endDel(str);
        }
        else if(str[0]=="PRINT"&&str[1]==""&&str[2]==""&&str[3]==""){
            string r="";
            inorder(tree.root,r);
            cout<<r.substr(0,r.length()-1);
            if(r!="") {cout<<endl;}
        
   }
   else {
            clearProgram(tree.root,str);
            throw InvalidInstruction(line);
        }
  
  
}
 if(tree.level!=0){
       throw UnclosedBlock(tree.level);
   }
   else {clearProgram(tree.root,str);}
infile.close();
}






