/* compile : g++ -std=c++11 main.cpp
Author : Ali Kerem Yıldız
Student ID = 150170013

*/

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

bool flag = false; // via using this flag we decide whether we need to insert data or just update it. To see usage of it, go RBInsert function

enum Color
{
    BLACK,
    RED
};

class Player {

    public:
    string last_season;
    string name;
    string team;
    int total_points;
    int total_assists;
    int total_rebounds;
    Player(){}
    Player(string last_season, string name, string team, int total_points, int total_assists, int total_rebounds);
    void updateInfo(int new_point, int new_assist, int new_rebounds,string last_season,string team);
};

Player::Player(string last_season, string name, string team, int total_points, int total_assists, int total_rebounds){
    this->last_season = last_season;
    this->name = name;
    this->team = team;
    this->total_points = total_points;
    this->total_assists = total_assists;
    this->total_rebounds = total_rebounds;
}
void Player::updateInfo(int new_point,int new_assist,int new_rebounds,string last_season,string team){
    total_points += new_point;
    total_assists += new_assist;
    total_rebounds += new_rebounds;
    this->last_season = last_season;
    this->team = team;
}


class Node{
    public:
    Player player;
    Node *parent;
    Node *left_child;
    Node *right_child;
    Node *get_sibling();
    Node *get_uncle();
    enum Color color;
    string key;
    string s_color;
    Node(Player *player);
    void print();
};
int calculateDepth(Node* temp) // calculating depth of a node in order to put correct number of "-" when printinh
{
    int x = 0;
    while (temp->parent != nullptr) {
        x++;
        temp = temp->parent; 
    }
    return x;
}
void Node::print() 
{
    if (this->color == BLACK) {
        this->s_color = "BLACK";
    }
    else {
        this->s_color = "RED";
    }
    int i = calculateDepth(this);
    for (int j = 0; j < i;j++){
        cout << "-";
    }
        cout << "(" << this->s_color << ")" <<" "<<this->key << endl;
}
Node* Node::get_sibling() // returns sibling 
{
    if(this->parent == nullptr) 
    {
        return nullptr;
    }
    if(this == parent->left_child) 
    {
        return parent->right_child;
    }
    else
    {
        return parent->left_child;
    }
}
Node* Node::get_uncle() // returns uncle
{
    return parent->get_sibling();
}
Node::Node(Player *player) // contructor with given player infos, node color given red by default from Red Black Tree property
{
    this->player = *player;
    this->key = player->name;
    this->parent = nullptr;
    this->left_child = nullptr;
    this->right_child = nullptr;
    this->color = RED; // every nodes color should be red as default
}
Node* BSTInsert(Node* root,Node *&new_node) // simple binary search tree insertion operation
{
    if (root == nullptr) 
    {
        return new_node;
    }
    else
    {
        if (new_node->key < root->key) // if key value less than root, insert leftsubtree of root, recursively
        {
            root->left_child= BSTInsert(root->left_child,new_node);
            root->left_child->parent = root; // until now, parent has child but child to parent connection is not established. we are doing it here
        }
        else if (new_node->key > root->key)// new_node's key greater than root's key
        {
            root->right_child = BSTInsert(root->right_child,new_node);
            root->right_child->parent = root; 
        }
        else // if the data which will be inserted is already inserted, key values maps. So we shouldnt add extra node to the tree, we need to just update its value
        {
            root->player.updateInfo(new_node->player.total_points, new_node->player.total_assists, new_node->player.total_rebounds,new_node->player.last_season,new_node->player.team);
            flag = true; // flag shows us node is updated, not inserted. So, we dont need to fix violations.
            new_node = root;
        }
        return root;
    }
}

class RBTree {
    public: 
    Node *root;
    void RBInsert(Node *new_node);
    RBTree();
    void FixViolations(Node* new_node);
    void insert1(Node *&new_node);
    void insert2(Node *&new_node);
    void insert3(Node *&new_node);
    void insert4(Node *&new_node);
    void insert5(Node *&new_node);
    void RotateLeft(Node *&ew_node);
    void RotateRight(Node *&new_node);

