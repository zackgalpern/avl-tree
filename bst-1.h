#pragma once
#include <vector>
#include <exception>
using namespace std;

namespace cop3530 {

template<typename Key, typename T>
class bst {
    struct node {
        Key key;
        T value;
        node* leftChild;
        node* rightChild;
        int height;

        node() {
            leftChild = nullptr;
            rightChild = nullptr;
            height = 0;
        }
        node(Key k, T t) {
            this->key = k;
            this->value = t;
            leftChild = nullptr;
            rightChild = nullptr;
            height = 0;

        }
    };

    node* globalRoot;
    int treeSize;

    public:
        bst();
        ~bst();
        void destroy(node* root);
        void insert(const Key &key, const T &t);
        typename bst<Key, T>::node* insertRec(node* &root, const Key &key, const T &t);
        void erase(const Key &key);
        typename bst<Key, T>::node* rightHelper(node* root);
        typename bst<Key, T>::node* eraseRec(node* &root, const Key &key);
        T& at(const Key &key);
        T& atRec(node* root,const Key &key);
        bool contains(const Key &key) const;
        //bool containsRec(node* root, const Key &key) const;
        bool empty() const;
        size_t size() const;
        std::vector<std::pair<Key, T>> inorder_contents();
        void inorderRec(vector<pair<Key,T>> &contents, node* root);
        std::vector<std::pair<Key, T>> preorder_contents();
        void preorderRec(vector<pair<Key,T>> &contents, node* root);

