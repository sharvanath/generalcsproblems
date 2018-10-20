#include <dirent.h>
#include <fstream>
#include <list>
#include <map>
#include <string>


#define MAX_MEMTABLE_SIZE (64 * 1024 * 1024)
#define MAX_KEYSIZE 20
#define MAX_OFFSET_DIGITS 10
#define HEADER_ENTRY_SIZE (MAX_KEYSIZE + MAX_OFFSET_DIGITS)
#define HEADER_ENTRY_LINE_SIZE (HEADER_ENTRY_SIZE + 1)
#define MEMTABLE_BASE_NAME "sha-store"
#define SEPERATOR "-"

using std::string;
using std::map;

class MemTable {

 public:
  MemTable(const bool& read_only, const string& dir_path);
  int Flush();
  // int SpaceLeft();
  int Append(const string& key, const string& value);
  int getMaxTableNum();

 private:
  const bool read_only_;
  const string dir_path_;
  map<string, string> key_values_;
  int curr_size_;
};