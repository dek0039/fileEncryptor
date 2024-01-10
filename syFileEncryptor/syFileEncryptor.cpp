#include <iostream>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <fstream>
#include <Windows.h>

constexpr const char* IDENTIFIER = "syEncryption";
constexpr int IDENTIFIER_LENGTH = 12; /*char length*/

static std::unordered_map<char, char> substitutionMap = {
    {'a', 'z'},
    {'b', 'd'},
    {'c', 'x'},
    {'d', 'y'},
    {'e', 'w'},
    {'f', 'v'},
    {'g', 'u'},
    {'h', 't'},
    {'i', 's'},
    {'j', 'r'},
    {'k', 'p'},
    {'l', 'q'},
    {'m', 'o'},
    {'n', 'n'},
    {'o', 'm'},
    {'p', 'l'},
    {'q', 'j'},
    {'r', 'k'},
    {'s', 'i'},
    {'t', 'h'},
    {'u', 'g'},
    {'v', 'f'},
    {'w', 'e'},
    {'x', 'b'},
    {'y', 'c'},
    {'z', 'a'}
};

std::string readFileContents(const std::string& filePath)
{
    std::string fileContent;
    std::ifstream fileStream(filePath);

    if (fileStream.is_open()) {
        fileContent.assign(
            (std::istreambuf_iterator<char>(fileStream)),
            (std::istreambuf_iterator<char>())
        );

        fileStream.close();

        if (fileContent.size() <= IDENTIFIER_LENGTH)
        {
            std::cerr << "File too short or invalid!" << std::endl;
            return "";
        }
    }

    return fileContent;
}

std::string mutateString(const std::string& input) {
    std::string mutated;
    for (char c : input) {
        if (substitutionMap.find(c) != substitutionMap.end()) {
            mutated.push_back(substitutionMap[c]);
        }
        else {
            mutated.push_back(c);
        }
    }
    return mutated;
}

std::string reverseMutate(const std::string& mutated) {
    std::string original;
    for (char c : mutated) {
        bool found = false;
        for (const auto& entry : substitutionMap) {
            if (entry.second == c) {
                original.push_back(entry.first);
                found = true;
                break;
            }
        }
        if (!found) {
            original.push_back(c);
        }
    }
    return original;
}

std::string xorString(const std::string& input, uint8_t key) {
    std::string obfuscated;

    for (char c : input) {
        char obfuscatedChar = c ^ key;
        obfuscated.push_back(obfuscatedChar);
    }
    return obfuscated;
}

std::string enlargeString(const std::string& input)
{
    return input + input;
}

std::string delargeString(const std::string& input) 
{
    std::string newString =  input.substr(0, input.size() / 2);

    return newString;
}

std::string reverseString(const std::string& input)
{
    std::string newString = input;

    std::reverse(newString.begin(), newString.end());

    return newString;
}

void writeNewFile(const std::string& filePath, std::string buffer, bool wasEncrypted)
{
    auto file = std::filesystem::path(filePath);
    std::string newPath = file.parent_path().string() + "\\" + file.filename().stem().string() + (wasEncrypted ? "_then_decrypted" : "_encrypted") + file.extension().string();

    std::ofstream outputFile(newPath);

    if (outputFile.is_open()) {
        outputFile << buffer;
        outputFile.close();
        std::cout << "File '" << newPath << "' has been created successfully." << std::endl;
    }
    else {
        std::cerr << "Failed to open the file for writing." << std::endl;
    }
}

int findFileKey(const std::string& input)
{
    std::string extracted = input.substr(0, IDENTIFIER_LENGTH);

    for (int i = 0; i < 255; i++)
    {
        std::string temp;
        for (char c : extracted)
        {
            char deobfuscatedChar = c ^ i;
            temp.push_back(deobfuscatedChar);
        }

        if (temp.find(IDENTIFIER) != std::string::npos)
        {
            return i;
        }
    }

    return -1;
}

bool isFileEncrypted(const std::string& input)
{
    std::string extracted = input.substr(0, IDENTIFIER_LENGTH);

    for (int i = 0; i < 255; i++)
    {
        std::string temp;
        for (char c : extracted)
        {
            char deobfuscatedChar = c ^ i;
            temp.push_back(deobfuscatedChar);
        }

        if (temp.find(IDENTIFIER) != std::string::npos)
        {
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[])
{
    std::srand(std::time(nullptr));

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << ", <file>, " << "<decrypt(leave empty if not needed)> " << std::endl;
        return 0;
    }

    if (!std::filesystem::exists(argv[1]))
    {
        std::cerr << "Invalid file path!" << std::endl;
        return 0;
    }


    std::string fileContents = readFileContents(argv[1]);
    std::string fileContentsCopy = fileContents;

    size_t originalFileLength = fileContents.length();
    std::cout << "File length: " << originalFileLength << std::endl;

#pragma region decryptFile
    if (argc == 3)
    {
        fileContentsCopy = fileContents;

        fileContentsCopy = reverseString(fileContentsCopy);

        fileContentsCopy = delargeString(fileContentsCopy);

        if (findFileKey(fileContentsCopy) == -1)
        {
            std::cerr << "File is not encrypted or is invalid!" << std::endl;
            return 0;
        }

        fileContentsCopy = xorString(fileContentsCopy, findFileKey(fileContentsCopy));

        fileContentsCopy = fileContentsCopy.erase(0, IDENTIFIER_LENGTH);

        fileContentsCopy = reverseMutate(fileContentsCopy);

        writeNewFile(argv[1], fileContentsCopy, true);
        return 1;
    }
#pragma endregion

#pragma region checkFile
    fileContentsCopy = reverseString(fileContentsCopy);
    
    fileContentsCopy = delargeString(fileContentsCopy);
    
    fileContentsCopy = xorString(fileContentsCopy, findFileKey(fileContentsCopy));

    if (fileContentsCopy.substr(0, IDENTIFIER_LENGTH) == std::string(IDENTIFIER))
    {
        std::cerr << "File is already protected!" << std::endl;
        return 0;
    }

#pragma endregion

#pragma region encryptFile
    ENCRYPT:
    Sleep(1000);
    int pseudoRandom = rand() % 255;
    std::cerr << pseudoRandom << std::endl;

    //Stage 1
    fileContents = mutateString(fileContents);

    //Stage 2
    fileContents.insert(0, IDENTIFIER);

    //Stage 3
    fileContents = xorString(fileContents, pseudoRandom);

    //Stage 4
    fileContents = enlargeString(fileContents);

    //Stage 5
    fileContents = reverseString(fileContents);
#pragma endregion

#pragma region ensureEncryption
    fileContentsCopy = fileContents;

    fileContentsCopy = reverseString(fileContentsCopy);

    fileContentsCopy = delargeString(fileContentsCopy);

    std::cerr << findFileKey(fileContentsCopy) << std::endl;
    fileContentsCopy = xorString(fileContentsCopy, findFileKey(fileContentsCopy));

    fileContentsCopy = fileContentsCopy.erase(0, IDENTIFIER_LENGTH);

    fileContentsCopy = reverseMutate(fileContentsCopy);

    if (fileContentsCopy.length() != originalFileLength)
    {
        std::cerr << "Encryption failed! Trying again." << std::endl;
        goto ENCRYPT;
    }
#pragma endregion

    writeNewFile(argv[1], fileContents, false);

    return 1;
}