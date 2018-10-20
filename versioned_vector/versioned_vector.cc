#include "versioned_vector.h"

#include <algorithm>
#include <iostream>

void VersionedVector::Set(int index, int value) {
  if (index < 0 || index >= values_.size()) {
    std::cerr << "Index " << index << " out of bounds.\n";
    return;
  }

  values_[index] = value;
  changed_.insert(index);
}

int VersionedVector::Get(int index) const {
  return values_.at(index);
}

int VersionedVector::TakeSnapshot() {
  for (const int& idx : changed_) {
    snapshots_[idx].push_back({snapshot_idx_, values_[idx]});
  }

  changed_.clear();
  return snapshot_idx_++;
}

int VersionedVector::Get(int index, int snapshot) {
  if (snapshot < 0 || snapshot >= snapshot_idx_) {
    std::cerr << "Error snapshot " << snapshot << "\n";
  }

  VersionedInt val = {snapshot, 0};

  auto it = std::lower_bound(
      snapshots_[index].begin(), snapshots_[index].end(), val,
      [](const VersionedInt& left, const VersionedInt& right) {
    return left.version < right.version;
  });

  if (it == snapshots_[index].end()) {
    return values_[index];
  }

  return it->value;
}

int main() {
 std::cout << "Hello World!\n";
 VersionedVector x(5);
 x.Set(1, 1);
 int snap = x.TakeSnapshot();
 x.Set(1, 5);
 int snap1 = x.TakeSnapshot();
 x.Set(1, 6);

 std::cout << x.Get(1, snap) << "\n";
 std::cout << x.Get(1, snap1) << "\n";
 std::cout << x.Get(1) << "\n";
}
