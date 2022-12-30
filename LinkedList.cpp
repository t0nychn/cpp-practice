// Online C++ compiler to run C++ program online
#include <iostream>
#include <vector>
using namespace std;

struct Node{
    int val;
    Node* next;
    Node(int v) : val(v), next(nullptr) {}
    Node(int v, Node* ptr) : val(v), next(ptr) {}
};

void input(int &val){
    cout << "Enter next number (-1 to stop): ";
    cin >> val;
    cout << endl;
    return;
}

void makeList(Node* head){
    Node* current = head;
    while (true){
        int val;
        input(val);
        if (val == -1)
            return;
        Node* next = new Node(val);
        current->next = next;
        current = next;
    }
}

void deleteList(Node* &head){
    Node* current = head;
    head = nullptr;
    while (current){
        Node* next = current->next;
        delete current;
        current = next;
    }
}

int main() {
    int val;
    input(val);
    Node* head = new Node(val);
    makeList(head);
    cout << "Head val: " << head->val << "\nNext val: " << head->next->val << endl;
    deleteList(head);
    cout << "Deleted, testing deletion (should produce seg fault)" << endl;
    cout << "This should not print: " << head->val << endl;
    return 0;
}
