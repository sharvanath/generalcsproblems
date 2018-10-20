class ResourceAllocator {
 public:
  // Initialize allocator with "total" items,
  // index 0 to total-1
  ResourceAllocator(int total);

  // Allocate contigous sequence of "amount" resources
  // and return the index of first resource.
  int AllocateResources(int amount);

  // Frees up amount resources starting at index
  void FreeResources(int index, int amount);

 private:
  bool *inUse;
  int total;
};
