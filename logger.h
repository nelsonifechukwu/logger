#include <dirent.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#define AMOUNT_IN_BYTES 20

using namespace std;
vector<string> lines;
bool writeToFile(string filename, string data);
void appendEachLineToVector(string eachline);
string readFromFile(string filename);
void getVectorContent(string type = "all", int index = 0);
static bool emptyDirectory(const char *path);
void createNewFile(const char *filename);
void getFileName(const char *path, vector<string> &filenames);
bool isCorrectNaming(vector<string> &filenames);
string lastFile(vector<string> &filenames);
int fileSize(const string &filename);
int extractFIleNumber(const string &filename);