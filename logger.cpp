#include "logger.h"

int main(int argc, char **argv) {
  string data = "1,2,4,4,5,6,\n7,8,9,10,11,12,\n13,14,15,16,17,4,\n25,26,27,28,"
                "29,30,\n7,8,9,10,11,12,\n31,32,33,34,35,";
  const char *directory = "./";
  const char *newfile = "data_1.csv";
  while (true) {
    // check if files exist in the directory
    if (!emptyDirectory(directory)) {
      // initialize vector to store all filenames
      vector<string> filenames;
      getFileName(directory, filenames);
      // sort the filenames
      sort(filenames.begin(), filenames.end());
      //print all filenames
      for (int i = 0; i < filenames.size(); i++) {
        cout << filenames[i] << '\n';
      }
      cout << '\n';
      // check if naming convention is correct
      if (isCorrectNaming(filenames)) {
        string last_file = lastFile(filenames);
        // check if fileSize is within threshold
        if (fileSize(last_file) < AMOUNT_IN_BYTES) {
          bool success = writeToFile(last_file, data);
        } else {
          int inc = extractFIleNumber(last_file) + 1;
          string nextfile_str =
              "data_" + to_string(inc) + ".csv"; // after data_
          const char *nextfile = nextfile_str.c_str();
          createNewFile(nextfile);
          bool success = writeToFile(nextfile, data);
        }
      } else {
        cout << "The naming convention is wrong; clear the disk \n  Creating "
                "new file"
             << '\n';
        createNewFile(newfile);
      }
    } else {
      cout << "Directory has no file... Creating new file: " << newfile << '\n';
      createNewFile(newfile);
    }

    break;
  }
  cout << "Success!" << '\n';
  // Read from files
  //  string output = readFromFile(filename);
  //  cout << output << '\n';
  //  getVectorContent("each", 5);
  return 0;
}

int fileSize(const string &filename) {
  ifstream file(filename, ios::binary | ios::ate);
  if (file.is_open()) {
    // Get the size by checking the position of the file pointer
    streamsize size = file.tellg();

    // Seek back to the beginning of the file
    file.seekg(0, ios::beg);

    // Count newline characters
    int newlineCount = {0};
    string line;
    while (getline(file, line))
      ++newlineCount;
    // Subtract the newline characters from the total size
    size -= newlineCount;
    file.close();
    cout <<filename<< " is: " << static_cast<int>(size) << " bytes"<< '\n';
    return static_cast<int>(size);
  } else {
    throw runtime_error("File Couldn't be opened'");
  }
}

bool writeToFile(string filename, string data) {
  ofstream myfile(filename);
  if (myfile.is_open()) {
    myfile << data;
    myfile.close();
    return true;
  } else {
    return false;
  }
} // implement the next function here

void appendEachLineToVector(string eachline) { lines.push_back(eachline); }

void getVectorContent(string type, int index) {
  if (type == "all") {
    for (vector<string>::iterator i = lines.begin(); i != lines.end();
         ++i) {
      cout << *i << '\n';
    }
  } else if (type == "each") {
    if (index < lines.size()) {
      cout << lines[index] << '\n';
    }

    else
      throw runtime_error("Index out of range");
  }
}

string readFromFile(string filename) {
  ifstream file(filename);
  string line;
  string out;
  if (file.is_open()) {
    while (getline(file, line)) {
      appendEachLineToVector(line);
      out.append(line + "\n");
    }
    file.close();
    return out;
  }

  else
    throw runtime_error("Unable to open file: " + filename);
}

static bool emptyDirectory(const char *path) {
  struct dirent *entry;
  DIR *dir = opendir(path);
  if (dir == NULL) {
    return false;
  }
  while ((entry = readdir(dir)) != nullptr) {
    if (entry->d_type != DT_DIR) { // If entry is not a directory, it's a file
      closedir(dir);
      return false;
    }

    // If entry is a directory, check its name
    if (string(entry->d_name) != "." && string(entry->d_name) != "..") {
      closedir(dir);
      return false;
    }
  }
  closedir(dir);
  return true;
}

void createNewFile(const char *filename) {
  ofstream outFile(filename);

  // Check if the file was successfully created and opened
  if (!outFile) {
    cerr << "Error creating file: " << filename << '\n';
    throw runtime_error("File not created");
  }

  // Close the file immediately
  outFile.close();
  cout << "File created successfully: " << filename << '\n';
}

void getFileName(const char *path, vector<string> &filenames) {
  struct dirent *entry;
  DIR *dir = opendir(path);
  if (dir == NULL) {
    throw runtime_error("Can't Open directory to ");
  }
  while ((entry = readdir(dir)) != NULL) {
    filenames.push_back(entry->d_name);
  }
  cout << "All filenames gotten" << '\n';
}

bool isCorrectNaming(vector<string> &filenames) {
  string last_file = filenames[filenames.size() - 4]; //the number 4 indicates the lastfile. It should be 1 if the directory only contains files of type data_(n).csv not ".h", "." , "..", otherwise, manipulate it.
  if (last_file.substr(0, 5) == "data_") {
    return true;
  } else {
    return false;
  }
}
string lastFile(vector<string> &filenames) {
  int max = 0;
  for (int i = 0; i < filenames.size(); i++) {
    string filename = filenames[i];
    // only extract filenumber for valid file format
    if (filename.substr(0, 5) == "data_") {
      int filenumber = extractFIleNumber(filename);
      if (filenumber > max)
        max = filenumber;
    }
  }
  return "data_" + to_string(max) + ".csv";
}

int extractFIleNumber(const string &filename) {
  size_t underscorePos = filename.find('_');
  size_t dotPos = filename.find('.');

  // Extract the substring between the underscore and the period
  // N.B: the second parameter to .substr is the length of the substr to extract
  string numberStr =
      filename.substr(underscorePos + 1, dotPos - underscorePos - 1);
  // Convert the substring to an integer
  return stoi(numberStr);
}