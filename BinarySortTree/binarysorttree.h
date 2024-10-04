// binarysorttree.h
#ifndef BINARYSORTTREE_H
#define BINARYSORTTREE_H

#include <qpainter.h>

template<typename T>
class TreeNode {
public:
    T val;
    TreeNode* left;
    TreeNode* right;

    // Constructor Function
    // Parameter Initialization Table
    TreeNode(T x) : val(x), left(nullptr), right(nullptr) {}

    // destuctor function : no new object claimed
};

template<typename T>
class BinarySortTree {
private:
    TreeNode<T>* root;

    // insert node recursively
    void insert(TreeNode<T>*& node, const T& val) {
        if (node == nullptr)
            node = new TreeNode<T>(val);
        else if (val < node->val)
            insert(node->left, val);
        // already exist this value
        else if (val == node->val)
            return;
        else
            insert(node->right, val);
    }

    // search node recursively
    TreeNode<T>* search(TreeNode<T>* node, const T& val) {
        if (node == nullptr || node->val == val)
            return node;

        if (val < node->val)
            return search(node->left, val);
        else
            return search(node->right, val);
    }

    // delete the node
    TreeNode<T>* deleteNode(TreeNode<T>*& root, const T& key) {
        if (root == nullptr) return root;

        if (key < root->val)
            root->left = deleteNode(root->left, key);
        else if (key > root->val)
            root->right = deleteNode(root->right, key);
        else {
            if (root->left == nullptr) {
                TreeNode<T>* temp = root->right;
                delete root;
                return temp;
            }
            else if (root->right == nullptr) {
                TreeNode<T>* temp = root->left;
                delete root;
                return temp;
            }

            // use min on the right to replace root
            TreeNode<T>* temp = minValueNode(root->right);

            root->val = temp->val;

            root->right = deleteNode(root->right, temp->val);
        }
        return root;
    }

    void draw(TreeNode<T>* node, QPainter &painter, int x, int y, int level) {
        if (!node) return;

        int offset = 50 / (level + 1);
        painter.drawText(x, y, QString::number(node->val));

        if (node->left) {
            painter.drawLine(x, y, x - offset, y + 40);
            draw(node->left, painter, x - offset, y + 40, level + 1);
        }

        if (node->right) {
            painter.drawLine(x, y, x + offset, y + 40);
            draw(node->right, painter, x + offset, y + 40, level + 1);
        }
    }

    // find the min-value node
    TreeNode<T>* minValueNode(TreeNode<T>* node) {
        TreeNode<T>* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

public:
    // construct function
    BinarySortTree() : root(nullptr) {}

    bool isEmpty(){
        return root == nullptr;
    }

    TreeNode<T>* getRoot(){
        return root;
    }

    // insert into element
    void insert(const T& val) {
        insert(root, val);
    }

    // search for certain element
    bool search(const T& val) {
        return search(root, val) != nullptr;
    }

    // delete certain element,
    // including leaves, root, and other nodes
    void remove(const T& val) {
        // redefine the root node
        root = deleteNode(root, val);
    }

    void deleteTree(){
        root = nullptr;
    }
};

#endif // BINARYSORTTREE_H