    Player *MaxScorer;
    Player *MaxAssister;
    Player *MaxRebounder;
    int max_point;
    int max_rebound;
    int max_assist;
};
RBTree::RBTree() // RBTree constructor
{
    root = nullptr;
    max_point = 0;
    max_rebound = 0;
    max_assist = 0;
}
void preorder(Node* iter) // preorder traversal as asked
{
    if (iter == nullptr)
        return;

    iter->print();
    preorder(iter->left_child);
    preorder(iter->right_child);

}
void RBTree::RotateLeft(Node *&ptr) {
    Node *right_child = ptr->right_child;
    ptr->right_child = right_child->left_child;

    if (ptr->right_child != nullptr)
        ptr->right_child->parent = ptr;

    right_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = right_child;
    else if (ptr == ptr->parent->left_child)
        ptr->parent->left_child = right_child;
    else
        ptr->parent->right_child = right_child;

    right_child->left_child = ptr;
    ptr->parent = right_child;
}
void RBTree::RotateRight(Node *&ptr) {
    Node *left_child = ptr->left_child;
    ptr->left_child = left_child->right_child;

    if (ptr->left_child != nullptr)
        ptr->left_child->parent = ptr;

    left_child->parent = ptr->parent;

    if (ptr->parent == nullptr)
        root = left_child;
    else if (ptr == ptr->parent->left_child)
        ptr->parent->left_child = left_child;
    else
        ptr->parent->right_child = left_child;

    left_child->right_child = ptr;
    ptr->parent = left_child;
}

void RBTree::insert1(Node *&new_node) // parent and uncle red. therefore, we dont need to rotate now. we can simply recolor node's parent,uncle and grandparent. Then iterate violation upwards
{
    new_node->parent->color = BLACK;
    new_node->get_uncle()->color = BLACK;
    new_node->parent->parent->color = RED;
    FixViolations(new_node->parent->parent);// violation iterated to grandparent
}
void RBTree::insert2(Node *&new_node) // uncle is black, parent is red,left child and node is right child. we need to transform it to fit into insert3 function. tree is now zig-zagged. we need to make violations kind of linear
{
    Node* p = new_node->parent;
    RotateLeft(p); //
    new_node = new_node->left_child; // we rotate it to left. so our node is parent of its previous parent. and violation (red-red) occurs in its previoys parent now. Therefore we update the node with its previous parent
}
void RBTree::insert3(Node *&new_node) // uncle is black and parent is left child and node itself is leftchild. node-parent-grandparent are not zig-zagged. They are like linear.
{
    Node *p = new_node->parent;
    Node *g = new_node->parent->parent;
    RotateRight(g); // i heuristically approached it. when rotate from parent, we cannot evaluate proper tree. Since uncle is black and they are linear we need to rotate it from grandparent
    p->color = BLACK;
    g->color = RED;
}
void RBTree::insert4(Node *&new_node) // same with insert2, they are symmetrical only change left and right values
{
    Node *p = new_node->parent;
    RotateRight(p); //TODO:Rotateright(p)
    new_node = new_node->right_child;
}
void RBTree::insert5(Node *&new_node) // same with insert3, they are symmetrical only change left and right values
{
    Node *p = new_node->parent;
    Node *g = new_node->parent->parent;
    
    RotateLeft(g); //TODO:Rotateleft(g)
    p->color = BLACK;
    g->color = RED;
}
void RBTree::FixViolations(Node *new_node)
{
    if (new_node != root && new_node->parent->color == RED) 
    {
        if (new_node->parent == new_node->parent->parent->left_child) // if it's parent is left child of it's parent
        {
            Node *uncle = new_node->parent->parent->right_child;
            if(uncle != nullptr && uncle->color == RED) 
            {
                insert1(new_node); //Case 1 : uncle is red and uncle is right child and parent is left child
            }
            else // uncle is black or null (null is also black)
            {
                if(new_node == new_node->parent->right_child) // if node is right child of it's parent
                {
                    insert2(new_node); // Case 2 : uncle is black and node is right child
                }
                insert3(new_node); //Case 3 : case2 buna dönüştürülür uncle is black and node is left child
            }
        } 
        else // node's parent is right child
        {
            Node *uncle = new_node->parent->parent->left_child;
            if(uncle != nullptr && uncle->color == RED) 
            {
                insert1(new_node); //  Case 1 : uncle is red and uncle is left child and parent is right child
            }
            else // uncle is black or null (null is also black)
            {
                if(new_node == new_node->parent->left_child) // if node is left child of it's parent
                {
                    insert4(new_node); //  Case 4 : uncle is black and node is left child
                }
                insert5(new_node); // Case 5 : case4 buna dönüştürülür uncle is black and node is right child
            }
        }
    } 
}

