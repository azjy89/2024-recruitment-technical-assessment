#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <iostream>
#include <stdio.h>

// Definitions
#define NO_PARENT -1

// Struct File Defintion
struct File {
    int id;
    std::string name;
    std::vector<std::string> categories;
    int numCategories;
    int parent;
    int size;
};

// Helper Function Prototypes
static int maxCount(
    std::unordered_map<std::string, int> map
);

static void bucketSort(std::unordered_set<std::string> categories, 
                       std::unordered_map<std::string, int> categoryCounts,
                       std::vector<std::vector<std::string>> &buckets);

static void alphabetic_sort(std::vector<std::string> &strings);

static bool stringComparison(std::string a, std::string b);

/**
 * Task 1
 * This function returns an array containing the names of the leaf files
 * given an array of Files
 */
std::vector<std::string> leafFiles(std::vector<File> files) {
    // Going through each file in files and adding the parentId to parentFileIds
    std::unordered_set<int> parentFileIds;
    for (int i = 0; i < files.size(); i++) {
        parentFileIds.insert(files[i].parent);
    }

    // Checking if each file id exists in parentFileIds and adding the file's
    // name to the leafFiles vector if the file is not in parentFileIds
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
 * This function determines the k largest categories by number of files in the 
 * category and returns an array of the names of those categories given an array
 * of Files and an integer k
 */
std::vector<std::string> kLargestCategories(std::vector<File> files, int k) {
    // Creating a set of categories and a map of category names to the number
    // of files in the category
    std::unordered_set<std::string> categories;
    std::unordered_map<std::string, int> categoryCounts;
    for (int i = 0; i < files.size(); i++) {
         for (int j = 0; j < files[i].numCategories; j++) {
            categories.insert(files[i].categories[j]);
            categoryCounts[files[i].categories[j]]++;
        }
    }

    // Finding the size of the largest category
    int maxCategoryCount = maxCount(categoryCounts);
    
    // Initialising first values of buckets
    std::vector<std::vector<std::string>> buckets(categories.size() + 1);
    for (int i = 0; i < buckets.size(); i++) {
        buckets[i].push_back("");
    }
    
    // Bucketsorting strings
    bucketSort(categories, categoryCounts, buckets);
    
    // Filling out the largest Categories array
    std::vector<std::string> largestCategories;
    int numCategoriesUnaccounted = k;
    for (int i = maxCategoryCount; i > 0; i--) {
        for (int j = 0; j < buckets[i].size(); j++) {
            if (buckets[i][j] != "") {
                largestCategories.push_back(buckets[i][j]);
                numCategoriesUnaccounted--;
            }

            if (numCategoriesUnaccounted == 0) { // If k categories added, break
                break;  
            }
        }
        if (numCategoriesUnaccounted == 0) {     // If k categories added, break
            break;
        }
    }
    
    return largestCategories;
}

// This function find the largest value given a map of strings to ints
static int maxCount(std::unordered_map<std::string, int> map) {
    int max = 0;
    for (const auto& pair : map) {
        if (pair.second > max) {
            max = pair.second;
        }
    }

    return max;
}

// This function performs bucket sort given a set of category names, a map of 
// category names to files in the category, and an array of string "buckets"
static void bucketSort(std::unordered_set<std::string> categories, 
                       std::unordered_map<std::string, int> categoryCounts,
                       std::vector<std::vector<std::string>> &buckets) {
    // Bucket sorting the categories based on their counts
    for (std::string x : categories) {
        buckets[categoryCounts[x]].push_back(x);
    }

    // Sorting each bucket alphabetically
    for (int i = 0; i < buckets.size(); i++) {
        alphabetic_sort(buckets[i]);
    }
}

// This function sorts an array of strings alphabetically
static void alphabetic_sort(std::vector<std::string> &strings) {
    std::sort(strings.begin(), strings.end(), stringComparison);
}

// This function returns compares two strings using default string comparison
// operator
static bool stringComparison(std::string a, std::string b) { return a < b; } 

/**
 * Task 3
 * This function returns the size of the largest files (including all children,
 * grandchildren, etc.) given an array of Files
 */
int largestFileSize(std::vector<File> files) {
    // Creating a map of file ids to pointers to files given the key file id, 
    // the value is a pointer to the file with that id
    std::unordered_map<int, File *> fileLookup;
    for (int i = 0; i < files.size(); ++i) {
        fileLookup[files[i].id] = &(files[i]);
    }
    
    // Creating a map of file ids of root files to the size of the file
    std::unordered_map<int, int> rootFileSize;
    for (auto file : files) {
        // find the root file
        File *root = &file;
        while (true) {
            if (root->parent == NO_PARENT) {
                break;
            } else {
                root = fileLookup[root->parent];
            }
        }

        // adding root file sizes
        if (rootFileSize.find(root->id) != rootFileSize.end()) {
            rootFileSize[root->id] += file.size;
        } else {
            rootFileSize[root->id] = file.size;
        }

    }

    // now loop over to find the largest size
    int currMax = 0;
    for (auto file_size_pair : rootFileSize) {
        currMax = std::max(currMax, file_size_pair.second);
    }


    return currMax;
}

int main(void) {
    // Test Files
    std::vector<File> testFiles1{
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

    std::vector<File> testFiles2{
        { .id = 1, .name = "Document.txt", .categories = {"Documents"}, .numCategories = 1, .parent = 3, .size = 1024 },
        { .id = 2, .name = "Image.jpg", .categories = {"Media", "Photos"}, .numCategories = 2, .parent = 34, .size = 2048 },
        { .id = 3, .name = "Folder", .categories = {"Medib"}, .numCategories = 1, .parent = -1, .size = 0 },
        { .id = 5, .name = "Spreadsheet.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 4096 },
        { .id = 8, .name = "Backup.zip", .categories = {"Backup"}, .numCategories = 1, .parent = 233, .size = 8192 },
        { .id = 13, .name = "Presentation.pptx", .categories = {"Documents", "Presentation"}, .numCategories = 2, .parent = 3, .size = 3072 },
        { .id = 21, .name = "Video.mp4", .categories = {"Media", "Videos"}, .numCategories = 2, .parent = 34, .size = 6144 },
        { .id = 34, .name = "Folder2", .categories = {"Medib"}, .numCategories = 1, .parent = 3, .size = 0 },
        { .id = 55, .name = "Code.py", .categories = {"Programming"}, .numCategories = 1, .parent = -1, .size = 1536 },
        { .id = 89, .name = "Audio.mp3", .categories = {"Media", "Audio"}, .numCategories = 2, .parent = 34, .size = 2560 },
        { .id = 144, .name = "Spreadsheet2.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 2048 },
        { .id = 233, .name = "Folder3", .categories = {"Medib"}, .numCategories = 1, .parent = -1, .size = 4096 },
    };

    std::vector<File> testFiles3{
        { .id = 1, .name = "Document.txt", .categories = {"Documents"}, .numCategories = 1, .parent = 3, .size = 1024 },
        { .id = 2, .name = "Image.jpg", .categories = {"Medib", "Photos"}, .numCategories = 2, .parent = 34, .size = 2048 },
        { .id = 3, .name = "Folder", .categories = {"Media"}, .numCategories = 1, .parent = -1, .size = 0 },
        { .id = 5, .name = "Spreadsheet.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 4096 },
        { .id = 8, .name = "Backup.zip", .categories = {"Backup"}, .numCategories = 1, .parent = 233, .size = 8192 },
        { .id = 13, .name = "Presentation.pptx", .categories = {"Documents", "Presentation"}, .numCategories = 2, .parent = 3, .size = 3072 },
        { .id = 21, .name = "Video.mp4", .categories = {"Medib", "Videos"}, .numCategories = 2, .parent = 34, .size = 6144 },
        { .id = 34, .name = "Folder2", .categories = {"Media"}, .numCategories = 1, .parent = 3, .size = 0 },
        { .id = 55, .name = "Code.py", .categories = {"Programming"}, .numCategories = 1, .parent = -1, .size = 1536 },
        { .id = 89, .name = "Audio.mp3", .categories = {"Medib", "Audio"}, .numCategories = 2, .parent = 34, .size = 2560 },
        { .id = 144, .name = "Spreadsheet2.xlsx", .categories = {"Documents", "Excel"}, .numCategories = 2, .parent = 3, .size = 2048 },
        { .id = 233, .name = "Folder3", .categories = {"Media"}, .numCategories = 1, .parent = -1, .size = 4096 },
    };

    std::vector<File> testFiles4{
        { .id = 1, .name = "Document.txt", .categories = {"Documents"}, .numCategories = 1, .parent = 3, .size = 1024 },
        { .id = 2, .name = "Image.jpg", .categories = {"Excel", "Photos"}, .numCategories = 2, .parent = 34, .size = 2048 },
        { .id = 3, .name = "Folder", .categories = {"Videos"}, .numCategories = 1, .parent = -1, .size = 0 },
        { .id = 5, .name = "Spreadsheet.xlsx", .categories = {"Documents"}, .numCategories = 1, .parent = 3, .size = 4096 },
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

    // Task 1 tests

    std::vector<std::string> returnedLeafFiles = leafFiles(testFiles1);
    std::sort(returnedLeafFiles.begin(), returnedLeafFiles.end());
    assert(expectedLeafFiles == returnedLeafFiles);

    // Task 2 tests

    // General tests
    std::vector<std::string> expectedCategories1{"Documents", "Folder", "Media"};
    std::vector<std::string> returnedCategories1 = kLargestCategories(testFiles1, 3);
    assert(expectedCategories1 == returnedCategories1);

    // Testing alphabetic sorting
    std::vector<std::string> expectedCategories2{"Documents", "Media", "Medib"};
    std::vector<std::string> returnedCategories2 = kLargestCategories(testFiles2, 3);
    assert(expectedCategories2 == returnedCategories2);

    std::vector<std::string> expectedCategories3{"Documents", "Media", "Medib"};
    std::vector<std::string> returnedCategories3 = kLargestCategories(testFiles3, 3);
    assert(expectedCategories3 == returnedCategories3);

    // Testing non-contiguous buckets
    std::vector<std::string> expectedCategories4{"Documents", "Excel", "Folder"};
    std::vector<std::string> returnedCategories4 = kLargestCategories(testFiles4, 3);
    assert(expectedCategories4 == returnedCategories4);

    // Task 3 tests

    assert(largestFileSize(testFiles1) == 20992);

}