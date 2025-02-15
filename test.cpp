#include <iostream>
#include <fstream>
#include <string>
#include <map>

class FileReader {
private:
    std::ifstream myFile;
    std::string fileContent;

public:
    FileReader(const std::string& filename) : myFile(filename.c_str()) {
        if (myFile.is_open()) {
            std::string line;
            while (std::getline(myFile, line)) {
                fileContent += line + '\n';
            }
            myFile.close();
        } else {
            std::cerr << "Unable to open file: " << filename << std::endl;
            fileContent = "Error: Could not open file."; // Or throw an exception
        }
    }

    std::string getFileContent() const {
        return fileContent;
    }
};

int main() {
    std::string test = "mohammed\0omari";
    std::cout << test.size() << std::endl;

    return 0;
}
