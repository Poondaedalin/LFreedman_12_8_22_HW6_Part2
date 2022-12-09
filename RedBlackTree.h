#ifndef REDBLACKTREE_H
#define REDBLACKTREE_H
#include <string>
#include <vector>

using namespace std;

#define COLOR_BLACK 0
#define COLOR_RED 1
#define COLOR_BLACK_BLACK 2

struct RBTNode {
int val;
int color;
RBTNode* left = nullptr;
RBTNode* right = nullptr;
RBTNode* prev = nullptr;
};

class RedBlackTree {

    public:
        RedBlackTree();
        RedBlackTree(const RedBlackTree &other);
        ~RedBlackTree();
        bool Contains(int a);
        int GetMin();
        int GetMax();
        int GetRoot() {return root->val;};
        void Insert(int a);
        void Remove(int a);
        RBTNode* in_order_successor(RBTNode* rbt);
        unsigned long long int Size() { return numItems; };
        string ToInfixString() const {return ToInfixString(root);};
        string ToPrefixString() const { return ToPrefixString(root);};
        string ToPostfixString() const { return ToPostfixString(root);};

    private:
        unsigned long long int numItems = 0;
        vector<int> blueprint; // Not used in operations, but used for copy constructor
        RBTNode *root = nullptr;
        RBTNode* empty = new RBTNode;
        void copy_helper(RBTNode* where, RedBlackTree *rbt);
        void remove_helper(RBTNode* where, int a);
        void shift_up(RBTNode* u, RBTNode* v);
        const string ToInfixString(RBTNode* rbt) const;
        const string ToPrefixString(RBTNode* rbt) const;
        const string ToPostfixString(RBTNode* rbt) const;
        bool Search(RBTNode* where, int a);
        void validate(RBTNode* rbt);
        void validate_remove(RBTNode* where);
        void L_Rotate(RBTNode* rbt);
        void R_Rotate(RBTNode* rbt);
        RBTNode* zenith(RBTNode* where, int a);
        int SearchMin(RBTNode* where);
        int SearchMax(RBTNode* where);
};

#endif