#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

struct File {
    int id;
    std::string name;
    std::vector<std::string> categories;
    int numCategories;
    int parent;
    int size;
};

/**
 * Task 1
 */
std::vector<std::string> leafFiles(std::vector<File> files) {
    // Going through each file in files and adding the parentId to am
    // unordered set
    std::unordered_set<int> parentFileIds;
    for (int i = 0; i < files.size(); i++) {
        parentFileIds.insert(files[i].parent);
    }

    // Checking if each file id exists in the parentFileIds unordered set
    // and adding the file's name to the leafFiles vector if the file is
    // not in parentFileIds
    std::vector<std::string> leafFiles;
    for (int i = 0; i < files.size(); i++) {
        if (parentFileIds.find(files[i].id) == parentFileIds.end()) {
            leafFiles.push_back(files[i].name);
        }
    }

    return leafFiles;  
}

/**
 * Task 2
 */
std::vector<std::string> kLargestCategories(std::vector<File> files, int k) {
    // m is num categories the average case time required is O(mn) bcos
    // unordered_map uses hasmap implementation
    std::unordered_set<std::string> categories;
    std::unordered_map<std::string, int> categoryCounts;
    for (int i = 0; i < files.size(); i++) {
         for (int j = 0; j < files[i].numCategories; j++) {
            categories.insert(files[i].categories[j]);
            categoryCounts[files[i].categories[j]]++;
        }
    }
    // Finding the maximum category count
    int maxCount = 0;
    for (const auto& pair : categoryCounts) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
        }
    }
    // Bucket sorting O(n)
    std::vector<std::vector<std::string>> buckets(categories.size() + 1);
    for (std::string x : categories) {
        buckets[categoryCounts[x]].push_back(x);
    }
    // Filling out the largest Categories array
    std::vector<std::string> largestCategories;
    int numCategoriesUnaccounted = k;
    for (int i = maxCount; i > 0; i--) {
        for (int j = 0; j < buckets[i].size(); j++) {
            largestCategories.push_back(buckets[i][j]);
            numCategoriesUnaccounted--;
            if (numCategoriesUnaccounted == 0) {
                break;
            }
        }
        if (numCategoriesUnaccounted == 0) {
            break;
        }
    }
    
    for (int i = 0; i < k; i++) {
        std::cout << largestCategories[i] << std::endl;
    }

    return largestCategories;
}

/**
 * Task 3
 */
int largestFileSize(std::vector<File> files) {
    return 0;
}

int main(void) {
    std::vector<File> testFiles{
        { .id = 1, .name = "Document.txt", .categories = {"Documents"}, .numCategories = 1, .parent = 3, .size = 1024 },
        { .id = 2, .name = "Image.jpg", .categories = {"Media", "Photos"}, .numCategories = 2, .parent = 34, .size = 2048 },
        { .id = 3, .name = "Folder", .categories = {"Folder"}, .numCategories = 1, .parent = -1, .size = 0 },
        { .id = 5, .name = "Spreadsheet.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 4096 },
        { .id = 8, .name = "Backup.zip", .categories = {"Backup"}, .numCategories = 1, .parent = 233, .size = 8192 },
        { .id = 13, .name = "Presentation.pptx", .categories = {"Documents", "Presentation"}, .numCategories = 2, .parent = 3, .size = 3072 },
        { .id = 21, .name = "Video.mp4", .categories = {"Media", "Videos"}, .numCategories = 2, .parent = 34, .size = 6144 },
        { .id = 34, .name = "Folder2", .categories = {"Folder"}, .numCategories = 1, .parent = 3, .size = 0 },
        { .id = 55, .name = "Code.py", .categories = {"Programming"}, .numCategories = 1, .parent = -1, .size = 1536 },
        { .id = 89, .name = "Audio.mp3", .categories = {"Media", "Audio"}, .numCategories = 2, .parent = 34, .size = 2560 },
        { .id = 144, .name = "Spreadsheet2.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 2048 },
        { .id = 233, .name = "Folder3", .categories = {"Folder"}, .numCategories = 1, .parent = -1, .size = 4096 },
    };

    std::vector<std::string> expectedLeafFiles{
        "Audio.mp3",
        "Backup.zip",
        "Code.py",
        "Document.txt",
        "Image.jpg",
        "Presentation.pptx",
        "Spreadsheet.xlsx",
        "Spreadsheet2.xlsx",
        "Video.mp4"
    };
    std::vector<std::string> returnedLeafFiles = leafFiles(testFiles);
    std::sort(returnedLeafFiles.begin(), returnedLeafFiles.end());
    assert(expectedLeafFiles == returnedLeafFiles);

    std::vector<std::string> expectedCategories{"Documents", "Folder", "Media"};
    std::vector<std::string> returnedCategories = kLargestCategories(testFiles, 3);
    assert(expectedCategories == returnedCategories);

    assert(largestFileSize(testFiles) == 20992);

}