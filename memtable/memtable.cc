#include <algorithm>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "memtable.h"

using std::fstream;
using std::string;
using std::map;

MemTable::MemTable(const bool& read_only,
                   const string& dir_path)
    : read_only_(read_only), dir_path_(dir_path), curr_size_(0) {}

int MemTable::Append(const string& key, const string& value) {
  if (read_only_) {
    errno = EPERM;
    return -1;
  }

  int extra = HEADER_ENTRY_LINE_SIZE + value.size() + 1;
  if (extra + curr_size_ >= MAX_MEMTABLE_SIZE) {
    errno = EFBIG;
    return -1;
  }

  key_values_[key] = value;
  curr_size_ += extra;
  std::cout << "new size : " << curr_size_ << std::endl;
  return 0;
}

int MemTable::Flush() {
  int new_num = getMaxTableNum() + 1;
  string filename =  dir_path_ + "/" + string(MEMTABLE_BASE_NAME)
                     + string(SEPERATOR) + std::to_string(new_num);
  std::cout << "Creating new memtable : " << filename << std::endl;
  fstream table_file(filename, std::fstream::out);

  if (!table_file.is_open()) {
    std::cout << "Error creating table file" << filename << std::endl;
    return -1;
  }

  int index_size = (HEADER_ENTRY_LINE_SIZE) * (key_values_.size() + 1);
  int curr_offset = index_size;
  table_file.width(HEADER_ENTRY_SIZE);
  table_file << key_values_.size() << "\n";
  for (const auto& kv_pair: key_values_) {
    table_file.width(HEADER_ENTRY_SIZE);
    table_file << kv_pair.first + " " + std::to_string(curr_offset);
    table_file << std::endl;
    curr_offset += kv_pair.second.size() + 1;
  }

  for (const auto& p: key_values_) {
    table_file << p.second << "\n";
  }

  table_file.flush();
  return 0;
}

int MemTable::getMaxTableNum() {
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir(dir_path_.c_str())) == NULL) {
    perror(("Failed while opening memtable dir " + dir_path_).c_str());
    errno = ENOENT;
    return -1;
  }

  int max_num = -1;
  while ((ent = readdir(dir)) != NULL) {
    string curr_file_name = ent->d_name;
    std::size_t index_of_sep = curr_file_name.find(SEPERATOR);
    std::size_t length_of_int = curr_file_name.size() - 1 - index_of_sep;
    if (index_of_sep == string::npos ||
        curr_file_name.substr(0, index_of_sep) != MEMTABLE_BASE_NAME) {
      printf("Undentified file %s found in memtable dir %s\n",
             curr_file_name.c_str(), dir_path_.c_str());
      continue;
    }
    // this is not perfect
    int number = stoi(curr_file_name.substr(index_of_sep, length_of_int));
    max_num = std::max(max_num, number);
  }

  return max_num;
}

int main() {
  MemTable x(false, "db");
  x.Append("sharva","sharva");
  x.Append("sharva1","sharva1");
  x.Flush();
}