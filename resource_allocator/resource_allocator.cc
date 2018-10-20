#include "resource_allocator.h"

#include <iostream>

ResourceAllocator::ResourceAllocator(int total) {
  inUse = new bool[total];
  this->total = total;
  for (int i = 0; i < total; ++i) {
    inUse[i] = false;
  }
}

int ResourceAllocator::AllocateResources(int amount) {
  int last_streak = 0;
  for (int i = 0; i < total; ++i) {
   if(!inUse[i]) {
     ++last_streak;
   } else {
     last_streak = 0;
   }

   if (last_streak == amount) {
     for (int j = i; j >= i - last_streak + 1; --j) {
       inUse[j] = true;
     }
     return i - last_streak + 1;
   }
  }
  return -1;
}

void ResourceAllocator::FreeResources(int index, int amount) {
  for (int i = index; i < index + amount; ++i) {
    inUse[i] = false;
  }
}

int main() {
  ResourceAllocator r(10);
  std::cout << r.AllocateResources(5) << std::endl;
  std::cout << r.AllocateResources(3) << std::endl;
  r.FreeResources(0, 5);
  std::cout << r.AllocateResources(8) << std::endl;
  return 0;
}
