#ifndef filesmanage
#define filesmanage

#include <vector>
#include <string>

std::vector<int> readFromFile(const std::string& filename);
void writeToFile(const std::string& filename, const std::vector<int>& data);

#endif