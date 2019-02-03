#include<algorithm>
#include<iostream>
#include<list>
#include<map>
#include<utility>
#include<complex>
#include<tuple>
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
  std::list<K> recency_list_;
  std::map<K, std::pair<V, typename std::list<K>::iterator>> hash_cache_;
};

template<class K, class V>
void LruCache<K, V>::Add(K key, V value) {
  if (recency_list_.size() == this->size_) {
    K pop_key = recency_list_.front();
    recency_list_.pop_front();
    hash_cache_.erase(hash_cache_.find(pop_key));
  }
  recency_list_.push_back(key);
  hash_cache_[key].first = value;
  hash_cache_[key].second = --recency_list_.end();
}

template<class K, class V>
V* LruCache<K, V>::GetIfExists(K key) {
  auto it = hash_cache_.find(key);
  if (it == hash_cache_.end()) {
    return NULL;
  } else {
    auto listIter = it->second.second;
    recency_list_.erase(listIter);
    recency_list_.push_back(key);
    it->second.second = --recency_list_.end();
    return &(it->second.first);
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

  // Touch them in decreasing order
  for (int i = 2*n-1; i >= n; i--) {
    if (!x.GetIfExists(i)) {
      std::cout << "Key " << i << " does not exist"
      << " it should've been";
      return 1;
    }
  }

  // 2*n-1 was last touched hence should be evicted
  x.Add(5*n, 1);
  assert(!x.GetIfExists(2*n-1));

  // Touch 2*n-2 and force eviction of someone
  x.GetIfExists(2*n-2);
  x.Add(5*n+1, 1);

  // 2*n-2 should not be evicted since we will touch it
  // instead 2*n-3 will be evicted
  assert(x.GetIfExists(2*n-2));
  assert(!x.GetIfExists(2*n-3));

  std::cout << "No errors encountered in the test cases!" << std::endl;
  // check if access brings it to front
  return 0;
}
