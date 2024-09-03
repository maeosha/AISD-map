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

template <typename Val, typename Key, typename Container>
Hashmap<Val, Key, Container>::Hashmap() : size(0), hash_data(32, nullptr) {}

template <typename Val, typename Key, typename Container>
Hashmap<Val, Key, Container>::Hashmap(const Hashmap<Val, Key, Container>& other)
        : size(other.size), hash_data(other.hash_data.size(), nullptr), rand_constant(other.rand_constant) {
    for (size_t i = 0; i < other.hash_data.size(); ++i) {
        if (other.hash_data[i]) {
            hash_data[i] = new Container(*other.hash_data[i]);
        }
    }
}

template <typename Val, typename Key, typename Container>
Hashmap<Val, Key, Container>::Hashmap(size_t initial_size, Val min_value, Val max_value, Key min_key, Key max_key) {
    if (initial_size == 0) throw std::invalid_argument("Size = 0");
    hash_data.resize(initial_size, nullptr);
    for (size_t i = 0; i < initial_size; ++i) {
        Val rand_value = generate_random_real_number(min_value, max_value);
        Key rand_key = generate_random_real_number(min_key, max_key);
        insert(rand_value, rand_key);
    }
}

template <typename Val, typename Key, typename Container>
Hashmap<Val, Key, Container>::~Hashmap() {
    for (auto& bucket_list_ptr : hash_data) {
        delete bucket_list_ptr;
    }
    hash_data.clear();
}

template <typename Val, typename Key, typename Container>
void Hashmap<Val, Key, Container>::insert(const Val& value, Key& key) {
    size_t hashed_key = hash_function(key);

    if (!hash_data[hashed_key]) {
        hash_data[hashed_key] = new Container;
    }

    for (const auto& bucket : *hash_data[hashed_key]) {
        if (bucket.get_key() == key) {
            return;
        }
    }

    hash_data[hashed_key]->emplace_back(value, key);
    ++size;
}

template <typename Val, typename Key, typename Container>
void Hashmap<Val, Key, Container>::insert_or_assign(const Val& value, const Key& key) {
    size_t hashed_key = hash_function(key, hash_data.size());
    if (!hash_data[hashed_key]) {
        hash_data[hashed_key] = new Container;
    }

    for (auto& bucket : *hash_data[hashed_key]) {
        if (bucket.get_key() == key) {
            bucket.set_value(value);
            return;
        }
    }
    insert(value, key);
}

template <typename Val, typename Key, typename Container>
Bucket<Val, Key>* Hashmap<Val, Key, Container>::search(Key& key) {
    size_t hashed_key = hash_function(key);
    if (hash_data[hashed_key]) {
        for (auto& bucket : *hash_data[hashed_key]) {
            if (bucket.get_key() == key) {
                return &bucket;
            }
        }
    }
    return nullptr;
}

template <typename Val, typename Key, typename Container>
Val Hashmap<Val, Key, Container>::search_value(Key& key){
    size_t hashed_key = hash_function(key);
    if (hash_data[hashed_key]) {
        for (auto& bucket : *hash_data[hashed_key]) {
            if (bucket.get_key() == key) {
                return bucket.get_value();
            }
        }
    }
}

template <typename Val, typename Key, typename Container>
bool Hashmap<Val, Key, Container>::contains(const Val& value) const {
    for (const auto& bucket_list_ptr : hash_data) {
        if (bucket_list_ptr) {
            for (const auto& bucket : *bucket_list_ptr) {
                if (bucket.get_value() == value) {
                    return true;
                }
            }
        }
    }
    return false;
}

template <typename Val, typename Key, typename Container>
void Hashmap<Val, Key, Container>::erase(const Key& key) {
    size_t hashed_key = hash_function(key, hash_data.size());
    if (hash_data[hashed_key]) {
        auto* bucket_list = hash_data[hashed_key];
        for (auto it = bucket_list->begin(); it != bucket_list->end(); ++it) {
            if (it->get_key() == key) {
                bucket_list->erase(it);
                --size;
                if (bucket_list->empty()) {
                    delete bucket_list;
                    hash_data[hashed_key] = nullptr;
                }
                return;
            }
        }
    }
}

template <typename Val, typename Key, typename Container>
void Hashmap<Val, Key, Container>::print() const {
    std::cout << "{";
    bool first = true;
    for (const auto& bucket_list_ptr : hash_data) {
        if (bucket_list_ptr) {
            for (const auto& bucket : *bucket_list_ptr) {
                if (!first) {
                    std::cout << ", ";
                }
                first = false;
                std::cout << bucket.get_key() << ": " << bucket.get_value();
            }
        }
    }
    std::cout << "}" << std::endl;
}

template <typename Val, typename Key, typename Container>
size_t Hashmap<Val, Key, Container>::count(const Key& key) const {
    size_t hashed_key = hash_function(key, hash_data.size());
    return hash_data[hashed_key] ? hash_data[hashed_key]->size() : 0;
}

template <typename Val, typename Key, typename Container>
void Hashmap<Val, Key, Container>::grow() {
    size_t new_size = static_cast<size_t>(size * GROW_RATIO);
    std::vector<Container*> new_hash_data(new_size, nullptr);

    for (auto& bucket_list_ptr : hash_data) {
        if (bucket_list_ptr) {
            for (auto& bucket : *bucket_list_ptr) {
                size_t new_hashed_key = hash_function(bucket.key, new_size);
                if (!new_hash_data[new_hashed_key]) {
                    new_hash_data[new_hashed_key] = new Container;
                }
                new_hash_data[new_hashed_key]->push_back(bucket);
            }
            delete bucket_list_ptr;
        }
    }
}

template <typename Val, typename Key, typename Container>
size_t Hashmap<Val, Key, Container>::hash_function(Key& key) const {
    float hashed_key = static_cast<int>(key) * rand_constant;
    return static_cast<size_t>(hashed_key * hash_data.size());
}

int translation_roman_num(std::string roman_num){
    auto map = Hashmap<int, char>();
    map.insert(1, (char &) "I");
    map.insert(5, (char &)"V");
    map.insert(10, (char &)"X");
    map.insert(50, (char &)"L");
    map.insert(100, (char &)"C");
    map.insert(500, (char &)"D");
    map.insert(1000, (char &)"M");

    int sum = 0;

    for (size_t index = 0; index < roman_num.size() - 1; index++){
        if (map.search_value(roman_num[index]) < map.search_value(roman_num[index + 1])){
            sum -= map.search_value(roman_num[index]);
        }

        else{
            sum += map.search_value(roman_num[index]);
        }
    }

    sum += map.search_value(roman_num[roman_num.size() - 1]);
    return sum;
}

int main() {
    std::cout << translation_roman_num("XIX");
}