//
// Created by SiriusNEO on 2020/11/3.
// Filename:HashTable.h
// 哈希表类 实现文件
//

#include "HashTable.h"
#include <iostream>

Node::Node() {
    offset = -1;
    hash = 0ll, hash1 = 0ll;
}
bool Node::operator < (const Node &node) const {
    return hash < node.hash;
}
bool Node::operator == (const Node &node) const {
    return hash == node.hash;
}
bool Node:: operator <= (const Node &node) const {
    return (hash < node.hash || hash == node.hash);
}
Block::Block() {
    next = -1;
    nodeNum = 0;
}
/*
 * 调试用
void Block::Print() {
    std::cout << next << " " << nodeNum << '\n';
    for (int i = 0; i < nodeNum; ++i) {
        std::cout << nodes[i].offset << " " << nodes[i].hash << '\n';
    }
    std::cout << '\n';
}
*/
HashTable::HashTable(const char* fileName) {
    FILE_NAME = fileName;
    indexFile.open(FILE_NAME, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    if (!indexFile) {
        std::ofstream outFile(FILE_NAME);
        outFile.close();
        indexFile.open(FILE_NAME, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
        first = -1;
        indexFile.seekp(0);
        indexFile.write(reinterpret_cast<char *>(&first), sizeof(int));
    }
    else {
        indexFile.seekg(0);
        indexFile.read(reinterpret_cast<char *>(&first), sizeof(int));
    }
    indexFile.close();
}
template <class T> int HashTable::Save(T x) {
    indexFile.open(FILE_NAME, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    int savePos = indexFile.tellp();
    indexFile.write(reinterpret_cast<char *>(&x), sizeof(T));
    indexFile.close();
    return savePos;
}
template <class T> void HashTable::SaveIn(T x, int offset) {
    indexFile.open(FILE_NAME, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    indexFile.seekp(offset, std::ios::beg);
    indexFile.write(reinterpret_cast<char *>(&x), sizeof(T));
    indexFile.close();
}
template <class T> T HashTable::Load(int offset) {
    indexFile.open(FILE_NAME, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    T tmp;
    indexFile.seekg(offset, std::ios::beg);
    indexFile.read(reinterpret_cast<char *>(&tmp), sizeof(T));
    indexFile.close();
    return tmp;
}
/*
 * 调试用
void HashTable::Print() {
    int pointer = first;
    Block tmp;
    while (pointer != -1) {
        tmp = Load<Block>(pointer);
        tmp.Print();
        pointer = tmp.next;
    }
}
 */
void HashTable::AddBlock(int rank, Node node) {
    Block tmp;
    tmp.nodes[0] = node;
    tmp.nodeNum = 1;
    int pointer = first;
    if (rank == 0 || first == -1) {
        tmp.next = first;
        first = Save(tmp);
        SaveIn(first, 0);
        return ;
    }
    while (--rank) pointer = Load<int>(pointer);
    tmp.next = Load<int>(pointer);
    SaveIn(Save(tmp), pointer);
}
int HashTable::FindBlock(int rank) {
    int pointer = first;
    while (--rank) pointer = Load<int>(pointer);
    return pointer;
}
void HashTable::SplitBlock(int rank, int pos, Node node) {
    AddBlock(rank, node);
    int pointer = FindBlock(rank), nextPointer = Load<int>(pointer);
    Block tmp = Load<Block>(pointer);
    Block nextTmp = Load<Block>(nextPointer);
    int tmpNum = tmp.nodeNum;
    for (int i = pos; i < tmpNum; ++i) {
        nextTmp.nodes[i-pos+1] = tmp.nodes[i];
        tmp.nodeNum--, nextTmp.nodeNum++;
    }
    SaveIn(tmp, pointer);
    SaveIn(nextTmp, nextPointer);
}
void HashTable::MergeBlock(int rank) {
    int pointer = FindBlock(rank);
    Block tmp = Load<Block>(pointer), nextTmp = Load<Block>(Load<int>(pointer));
    int tmpNum = tmp.nodeNum, nextTmpNum = tmp.nodeNum + nextTmp.nodeNum;
    for (int i = tmpNum; i < nextTmpNum; ++i) {
        tmp.nodes[i] = nextTmp.nodes[i-tmpNum];
        tmp.nodeNum++, nextTmp.nodeNum--;
    }
    tmp.next = nextTmp.next;
    SaveIn(tmp, pointer);
}
void HashTable::AddNode(Node node) {
    int pointer = first, rank = 1;
    if (pointer == -1) {
        AddBlock(0, node); return ;
    }
    else if (node < Load<Node>(pointer+2*sizeof(int))) AddBlock(0, node);
    else {
        while (Load<int>(pointer) != -1 && Load<Node>(Load<int>(pointer)+2*sizeof(int)) <= node)
            pointer = Load<int>(pointer), ++rank;
        Block tmp = Load<Block>(pointer);
        int i = 0;
        for (; i < tmp.nodeNum && tmp.nodes[i] <= node; ++i);
        SplitBlock(rank, i, node);
    }
    pointer = first, rank = 1;
    while (Load<int>(pointer) != -1) {
        if (Load<int>(pointer + sizeof(int)) + Load<int>(Load<int>(pointer) + sizeof(int)) <= BLOCK_SIZE) {
            MergeBlock(rank);
            break;     //try
        }
        pointer = Load<int>(pointer), ++rank;
    }
}
std::vector<int> HashTable::FindNode(unsigned long long hash, unsigned long long hash1) {
    std::vector<int> returnList;
    int pointer = first;
    if (pointer != -1) {
        while (Load<int>(pointer) != -1 && Load<Node>(Load<int>(pointer)+2*sizeof(int)).hash < hash)
            pointer = Load<int>(pointer);
    }
    Block tmp;
    while (pointer != -1 && tmp.nodes[0].hash <= hash) {
        tmp = Load<Block>(pointer);
        for (int i = 0; i < tmp.nodeNum; ++i)
            if (tmp.nodes[i].hash == hash && tmp.nodes[i].hash1 == hash1) returnList.push_back(tmp.nodes[i].offset);
        pointer = Load<int>(pointer);
    }
    return returnList;
}
unsigned long long HashTable::BKDRHash(const char *str, int typ) {
    unsigned long long seed, hash = 0, Mod;
    seed = (typ) ? 131 : 13131;
    Mod = (typ) ? Mod1 : Mod2;
    while (*str) {
        hash = (hash * seed + (*str++)) % Mod;
    }
    return hash;
}
void HashTable::Insert(int offset, const char* key) {
    Node tmp;
    tmp.offset = offset, tmp.hash = BKDRHash(key, 0), tmp.hash1 = BKDRHash(key, 1);
    AddNode(tmp);
}
std::vector<int> HashTable::Find(const char* key) {
    unsigned long long hash = BKDRHash(key, 0), hash1 = BKDRHash(key, 1);
    return FindNode(hash, hash1);
}