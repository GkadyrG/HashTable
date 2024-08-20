#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <functional>

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashTable {
public:
    HashTable(size_t size = 10) : m_size(size), m_hash(Hash()) {
        m_data.resize(m_size);
    }

    void insert(const Key& key, const Value& value) {
        size_t index = hash(key);
        for (const auto& pair : m_data[index]) {
            if (pair.first == key) {
                pair.second = value; // Обновление существующего значения
                return;
            }
        }
        m_data[index].emplace_back(key, value); // Добавление новой пары
        ++m_count;
        if (m_count > m_size * 0.75) { // Расширение таблицы при достижении 75% заполнения
            rehash(m_size * 2);
        }
    }

    Value get(const Key& key) const {
        size_t index = hash(key);
        for (const auto& pair : m_data[index]) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        throw std::runtime_error("Key not found");
    }

    void remove(const Key& key) {
        size_t index = hash(key);
        auto it = m_data[index].begin();
        while (it != m_data[index].end()) {
            if (it->first == key) {
                m_data[index].erase(it);
                --m_count;
                return;
            }
            ++it;
        }
        throw std::runtime_error("Key not found");
    }

    bool contains(const Key& key) const {
        size_t index = hash(key);
        for (const auto& pair : m_data[index]) {
            if (pair.first == key) {
                return true;
            }
        }
        return false;
    }

    size_t size() const {
        return m_size;
    }

    size_t count() const {
        return m_count;
    }

    void clear() {
        m_data.clear();
        m_data.resize(m_size);
        m_count = 0;
    }

private:
    size_t hash(const Key& key) const {
        return m_hash(key) % m_size;
    }

    void rehash(size_t new_size) {
        std::vector<std::list<std::pair<Key, Value>>> new_data(new_size);
        for (const auto& bucket : m_data) {
            for (const auto& pair : bucket) {
                size_t index = hash(pair.first);
                new_data[index].emplace_back(pair);
            }
        }
        m_data = std::move(new_data);
        m_size = new_size;
        m_count = 0;
    }

    std::vector<std::list<std::pair<Key, Value>>> m_data;
    size_t m_size;
    size_t m_count = 0;
    Hash m_hash;
};

int main() {}