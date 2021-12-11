//
// Created by lu'ren'jia'd's on 2021/12/7.
//

#ifndef UNTITLED_COMMAND_H
#define UNTITLED_COMMAND_H

#include <string>
#include <sstream>
#include "blocklist2.h"

void CarryOut(const std::string &command) {
    BlockList list("BookstoreFile");
    std::string op;
    int counter = 0;
    while (command[counter] != ' ') {
        op += command[counter];
        counter++;
    }
    counter++;
    if (op == "insert") {
        char first[200];
        int counter_from0=0;
        while (command[counter] != ' ') {
            first[counter_from0] = command[counter];
            counter++;
            counter_from0++;
        }
        first[counter_from0] ='\0';
        counter++;
        std::string second_str;
        while (command[counter] != '\n') {
            second_str += command[counter];
            counter++;
        }
        std::stringstream ss;
        ss.clear();
        ss << second_str;
        int second;
        ss >> second;
        list.InsertPair(first, second);
    } else if (op == "delete") {
        char first[200];
        int counter_from0=0;
        while (command[counter] != ' ') {
            first[counter_from0] = command[counter];
            counter++;
            counter_from0++;
        }
        first[counter_from0] ='\0';
        counter++;
        std::string second_str;
        while (command[counter] != '\n') {
            second_str += command[counter];
            counter++;
        }
        std::stringstream ss;
        ss.clear();
        ss << second_str;
        int second;
        ss >> second;
        list.DeletePair(first, second);
    } else if (op == "find") {
        char first[200];
        int counter_from0=0;
        while (command[counter] != '\0'&&command[counter]!='\n') {
            first[counter_from0] = command[counter];
            counter++;
            counter_from0++;
        }
        first[counter_from0] ='\0';
        counter++;
        std::string result = list.FindPairs(first);
        std::cout << result;
    }
}

#endif //UNTITLED_COMMAND_H
