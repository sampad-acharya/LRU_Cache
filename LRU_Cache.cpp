#include <iostream>
#include <unordered_map>

struct Node{
    Node *prev;
    Node *next;
    int val; 
    int key;
    Node(int node_key, int node_val): prev{nullptr}, next{nullptr}, val(node_val), key{node_key}{};
};
class LRUCache {
    int capacity;
    std::unordered_map<int, Node*> key_dict;
    Node *head;
    Node *tail;

public:
    LRUCache(int capacity): capacity{capacity}{
        head = new Node(-1, -1);
        tail = new Node(-1, -1);
        head->next = tail;
        tail->prev = head;
    }
    
    int get(int key) {
        /*
        if (key_dict.find(key) == key_dict.end()) return -1;
        int result = key_dict[key]->val;
        Node *node_to_be_deleted = key_dict[key];
        delete_node(node_to_be_deleted);
        Node *new_node_to_be_added = new Node(key, result);
        put_node(new_node_to_be_added);
        key_dict[key] = new_node_to_be_added;
        return result;
        */

        if (key_dict.find(key) == key_dict.end()) return -1;
        Node *node_to_be_moved = key_dict[key];
        int result = node_to_be_moved->val;
        delete_node(node_to_be_moved);
        put_node(node_to_be_moved);  // reuse existing node
        return result;
    }
    
    void put(int key, int value) {
        if (key_dict.find(key) == key_dict.end()){
            if (key_dict.size() == capacity){
                Node *node_to_be_deleted = head->next;
                int key_to_be_moved = node_to_be_deleted->key;
                delete_node(node_to_be_deleted);
                key_dict.erase(key_to_be_moved);
                delete node_to_be_deleted;
            }
            key_dict[key] = new Node(key, value);
            put_node(key_dict[key]);
        }else{
            Node *node_to_be_updated = key_dict[key];
            node_to_be_updated->val = value;
            delete_node(node_to_be_updated);
            put_node(key_dict[key]); 
            
        }

    }

    void put_node(Node *node){
        Node *previous_end = tail->prev;
        previous_end->next = node;
        node->next = tail;
        node->prev = previous_end;
        tail->prev = node;

    }
    void delete_node(Node *node){
        Node *node_prev = node->prev;
        node_prev->next = node->next;
        node->next->prev = node->prev;

    }
};




int main() {
    LRUCache cache(2);  // capacity = 2

    cache.put(1, 10);
    cache.put(2, 20);
    std::cout << "Get 1: " << cache.get(1) << "\n";  // returns 10

    cache.put(3, 30);  // evicts key 2
    std::cout << "Get 2: " << cache.get(2) << "\n";  // returns -1 (not found)

    cache.put(4, 40);  // evicts key 1
    std::cout << "Get 1: " << cache.get(1) << "\n";  // returns -1 (not found)
    std::cout << "Get 3: " << cache.get(3) << "\n";  // returns 30
    std::cout << "Get 4: " << cache.get(4) << "\n";  // returns 40

    return 0;
}
