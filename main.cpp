#include <vector>
#include <list>
#include <random>
#include <iostream>
#include <type_traits>
#include <stdexcept>

template <typename RType>
RType generate_random_real_number(RType min_value, RType max_value);

template <typename Val, typename Key>
class Bucket {
public:
    Val value;
    Key key;

    Bucket();
    Bucket(Val value, Key key);
    Val get_value() const;
    void set_value(const Val& value);
    Key get_key() const;
};

template <typename Val, typename Key, typename Container = std::list<Bucket<Val, Key>>>
class Hashmap {
private:
    size_t size;
    std::vector<Container*> hash_data;
    float rand_constant;

    void grow();
    size_t hash_function(Key& key) const;

public:
    Hashmap();
    Hashmap(const Hashmap<Val, Key, Container>& other);
    Hashmap(size_t initial_size, Val min_value, Val max_value, Key min_key, Key max_key);
    ~Hashmap();

    void insert(const Val& value, Key& key);
    void insert_or_assign(const Val& value, const Key& key);
    Bucket<Val, Key>* search(Key& key);
    bool contains(const Val& value) const;
    void erase(const Key& key);
    void print() const;
    size_t count(const Key& key) const;

    Val search_value(Key &key);
};

template <typename RType>
RType generate_random_real_number(RType min_value, RType max_value) {
    std::random_device rd;
    std::mt19937 gen(rd());
    if constexpr (std::is_integral_v<RType>) {
        std::uniform_int_distribution<RType> dist(min_value, max_value);
        return dist(gen);
    } else {
        std::uniform_real_distribution<RType> dist(min_value, max_value);
        return dist(gen);
    }
}

template <typename Val, typename Key>
Bucket<Val, Key>::Bucket() : value(Val()), key(Key()) {}

template <typename Val, typename Key>
Bucket<Val, Key>::Bucket(Val value, Key key) : value(value), key(key) {}

template <typename Val, typename Key>
Val Bucket<Val, Key>::get_value() const {
    return value;
}

template <typename Val, typename Key>
void Bucket<Val, Key>::set_value(const Val& value) {
    this->value = value;
}

template <typename Val, typename Key>
Key Bucket<Val, Key>::get_key() const {
    return key;
}


