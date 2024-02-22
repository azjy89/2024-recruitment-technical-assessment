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

// Helper Function Function Prototypes

static int maxCount(
    std::unordered_map<std::string, int> map
);

static void bucketSort(
    std::unordered_set<std::string> categories, 
    std::unordered_map<std::string, int> categoryCounts,
    std::vector<std::vector<std::string>> &buckets
);

static void alphabetic_sort(
    std::vector<std::string> &strings
);

static bool stringComparison(
    std::string a, std::string b
);

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
    int numCategoriesUnaccounted = k;           // Number of unadded categories
    for (int i = maxCategoryCount; i > 0; i--) {
        for (int j = 0; j < buckets[i].size(); j++) {
            if (buckets[i][j] != "") {
                largestCategories.push_back(buckets[i][j]);
                numCategoriesUnaccounted--;
            }

            if (numCategoriesUnaccounted == 0) {
                break;
            }
        }
        if (numCategoriesUnaccounted == 0) {
            break;
        }
    }
    
    return largestCategories;
}

static int maxCount(std::unordered_map<std::string, int> map) {
    int max = 0;
    for (const auto& pair : map) {
        if (pair.second > max) {
            max = pair.second;
        }
    }

    return max;
}

static void bucketSort(
    std::unordered_set<std::string> categories, 
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

static void alphabetic_sort(std::vector<std::string> &strings) {
    std::sort(strings.begin(), strings.end(), stringComparison);
}

static bool stringComparison(std::string a, std::string b) { return a < b; } 

/**
 * Task 3
 */
int largestFileSize(std::vector<File> files) {
    // A map of file ids to pointers to files given the key file id, 
    // the value is a pointer to the file with that id
    std::unordered_map<int, File *> fileLookup;
    for (int i = 0; i < files.size(); ++i) {
        fileLookup[files[i].id] = &(files[i]);
    }
    // A map of 
    // given the file id, the value is the size of the file
    // only roots are stored in here
    std::unordered_map<int, int> file_sizes;
    for (auto file : files) {
        // find the root
        File *root = &file;
        while (true) {
            if (root->parent == NO_PARENT) {
                break;
            } else {
                root = fileLookup[root->parent];
            }
        }

        // now add to size
        if (file_sizes.find(root->id) != file_sizes.end()) {
            file_sizes[root->id] += file.size;
        } else {
            file_sizes[root->id] = file.size;
        }

    }

    // now loop over to find the largest size
    int curmax = 0;
    for (auto file_size_pair : file_sizes) {
        curmax = std::max(curmax, file_size_pair.second);
    }


    return curmax;
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