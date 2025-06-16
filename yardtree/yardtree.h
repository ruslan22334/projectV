#pragma once
#include <cstdlib>

//YardTree - treap data structure for efficient range queries and dynamic updates
struct YardTreeNode {
    int key;
    int priority;
    YardTreeNode* left;
    YardTreeNode* right;

    YardTreeNode(int k)
            : key(k), priority(rand()), left(nullptr), right(nullptr) {}
};

class YardTree {
public:
    YardTreeNode* root;

    YardTree() : root(nullptr) {}

    void insert(int key) {
        root = insertRec(root, new YardTreeNode(key));
    }

    void mergeWith(YardTree& other) {
        root = merge(root, other.root);
        other.root = nullptr;
    }

    void splitRange(int key, YardTree& leftTree, YardTree& rightTree) {
        split(root, key, leftTree.root, rightTree.root);
    }

private:
    YardTreeNode* insertRec(YardTreeNode* t, YardTreeNode* it) {
        if (!t) return it;

        if (it->priority > t->priority) {
            split(t, it->key, it->left, it->right);
            return it;
        } else if (it->key < t->key) {
            t->left = insertRec(t->left, it);
        } else {
            t->right = insertRec(t->right, it);
        }
        return t;
    }

    YardTreeNode* merge(YardTreeNode* l, YardTreeNode* r) {
        if (!l || !r) return l ? l : r;

        if (l->priority > r->priority) {
            l->right = merge(l->right, r);
            return l;
        } else {
            r->left = merge(l, r->left);
            return r;
        }
    }

    void split(YardTreeNode* t, int key, YardTreeNode*& l, YardTreeNode*& r) {
        if (!t) {
            l = r = nullptr;
            return;
        }

        if (key < t->key) {
            split(t->left, key, l, t->left);
            r = t;
        } else {
            split(t->right, key, t->right, r);
            l = t;
        }
    }
};
