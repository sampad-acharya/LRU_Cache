#include <iostream>
#include <unordered_map>
#include <optional>

template<typename K, typename V>
struct Node {
    Node *prev;
    Node *next;
    V val;
    K key;
    Node(K node_key, V node_val)
        : prev{nullptr}, next{nullptr}, val(node_val), key{node_key} {}
};

template<typename K, typename V>
class LRUCache {
    int capacity;
    std::unordered_map<K, Node<K, V>*> key_dict;
    Node<K, V> *head;
    Node<K, V> *tail;

public:
    LRUCache(int capacity) : capacity{capacity} {
        head = new Node<K, V>(K{}, V{});
        tail = new Node<K, V>(K{}, V{});
        head->next = tail;
        tail->prev = head;
    }

    std::optional<V> get(K key) {
        if (key_dict.find(key) == key_dict.end()) return std::nullopt;
        Node<K, V> *node_to_be_moved = key_dict[key];
        V result = node_to_be_moved->val;
        delete_node(node_to_be_moved);
        put_node(node_to_be_moved);
        return result;
    }

    void put(K key, V value) {
        if (key_dict.find(key) == key_dict.end()) {
            if (key_dict.size() == capacity) {
                Node<K, V> *node_to_be_deleted = head->next;
                K key_to_be_moved = node_to_be_deleted->key;
                delete_node(node_to_be_deleted);
                key_dict.erase(key_to_be_moved);
                delete node_to_be_deleted;
            }
            key_dict[key] = new Node<K, V>(key, value);
            put_node(key_dict[key]);
        } else {
            Node<K, V> *node_to_be_updated = key_dict[key];
            node_to_be_updated->val = value;
            delete_node(node_to_be_updated);
            put_node(key_dict[key]);
        }
    }

private:
    void put_node(Node<K, V> *node) {
        Node<K, V> *previous_end = tail->prev;
        previous_end->next = node;
        node->next = tail;
        node->prev = previous_end;
        tail->prev = node;
    }

    void delete_node(Node<K, V> *node) {
        Node<K, V> *node_prev = node->prev;
        node_prev->next = node->next;
        node->next->prev = node_prev;
    }
};

int main() {
    LRUCache<int, int> cache(2);  // Create a cache with capacity 2

    cache.put(1, 100);
    cache.put(2, 200);

    auto val1 = cache.get(1);
    if (val1) {
        std::cout << "Get 1: " << val1.value() << "\n";  // Output: 100
    } else {
        std::cout << "Get 1: not found\n";
    }

    cache.put(3, 300);  // Evicts key 2

    auto val2 = cache.get(2);
    std::cout << "Get 2: " << (val2 ? std::to_string(val2.value()) : "not found") << "\n";  // Output: not found

    cache.put(4, 400);  // Evicts key 1

    auto val3 = cache.get(1);
    auto val4 = cache.get(3);
    auto val5 = cache.get(4);

    std::cout << "Get 1: " << (val3 ? std::to_string(val3.value()) : "not found") << "\n";  // Output: not found
    std::cout << "Get 3: " << (val4 ? std::to_string(val4.value()) : "not found") << "\n";  // Output: 300
    std::cout << "Get 4: " << (val5 ? std::to_string(val5.value()) : "not found") << "\n";  // Output: 400

    return 0;
}