void RBTree::RBInsert(Node *new_node) 
{
    flag = false; // flag is false until it changes to true. Because by default, nodes should be inserted. Flag shows whether update or insert operation is done
    root = BSTInsert(root,new_node); // if it is going to be updated, we comvert flag to true in BSTInsert function
    if(!flag) // if flag is true which means we updated a value. therefore we dont need to fix violates
    {
        FixViolations(new_node);

        root->color = BLACK; // root must be always black.
    }
    if(new_node->player.total_points > max_point) // compare max_points with node's value
    {
        max_point = new_node->player.total_points;
        this->MaxScorer = &new_node->player;
    }
    if(new_node->player.total_assists > max_assist) // compare max_assist with node's value
    {
        max_assist = new_node->player.total_assists;
        this->MaxAssister = &new_node->player;
    }
    if(new_node->player.total_rebounds > max_rebound)// compare max_rebound with node's value
    {
        max_rebound = new_node->player.total_rebounds;
        this->MaxRebounder = &new_node->player;
    }
}
int main(int argc, char **argv) {

    RBTree rb_tree;

    ifstream ip(argv[1]);

    if (!ip.is_open())
        cout << "ERROR in file opening"<<endl;
    string season;
    string name;
    string team;
    string s_rebound; // string rebound
    string s_assist; // string assist
    string s_point; // string point
    string header;
    string foo;
    getline(ip, header); //this is the header line

    int i = 0; // i is for detecting first season in order to just print the first season in tree form
    string prev_season=""; // stores previous season's value in order to check if season changed
    
    while(ip.good()){
        bool _flag = false;
        getline(ip, season,',');
        getline(ip, name,',');
        getline(ip, team,',');
        getline(ip, s_rebound, ',');
        getline(ip, s_assist,',');
        getline(ip, s_point,'\n');

        int rebound = stod(s_rebound); // string to integer
        int assist = stod(s_assist);
        int point = stod(s_point);

        if(prev_season != season) // if season change, incremet i, after that we check its value in order to print only first season.
        {
            i++;
            if(i > 1)
            {
                cout <<"End of the " << prev_season << " Season" << endl;
                cout <<"Max Points: " << rb_tree.MaxScorer->total_points << " - "<<"Player Name: "<<rb_tree.MaxScorer->name<< endl;
                cout <<"Max Assists: " << rb_tree.MaxAssister->total_assists <<" - " <<"Player Name: " <<rb_tree.MaxAssister->name <<endl;
                cout <<"Max Rebs: " << rb_tree.MaxRebounder->total_rebounds <<" - " <<"Player Name: " <<rb_tree.MaxRebounder->name <<endl;

            }
            prev_season = season;
            _flag = true; // that means season changed
        }
        if(i == 2 && _flag) // if we finish first season we need to print it.
        {
        preorder(rb_tree.root);
        }

        Node *newNode = new Node(new Player(season,name,team,point,assist,rebound));
        rb_tree.RBInsert(newNode);
        
    }
    // last season cannot be printed in previoys section, therefore we print it here.
    cout <<"End of the " << prev_season << " Season" << endl;
    cout <<"Max Points: " << rb_tree.MaxScorer->total_points << " - "<<"Player Name: "<<rb_tree.MaxScorer->name<< endl;
    cout <<"Max Assists: " << rb_tree.MaxAssister->total_assists <<" - " <<"Player Name: " <<rb_tree.MaxAssister->name <<endl;
    cout <<"Max Rebs: " << rb_tree.MaxRebounder->total_rebounds <<" - " <<"Player Name: " <<rb_tree.MaxRebounder->name <<endl;
;
    return 0;

}