        //AVL
        typename bst<Key, T>::node* left_rotate(node* &newRoot);
        typename bst<Key, T>::node* right_rotate(node* &newRoot);
        //typename bst<Key, T>::node* left_right_rotate(node* &root);
        //typename bst<Key, T>::node* right_left_rotate(node* &root);
        int getHeight(node* &root);
        int getBalance(node* &root);

};

template<typename Key, typename T>
bst<Key, T>::bst() {
    globalRoot = nullptr;
    treeSize = 0;
}

template<typename Key, typename T>
bst<Key, T>::~bst() {
    destroy(globalRoot);
    treeSize = 0;
}

template<typename Key, typename T>
    void bst<Key, T>::destroy(node *root) {
        if (root != nullptr) {
            if (root->leftChild != nullptr)
                destroy(root->leftChild);
            if (root->rightChild != nullptr)
                destroy(root->rightChild);
            root = nullptr;
            delete root;
        }
    }


template<typename Key, typename T>
void bst<Key, T>::insert(const Key &key, const T &t) {
    globalRoot = insertRec(globalRoot, key, t);
    treeSize++;
}

template<typename Key, typename T>
typename bst<Key, T>::node* bst<Key, T>::insertRec(node* &root, const Key &key, const T &t) {
    if (root == nullptr) { //base case
        node* newNode = new node(key,t);
        root = newNode;
    }

    else if (key < root->key) {
        if (root->leftChild == nullptr) {
            root->leftChild = new node(key, t);
        }
        root->leftChild = insertRec(root->leftChild, key, t);
    }

    else if (key > root->key) {
        if (root->rightChild == nullptr) {
            root->rightChild = new node(key, t);
        }
        root->rightChild = insertRec(root->rightChild, key, t);
    }

    else if (root->key == key) { //base case
        root->value = t;
    }

    root->height = max(getHeight(root->leftChild), getHeight(root->rightChild)) + 1;
    int bal = getHeight(root->leftChild) - getHeight(root->rightChild);

    //cases for rotation
    if (bal > 1 && key<root->leftChild->key) //right (left-left) rotation on the root
        return right_rotate(root);
    if (bal < -1 && key>root->rightChild->key) //left (right-right) rotation on the root
        return left_rotate(root);
    if (bal > 1 && key>root->leftChild->key) { //left-right rotation on the root
        root->leftChild = left_rotate(root->leftChild);
        return right_rotate(root);
    }
    if (bal < -1 && key<root->rightChild->key) { //right-left rotation on the root
        root->rightChild = right_rotate(root->rightChild);
        return left_rotate(root);
    }

    return root;
}

template<typename Key, typename T>
void bst<Key, T>::erase(const Key &key) {
    eraseRec(globalRoot, key);
    treeSize--;
}

template<typename Key, typename T>
typename bst<Key, T>::node* bst<Key, T>::rightHelper(node* root) {
    if (root == nullptr)
        return nullptr;

    while (root->leftChild != nullptr)
        root = root->leftChild;
    return root;
}

template<typename Key, typename T>
typename bst<Key, T>::node* bst<Key, T>::eraseRec(node* &root, const Key &key) {
    if (root == nullptr) {
        throw out_of_range("Key not found in BST");
    }

    if (key < root->key)
        root->leftChild = eraseRec(root->leftChild, key);
    else if (key > root->key)
        root->rightChild = eraseRec(root->rightChild, key);
    else { //item is in the local root
        if (root->leftChild == nullptr && root->rightChild == nullptr) {
            delete root;
            root = nullptr; //if root's children is what you want to delete, delete the root

        } else if (root->rightChild == nullptr) {
            node *temp = root->leftChild;
            delete root;
            root = temp;

        } else if (root->leftChild == nullptr) {
            node *temp = root->rightChild;
            delete root;
            root = temp;

        } else {
            node *temp = rightHelper(root->rightChild);
            root->key = temp->key;
            root->value = temp->value;
            root->rightChild = eraseRec(root->rightChild, temp->key);
        }
    }


        if (root == nullptr) {
            return root;
        }

        root->height = max(getHeight(root->leftChild), getHeight(root->rightChild)) + 1;
        int bal = getBalance(root);
        int lBal = getBalance(root->leftChild);
        int rBal = getBalance(root->rightChild);

        //cases for rotation
        if (bal > 1 && lBal >= 0) //right (left-left) rotation on the root
            return right_rotate(root);
        if (bal < -1 && rBal <= 0) //left (right-right) rotation on the root
            return left_rotate(root);
        if (bal > 1 && lBal < 0) { //left-right rotation on the root
            root->leftChild = left_rotate(root->leftChild);
            return right_rotate(root);
        }
        if (bal < -1 && rBal > 0) { //right-left rotation on the root
            root->rightChild = right_rotate(root->rightChild);
            return left_rotate(root);
        }
    return root;
}

template<typename Key, typename T>
T& bst<Key, T>::at(const Key &key) {
    return atRec(globalRoot, key);
}

template<typename Key, typename T>
T& bst<Key, T>::atRec(node* root,const Key &key) {
    if (root == nullptr) //base case
        throw out_of_range("This key is not found in the BST");

    else if (root->key == key) //base case
        return root->value;

    else if (root->key < key) { //recursive case
        return atRec(root->rightChild, key);
    }

    else {
        return atRec(root->leftChild, key);
    }

}

template<typename Key, typename T>
bool bst<Key, T>::contains(const Key &key) const {
    node* curr = globalRoot;

    while(true) {
        if (curr == nullptr)
            return false;
        else if (curr->key == key)
            return true;
        else if (curr->key > key)
            curr = curr->leftChild;
        else
            curr = curr->rightChild;
    }
}

template<typename Key, typename T>
bool bst<Key, T>::empty() const {
    if (treeSize == 0)
        return true;
    else
        return false;
}

template<typename Key, typename T>
size_t bst<Key, T>::size() const {
    return treeSize;
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::inorder_contents() {
    std::vector<std::pair<Key, T>> contents;
    inorderRec(contents, globalRoot);
    return contents;
}

template<typename Key, typename T>
void bst<Key, T>::inorderRec(vector<pair<Key, T>> &contents, node* root) {
  if (root == nullptr) //base case
        return;

    inorderRec(contents, root->leftChild);
    contents.push_back(make_pair(root->key, root->value));
    inorderRec(contents, root->rightChild);
}

template<typename Key, typename T>
std::vector<std::pair<Key, T>> bst<Key, T>::preorder_contents() {
    std::vector<std::pair<Key, T>> contents;
    preorderRec(contents, globalRoot);
    return contents;
}

template<typename Key, typename T>
void bst<Key, T>::preorderRec(vector<pair<Key, T>> &contents, node* root) {
    if (root == nullptr) //base case
        return;

    contents.push_back(make_pair(root->key, root->value));
    preorderRec(contents, root->leftChild);
    preorderRec(contents, root->rightChild);
}

template<typename Key, typename T>
typename bst<Key, T>::node* bst<Key, T>::left_rotate(node* &newRoot) {
    node* grandchild = newRoot->rightChild->leftChild;
    node* newParent = newRoot->rightChild;
    newParent->leftChild = newRoot;
    newRoot->rightChild = grandchild;

    newRoot->height = max(getHeight(newRoot->leftChild), getHeight(newRoot->rightChild)) + 1;
    newParent->height = max(getHeight(newParent->leftChild), getHeight(newParent->rightChild)) + 1;

    if (newRoot == globalRoot) {
        globalRoot = newParent;
    }

    return newParent;
}

template<typename Key, typename T>
typename bst<Key, T>::node* bst<Key, T>::right_rotate(node* &newRoot) {
    node* grandchild = newRoot->leftChild->rightChild;
    node* newParent = newRoot->leftChild;
    newParent->rightChild = newRoot;
    newRoot->leftChild = grandchild;

    newRoot->height = max(getHeight(newRoot->leftChild), getHeight(newRoot->rightChild)) + 1;
    newParent->height = max(getHeight(newParent->leftChild), getHeight(newParent->rightChild)) + 1;

    if (newRoot == globalRoot) {
        globalRoot = newParent;
    }

    return newParent;
}

template<typename Key, typename T>
int bst<Key, T>::getHeight(node* &root) {
    if (root == nullptr)
        return -1;
    else
        return root->height;
}

template<typename Key, typename T>
int bst<Key, T>::getBalance(node* &root) {
    if(root == nullptr)
        return 0;
    return getHeight(root->leftChild) - getHeight(root->rightChild);
}


}
