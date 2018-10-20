#include <set>
#include <vector>

class VersionedVector {
 public:
  VersionedVector(int size) : snapshot_idx_(0) {
    snapshots_.resize(size);
    values_.resize(size);
  }

  void Set(int index, int value);
  int Get(int index) const;

  int TakeSnapshot();
  int Get(int index, int snapshot);

 private:
  struct VersionedInt {
    int version;
    int value;
  };

  std::vector<std::vector<VersionedInt>> snapshots_;
  std::vector<int> values_;
  std::set<int> changed_;
  int snapshot_idx_;
};
