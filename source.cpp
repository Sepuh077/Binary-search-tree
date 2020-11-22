#include <iostream>

template <class T>
struct nodeTree {
    nodeTree* left;
    nodeTree* right;
    int key;
    T value;
    nodeTree(T v, int k, nodeTree* l = NULL, nodeTree* r = NULL) {
        value = v;
        key = k;
        left = l;
        right = r;
    }
};

template <class T>
class binSTree {
    nodeTree<T>* first;
    
    struct node {
        node* prev;
        node* next;
        nodeTree<T>* v;
        bool use; // ստուգվելու է՝ preorder֊ի եւ postorder֊ի ժամանակ v-ի աջ ճյուղը ստուգվել է, թե ոչ։
        node() {
            use = 0;
            prev = nullptr;
            next = nullptr;
        }
    };

    //այս կլասը օգտագործվելու է միայն շրջանցումների ժամանակ
    struct list {
        node* front;
        node* back;

        list() {
            front = NULL;
            back = NULL;
        }

        void push_front(nodeTree<T>* q) {
            node* a = new node;
            a->v = q;
            if(front) {
                front->prev = a;
                a->next = front;
                front = a;
            }
            else {
                front = a;
                back = a;
            }
        }

        void push_back(nodeTree<T>* q) {
            node* a = new node;
            a->v = q;
            if(back) {
                back->next = a;
                a->prev = back;
                back = a;
            }
            else {
                front = a;
                back = a;
            }
        }

        void pop_back() {
            if(front == NULL || front == back) {
                clearList();
            }
            else {
                back = back->prev;
            }
        }

        void clearList() {
            front = NULL;
            back = NULL;
        }

        bool find(nodeTree<T>* a) {
            node* q = back;
            while(q->v != a) {
                q = q->prev;
            }
            return q->use;// եթե արժեքը 0 է ուրեմն՝ preorder֊ի ժամանակ տրված տարրի աջ ճյուղը չի ստոգվել
        }

        void change(nodeTree<T>* a) {
            node* q = back;
            while(q->v != a) {
                q = q->prev;
            }
            q->use = !q->use;
            return;
        }

        void print() {
            node* q = front;
            while(q) {
                std::cout<<q->v->value<<" ";
                q = q->next;
            }
        }
    };

    int length_p(nodeTree<T>* q) {
        if(!q) {
            return 0;
        }
        return 1 + length_p(q->left) + length_p(q->right);
    }

public:
    binSTree() {
        first = NULL;
    }

    void insert(nodeTree<T>* q) {
        nodeTree<T>* u = q;
        if(empty()) {
            first = u;
            return;
        }
        nodeTree<T>* g = first;
        while(true) {
            if(u->key > g->key) {
                if(g->right) {
                    g = g->right;
                }
                else {
                    g->right = u;
                    break;
                }
            }
            else if(u->key < g->key) {
                if(g->left) {
                    g = g->left;
                }
                else {
                    g->left = u;
                    break;
                }
            }
            else
            {
                break;
            }
            
        }
    }

    void deleteM(int k) {
        if(first->key == k) {
            if(first->left) {
                nodeTree<T>* q = first->right;
                first = first->left;
                if(q)
                    insert(q);
            }
            else {
                first = first->right;
            }
        }
        else {
            nodeTree<T>* parent = first;
            nodeTree<T>* q;
            if(first->key > k)
                q = first->left;
            else 
                q = first->right;
            while(1) {
                if(!q) {
                    return;
                }
                if(q->key > k) {
                    parent = q;
                    q = q->left;
                }
                if(q->key < k) {
                    parent = q;
                    q = q->right;
                }
                if(q->key == k) {
                    if(q->left) {
                        if(q == parent->left) {
                            parent->left = q->left;
                        }
                        else {
                            parent->right = q->left;
                        }
                        if(q->right) {
                            insert(q->right);
                        }
                    }
                    else if(q->right) {
                        if(q == parent->left) {
                            parent->left = q->right;
                        }
                        else {
                            parent->right = q->right;
                        }
                    }
                    else {
                        if(q == parent->left) {
                            parent->left = nullptr;
                        }
                        else {
                            parent->right = nullptr;
                        }
                    }
                    return;
                }
            }
        }
    }


