#ifndef UNTITLED_LIBRARY_H
#define UNTITLED_LIBRARY_H

#define FILL_FACTOR 0.75
#define GROW_RATIO 1.6

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

#include "library.h"

#endif // UNTITLED_LIBRARY_H
