//
// Created by lu'ren'jia'd's on 2021/12/7.
//

#ifndef UNTITLED_BLOCKLIST2_H
#define UNTITLED_BLOCKLIST2_H

#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <set>

bool compare_char(const char *lhs, const char *rhs) {
    if (strcmp(lhs, rhs) < 0)return true;
    else return false;
};
const int length = 66;


class BlockList {
    std::string FileName;
    long Start = 0;
public:
    struct Block {
        int CurrentSize = 0;
        const int MaxSize = 5000;
        char first_array[5000][length];
        int second_array[5000]; //maybe you need bigger array, to do
        char MaxValue[length];
        char MinValue[length];
        long Next = -100000;
        long MyLocation = -100000;
    };

    BlockList(const std::string &fn) : FileName(fn) {};

    void InsertPair(char *first_, int second_);

    void DeletePair(char *first_, int second_);

    std::string FindPairs(char *first_);
};

void BlockList::InsertPair(char *first_, int second_) {
    std::fstream file;
    file.open(FileName, std::ios::out | std::ios::app);
    //file.seekp(0);
    file.close();
    file.open(FileName);
    if (file.peek() == EOF) {
        file.seekg(0);
        auto block = new Block;
        //block->first_array[block->CurrentSize][length] = *first_;
        std::strcpy(block->first_array[block->CurrentSize], first_);
        block->second_array[block->CurrentSize] = second_;
        block->CurrentSize++;
        std::strcpy(block->MaxValue, first_);
        std::strcpy(block->MinValue, first_);
        block->MyLocation = Start;
        //std::cout << "is open?" << file.is_open() << std::endl;
        //std::cout << "is failed?" << file.fail() << std::endl;
        file.seekp(0);
        file.write(reinterpret_cast<char *>(block), sizeof(Block));
        //std::cout << "is open?" << file.is_open() << std::endl;
        //std::cout << "is failed?" << file.fail() << std::endl;
        delete block;
    } else {
        Block read_block;
        file.seekg(Start);
        file.read(reinterpret_cast<char *>(&read_block), sizeof(Block));
        while (strcmp(read_block.MaxValue, first_) < 0 && read_block.MaxValue[0] != '\0') {
            if (read_block.Next == -100000)break;
            file.seekg(read_block.Next);
            file.read(reinterpret_cast<char *>(&read_block), sizeof(Block));
        }
        //read_block这就是我要放进去的那一块，或者是最后一块
        //如果这一块已经满了，分为放最后，夹中间两种
        if (read_block.CurrentSize == read_block.MaxSize) {//放最后一定是最后一块了
            int index = std::lower_bound(read_block.first_array, read_block.first_array + read_block.CurrentSize,
                                         first_, compare_char) - read_block.first_array;
            Block *add_block;
            add_block = new Block;
            int j = 0;
            for (int i = read_block.MaxSize / 2; i < read_block.MaxSize; ++i, ++j) {
                strcpy(add_block->first_array[j], read_block.first_array[i]);
                add_block->second_array[j] = read_block.second_array[i];
                memset(read_block.first_array[i], '\0', sizeof(read_block.first_array[i]));
            }
            read_block.CurrentSize = read_block.MaxSize / 2;
            add_block->CurrentSize = read_block.CurrentSize;
            strcpy(read_block.MaxValue, read_block.first_array[read_block.CurrentSize - 1]);
            strcpy(add_block->MinValue, add_block->first_array[0]);
            strcpy(add_block->MaxValue, add_block->first_array[add_block->CurrentSize - 1]);
            file.seekp(0, std::ios::end);
            add_block->MyLocation = file.tellp();
            add_block->Next = read_block.Next;
            read_block.Next = add_block->MyLocation;
            if (index >= read_block.MaxSize / 2) {
                index -= read_block.MaxSize / 2;
                for (int i = add_block->CurrentSize; i > index; --i) {
                    strcpy(add_block->first_array[i], add_block->first_array[i - 1]);
                    add_block->second_array[i] = add_block->second_array[i - 1];
                }
                strcpy(add_block->first_array[index], first_);
                add_block->second_array[index] = second_;
                add_block->CurrentSize++;
            } else {
                for (int i = read_block.CurrentSize; i > index; --i) {
                    strcpy(read_block.first_array[i], read_block.first_array[i - 1]);
                    read_block.second_array[i] = read_block.second_array[i - 1];
                }
                strcpy(read_block.first_array[index], first_);
                read_block.second_array[index] = second_;
                read_block.CurrentSize++;
            }
            file.write(reinterpret_cast<char *>(add_block), sizeof(Block));
            file.seekp(read_block.MyLocation);
            file.write(reinterpret_cast<char *>(&read_block), sizeof(Block));
            delete add_block;
        } else {//如果这一块还没满
            if (strcmp(first_, read_block.MaxValue) > 0) {
                //read_block.MaxValue = first_;
                std::strcpy(read_block.MaxValue, first_);
                std::strcpy(read_block.first_array[read_block.CurrentSize], first_);
                read_block.second_array[read_block.CurrentSize] = second_;
                read_block.CurrentSize++;
            } else {
                int index = std::lower_bound(read_block.first_array, read_block.first_array + read_block.CurrentSize,
                                             first_,
                                             compare_char) -
                            read_block.first_array;
                for (int i = read_block.CurrentSize; i > index; --i) {
                    // read_block.first_array[i] = read_block.first_array[i - 1];
                    std::strcpy(read_block.first_array[i], read_block.first_array[i - 1]);
                    read_block.second_array[i] = read_block.second_array[i - 1];
                }
                read_block.CurrentSize++;
                //read_block.first_array[index] = first_;
                std::strcpy(read_block.first_array[index], first_);
                std::strcpy(read_block.MinValue, read_block.first_array[0]);
                read_block.second_array[index] = second_;
            }
            file.seekp(read_block.MyLocation);
            file.write(reinterpret_cast<char *>(&read_block), sizeof(Block));
        }
    }
    // file.flush();
    file.close();
}