    nodeTree<T>* find(int k) {
        nodeTree<T>* q = first;
        while(1) {
            if(!q) {
                return NULL;
            }
            if(q->key == k) {
                return q;
            }
            if(q->key > k) {
                q = q->left;
            }
            if(q->key < k) {
                q = q->right;
            }
        }
    }


    bool empty() const {
        if(first)
            return false;
        return true;
    }

    int length() {
        return length_p(first);
    }
    ///////////////////////////////
    void preorder() {
        if(empty())
            return;
        
        list a;
        a.push_back(first);
        nodeTree<T>* q = first;
        while(1) {
            if(q->left && a.back->v == q) {
                a.push_back(q->left);
                q = q->left;
            }
            else if(q->right && !a.find(q)) {
                a.change(q);
                a.push_back(q->right);
                q = q->right;
            }
            else if(!a.find(q)) {
                a.change(q);
            }
            else {
                node* g = a.back;
                while(g->use) {
                    g = g->prev;
                    if(!g)
                        break;
                }
                if(!g)
                    break;
                q = g->v;
            }
        }
        a.print();
    }
    ///////////////////////////////
    void inorder() { // որոնման բինար ծառերի համար այս շրջանցման դեպքում կարող ենք ուղղակի սորտավորել ըստ բանալու
        if(empty())
            return;

        list ans;
        if(length() == 1) {
            ans.push_back(first);
            ans.print();
            return;
        }

        nodeTree<T>* q = first;
        list a;
        while(1) {
            if(q->left) {
                a.push_back(q);
                q = q->left;
                continue;
            }
            else {
                ans.push_back(q);
            }
            if(q->right) {
                q = q->right;
            }
            else {
                
                do {
                    if(a.front)
                        ans.push_back(a.back->v);
                    a.pop_back();
                    q = ans.back->v;
                    q = q->right;
                    if(a.front == NULL)
                        break;
                } while(!q);

                if(!q && a.front == NULL)
                    break;
            }
        }
        ans.print();
    }
    ///////////////////////////////
    void postorder() {
        if(empty())
            return;
        
        list ans;

        if(length() == 1) {
            ans.push_back(first);
            ans.print();
            return;
        }

        list a;
        nodeTree<T>* q = first;

        while(1) {
            if(q->left) {
                a.push_back(q);
                if(!q->right)
                    a.change(q);
                q = q->left;
            }
            else if(q->right) {
                a.push_back(q);
                a.change(q);
                q = q->right;
            }
            else {
                ans.push_back(q);
                node* g = a.back;
                while(g->use) {
                    ans.push_back(g->v);
                    a.pop_back();
                    g = g->prev;
                    if(!g)
                        break;
                }
                if(!g)
                    break;
                q = g->v;
                a.change(q);
                q = q->right;
            }
        }

        ans.print();
    }

};

////////////////////////////////////////////////////////////////////////////////////////////////

using namespace std;

int main() {

    nodeTree<int>* a = new nodeTree<int>(1, 1);
    nodeTree<int>* b = new nodeTree<int>(2, 2);
    nodeTree<int>* c = new nodeTree<int>(3, 3);
    nodeTree<int>* d = new nodeTree<int>(4, 4);
    nodeTree<int>* e = new nodeTree<int>(5, 5);
    nodeTree<int>* f = new nodeTree<int>(6, 6);
    binSTree<int> tree;
    tree.insert(d);
    tree.insert(b);
    tree.insert(a);
    tree.insert(f);
    tree.insert(c);
    tree.insert(e);
    tree.deleteM(2);
    tree.preorder();
    cout<<endl;
    tree.inorder();
    cout<<endl;
    tree.postorder();

    return 0;
}