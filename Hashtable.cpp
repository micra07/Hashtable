#include <iostream>
#include <vector>
#include<list>
#include <stdexcept>
#include <random>

enum class roman_nums { I = 1, V = 5, X = 10, L = 50, C = 100, D = 500, M = 1000 };

template <typename K, typename V>
class HashTable {
    struct Pair {
        K key;
        V value;
        Pair* next;
    };

    std::vector<Pair*> _data;
    size_t _size;

public:
    HashTable(size_t size) : _size(size) {
        if (!size) throw std::invalid_argument("size = 0");
        _data.resize(size);
    }

    HashTable(size_t size, const V& min, const V& max) :_size(size) {
        if (!size) throw std::invalid_argument("size = 0");
        _data.resize(size);
        for (size_t i = 0; i < size; ++i) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist(min, max);
            _data[i] = new Pair{ static_cast<K>(i), dist(gen), nullptr };
        }
    }

    HashTable(const HashTable& other) : _size(other._size) {
        _data.resize(other._size);
        for (size_t i = 0; i < other._size; ++i) {
            Pair* current = other._data[i]; 
            Pair* prev = nullptr; 
            while (current) {
                Pair* newPair = new Pair{ current->key, current->value, nullptr };
                if (!prev) {
                    _data[i] = newPair;
                }
                else {
                    prev->next = newPair; 
                }
                prev = newPair; 
                current = current->next;
            }
        }
    }

    void swap(HashTable& other) {
        std::swap(_data, other.data);
        std::swap(_size, other._size);
    }

    size_t hash(const K& key) const {
        return key % _size;
    }

    HashTable& operator=(HashTable other) {
        swap(other);
        return *this;
    }

    ~HashTable() {
        for (size_t i = 0; i < _size; i++) {
            Pair* current = _data[i];
            while (current) {
                Pair* tmp = current;
                current = current->next;
                delete tmp;
            }
        }
    }

    size_t get_size() const {
        return _size;
    }

    void print() const {
        for (size_t i = 0; i < _size; ++i) {
            Pair* current = _data[i];
            while (current) {
                std::cout << current->key << ": [" << current->value << "]";
                if (current->next) std::cout << ", ";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }

    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        Pair* current = _data[index];
        while (current) {
            if (current->key == key) throw std::invalid_argument("key is not unique");
            current = current->next;
        }
        Pair* newPair = new Pair{ key, value, nullptr };
        newPair->next = _data[index];
        _data[index] = newPair;
    }

    void insert_or_assign(const K& key, const V& value) {
        size_t index = hash(key);
        Pair* current = _data[index];
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        Pair* newPair = new Pair{ key, value, nullptr };
        newPair->next = _data[index];
        _data[index] = newPair;
    }

    bool erase(const K& key) {
        size_t index = hash(key); 
        Pair* current = _data[index]; 
        Pair* prev = nullptr;
        while (current) { 
            if (current->key == key) { 
                if (prev)
                    prev->next = current->next; 
                else
                    _data[index] = current->next; 
                delete current; 
                return true; 
            }
            prev = current;
            current = current->next; 
        }
        return false; 
    }


    bool contains(const V& value) const {
        for (size_t i = 0; i < _size; ++i) {
            Pair* current = _data[i];
            while (current) {
                if (current->value == value)
                    return true;
                current = current->next;
            }
        }
        return false;
    }

    V* search(const K& key) {
        size_t index = hash(key);
        Pair* tmp = _data[index];
        while (tmp) {
            if (tmp->key == key)
                return &(tmp->value);
            tmp = tmp->next;
        }
        throw std::invalid_argument("key is not exist");
    }

    /*int count(const K& key) {
        size_t index = hash(key);
        Pair* tmp = _data[index];
        while (tmp) {
            if (tmp->key == key)
                return 1;
            tmp = tmp->next;
        }
        return 0;
    }*/

    size_t count(const K& key) {
        size_t index = hash(key);
        int count = 0;
        Pair* tmp = _data[index];
        while (tmp) {
            ++count;
            tmp = tmp->next;
        }
        return count;
    }
};

size_t hash(const std::string& roman) {
    size_t result = 0, prev = 0; 
    for (int i = roman.length() - 1; i >= 0; i--) {
        char symbol = roman[i]; 
        int cur = 0; 
        switch (symbol) { 
        case 'I': cur = static_cast<int>(roman_nums::I); break;
        case 'V': cur = static_cast<int>(roman_nums::V); break;
        case 'X': cur = static_cast<int>(roman_nums::X); break;
        case 'L': cur = static_cast<int>(roman_nums::L); break;
        case 'C': cur = static_cast<int>(roman_nums::C); break;
        case 'D': cur = static_cast<int>(roman_nums::D); break;
        case 'M': cur = static_cast<int>(roman_nums::M); break;
        default:
            throw std::invalid_argument(""); 
        }
        if (cur < prev)
            result -= cur;
        else result += cur;
        prev = cur; 
    }
    return result;
}


int main() {
    HashTable<int, int> hashTable(6, 2, 9);
    hashTable.insert(9, 15);
    hashTable.print();
    std::cout << "\n";
    //hashTable.insert(9, 25);
    hashTable.insert_or_assign(10, 4);
    hashTable.print();
    std::cout << "\n";
    hashTable.insert_or_assign(10, 6);
    hashTable.print();
    std::cout << "\n";
    hashTable.insert_or_assign(4, 43);
    hashTable.print();
    std::cout << "\n";
    std::cout << "Checking that there is a value: " << hashTable.contains(43) << "\n";
    std::cout << "Checking that there is a value: " << hashTable.contains(23232) << "\n";
    std::cout << "Counting the number of emails with keys that are and are not in the table for key 4: " << hashTable.count(4) << "\n";
    std::cout << "Counting the number of emails with keys that are and are not in the table for key 10: " << hashTable.count(9) << "\n";
    std::cout << "Counting the number of emails with keys that are and are not in the table for key 0: " << hashTable.count(0) << "\n";
    std::cout << "Searching for a value by a given key :" << *hashTable.search(4) << "\n";
    //std::cout << "Searching for a value by a given key :" << *hashTable.search(11) << "\n";
    std::cout << std::endl;
    std::cout << "Original :" << "\n";
    hashTable.print();
    std::cout << std::endl;
    std::cout << "Checking the work of the constructor :" << "\n";
    HashTable<int, int> copy(hashTable);
    copy.print();
    std::cout << std::endl;
    std::cout << "Checking the work of the operator :" << "\n";
    HashTable<int, int> copy1 = hashTable;
    copy1.print();
    std::cout << std::endl;
    std::cout << "Checking the work of the erase :";
    copy.erase(9);
    copy.erase(9);
    copy.erase(0);
    copy.print();
    std::cout << std::endl;
    std::cout << "Task :";
    std::cout << "MMMDCCLXXII: " << hash("MMMDCCLXXII") << std::endl;
    std::cout << "MCCCLXXV: " << hash("MCCCLXXV") << std::endl;
    std::cout << "MMMMMDCCXLIII: " << hash("MMMMMDCCXLIII") << std::endl;
    std::cout << "Testing rubish: " << hash("IIX") << std::endl;
    std::cout << "CCV: " << hash("CCV") << std::endl;
    std::cout << "XX: " << hash("XX") << std::endl;

    return 0;
}