void BlockList::DeletePair(char *first_, int second_) {
    std::fstream file;
    file.open(FileName);
    Block search;
    file.seekg(Start);
    file.read(reinterpret_cast<char *>(&search), sizeof(Block));
    bool find_it = false;
    int index;
    while (1) {
        if (strcmp(first_, search.MaxValue) <= 0 && strcmp(first_, search.MinValue) >= 0 &&
            search.MaxValue[0] != '\0') {
            index = std::lower_bound(search.first_array, search.first_array + search.CurrentSize, first_,
                                     compare_char) - search.first_array;
            while (strcmp(first_, search.first_array[index]) == 0 && index < search.CurrentSize) {
                if (search.second_array[index] == second_) {
                    find_it = true;
                    break;
                }
                ++index;
            }
        }
        if (find_it)break;
        if (strcmp(first_, search.MinValue) < 0)break;
        if (search.Next == -100000)break;
        file.seekg(search.Next);
        file.read(reinterpret_cast<char *>(&search), sizeof(Block));
    }
    file.close();
    if (find_it) {
        search.CurrentSize--;
        if (search.CurrentSize == 0) {
            memset(search.MaxValue, '\0', sizeof(search.MaxValue));
            memset(search.MinValue, '\0', sizeof(search.MinValue));
        } else {
            for (int i = index; i < search.CurrentSize; ++i) {
                //search.first_array[i] = search.first_array[i + 1];
                std::strcpy(search.first_array[i], search.first_array[i + 1]);
                search.second_array[i] = search.second_array[i + 1];
            }
            std::memset(search.first_array[search.CurrentSize], '\0', sizeof(search.first_array[search.CurrentSize]));
            // search.MaxValue = search.first_array[search.CurrentSize - 1];
            std::strcpy(search.MaxValue, search.first_array[search.CurrentSize - 1]);
            std::strcpy(search.MinValue, search.first_array[0]);
        }
        file.open(FileName);
        if (search.Next != -100000) {
            file.seekg(search.Next);
            Block next;
            file.read(reinterpret_cast<char *>(&next), sizeof(Block));
            if (search.CurrentSize + next.CurrentSize < (search.MaxSize / 2)) {
                for (int i = search.CurrentSize, j = 0; j < next.CurrentSize; ++i, ++j) {
                    strcpy(search.first_array[i], next.first_array[j]);
                    search.second_array[i] = next.second_array[j];
                }
                search.CurrentSize += next.CurrentSize;
                search.Next = next.Next;
                strcpy(search.MaxValue, next.MaxValue);
                strcpy(search.MinValue, search.first_array[0]);
            }
        }
        file.seekp(search.MyLocation);
        file.write(reinterpret_cast<char *>(&(search)), sizeof(Block));
        file.close();
    }
}

std::string BlockList::FindPairs(char *first_) {
    std::fstream file;
    file.open(FileName);
    Block search;
    file.seekg(Start);
    file.read(reinterpret_cast<char *>(&search), sizeof(Block));
    std::set<int> values;
    while (1) {
        if ((strcmp(first_, search.MaxValue) <= 0) && (strcmp(first_, search.MinValue) >= 0) &&
            search.MaxValue[0] != '\0') {
            // have_the_keyword.push_back(search);
            int index = std::lower_bound(search.first_array, search.first_array + search.CurrentSize, first_,
                                         compare_char) - search.first_array;
            while (strcmp(search.first_array[index], first_) == 0 && index < search.CurrentSize) {
                values.insert(search.second_array[index]);
                index++;
            }
        }
        if (strcmp(first_, search.MinValue) < 0)break;
        if (search.Next == -100000)break;
        file.seekg(search.Next);
        file.read(reinterpret_cast<char *>(&search), sizeof(Block));
    }
    file.close();
    if (values.empty())return "null\n";
    else {
        std::string ans;
        std::stringstream ss;
        for (auto ptr = values.begin(); ptr != values.end(); ++ptr) {
            ss.clear();
            ss << *ptr;
            std::string temp;
            ss >> temp;
            ans += temp + " ";
        }
        return ans + "\n";
    }
}

#endif //UNTITLED_BLOCKLIST2_H