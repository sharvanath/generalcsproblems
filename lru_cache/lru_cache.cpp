#include<algorithm>
#include<iostream>
#include<list>
#include<map>
#include<cassert>

using std::cout;
using std::list;
using std::map;

template<class K, class V>
class Cache {
 public:
  Cache(int size) : size_(size) {}
  virtual void Add(K key, V value) = 0;
  virtual V* GetIfExists(K key) = 0;

 protected:
  int size_;
};

template<class K, class V>
class LruCache : public Cache<K, V> {
 public:
  using Cache<K, V>::Cache;
  void Add(K key, V value) final;
  V* GetIfExists(K key) final;

 private:
  std::map<K, V> hash_cache_;
  std::list<K> recency_list_;
};

template<class K, class V>
void LruCache<K, V>::Add(K key, V value) {
  if (recency_list_.size() == this->size_) {
    K pop_key = recency_list_.front();
    recency_list_.pop_front();
    hash_cache_.erase(hash_cache_.find(pop_key));
  }
  recency_list_.push_back(key);
  hash_cache_[key] = value;
}

template<class K, class V>
V* LruCache<K, V>::GetIfExists(K key) {
  typename std::map<K, V>::iterator it = hash_cache_.find(key);
  if (it == hash_cache_.end()) {
    return NULL;
  } else {
    typename std::list<V>::iterator findIter = std::find(
      recency_list_.begin(), recency_list_.end(), key);
    assert (findIter != recency_list_.end());
    recency_list_.erase(findIter);
    recency_list_.push_back(key);
    return &(it->second);
  }
}

int main() {
  //
  int n = 5;
  LruCache<int, int> x(n);
  for (int i = 0; i < (2*n); i++) {
    x.Add(i, 0);
  }
  for (int i = 0; i < n; i++) {
    if (x.GetIfExists(i)) {
      std::cout << "Key " << i << " exists, while it should've been cleaned";
      return 1;
    }
  }
  for (int i = 2*n-1; i >= n; i--) {
    if (!x.GetIfExists(i)) {
      std::cout << "Key " << i << " does not exist"
      << " it should've been";
      return 1;
    }
  }
  for (int i = 0; i < (n/2); i++) {
    x.Add(i, 0);
  }

  for (int i = 0; i < (2*n); i++) {
    int * ptr = x.GetIfExists(i);
    if (ptr)
      std::cout << "Got " << i << "\n";
  }

  // check if access brings it to front
  return 0;
}
