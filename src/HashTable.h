//
// Created by SiriusNEO on 2020/11/3.
// Filename:HashTable.h
// 哈希表类 声明文件
//

#ifndef BOOKSTORESYSTEM_HASHTABLE_H
#define BOOKSTORESYSTEM_HASHTABLE_H

#include <fstream>
#include <vector>
#include <string>

const int BLOCK_SIZE = 1024;
const unsigned long long Mod1 = 1e9+7;
const unsigned long long Mod2 = 1e9+9;

class Node {
    public:
        int offset;
        unsigned long long hash, hash1;
        Node();
        bool operator < (const Node &node) const;
        bool operator == (const Node &node) const;
        bool operator <= (const Node &node) const;
};

class Block {
    public:
        int next, nodeNum;
        Node nodes[BLOCK_SIZE];
        Block();
        //void Print();
};

class HashTable {
    private:
        int first;
        std::fstream indexFile;
        std::string FILE_NAME;

    public:
        HashTable(const char * fileName);
        template <class T> int Save(T x);
        template <class T> void SaveIn(T x, int offset);
        template <class T> T Load(int offset);
        //void Print();
        void AddBlock(int rank, Node node);
        int FindBlock(int rank);
        void SplitBlock(int rank, int pos, Node node);
        void MergeBlock(int rank);
        void AddNode(Node node);
        std::vector<int> FindNode(unsigned long long hash, unsigned long long hash1);
        unsigned long long BKDRHash(const char *, int typ);
        void Insert(int offset, const char *key);
        std::vector<int> Find(const char *key);
};

#endif //BOOKSTORESYSTEM_HASHTABLE_H