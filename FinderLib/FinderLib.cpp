#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "pch.h"
#include "FinderLib.h"
#include <windows.h>


static unsigned long long previous_;  
static unsigned long long current_;   
static unsigned index_;               



#define compLT(a,b) (a < b)
#define compEQ(a,b) (a == b)
#define pageStep 10
typedef enum { BLACK, RED } nodeColor;

struct FileMapping {
    HANDLE hFile;
    HANDLE hMapping;
    size_t fsize;
    unsigned char* dataPtr;
};

struct Fields
{
    std::string phone;
    std::string firstname;
    std::string secondName;
    std::string thridName;
    std::string streat;
    std::string houseNumber;
    std::string corpus;
    std::string homeNumber;
    std::string bdData;
};

typedef struct Node_ {
    struct Node_* left;        
    struct Node_* right;       
    struct Node_* parent;       
    nodeColor color;            
    Fields data;              
} Node;

#define NIL &sentinel          
Node sentinel;
int page = 0;
int pageIndex = 0;
Node* root = NIL;  

std::vector<Node*> roots;

void rotateLeft(Node* x) 
{
    Node* y = x->right;

    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;

    if (y != NIL) y->parent = x->parent;
    if (x->parent) 
    {
        if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }
    else 
    {
        root = y;
    }

    y->left = x;
    if (x != NIL) x->parent = y;
}

void rotateRight(Node* x) 
{

    Node* y = x->left;

    x->left = y->right;
    if (y->right != NIL) y->right->parent = x;

    if (y != NIL) y->parent = x->parent;
    if (x->parent) 
    {
        if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;
    }
    else 
    {
        root = y;
    }

    y->right = x;
    if (x != NIL) x->parent = y;
}

void insertFixup(Node* x) 
{

    while (x != root && x->parent->color == RED) 
    {
        if (x->parent == x->parent->parent->left) 
        {
            Node* y = x->parent->parent->right;
            if (y->color == RED) 
            {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else {
                if (x == x->parent->right) 
                {
                    x = x->parent;
                    rotateLeft(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateRight(x->parent->parent);
            }
        }
        else 
        {
            Node* y = x->parent->parent->left;
            if (y->color == RED) 
            {
                x->parent->color = BLACK;
                y->color = BLACK;
                x->parent->parent->color = RED;
                x = x->parent->parent;
            }
            else 
            {
                if (x == x->parent->left) 
                {
                    x = x->parent;
                    rotateRight(x);
                }
                x->parent->color = BLACK;
                x->parent->parent->color = RED;
                rotateLeft(x->parent->parent);
            }
        }
    }
    root->color = BLACK;
}

std::string getData(std::string field, Node* current)
{
    if (field == "phone") return current->data.phone;
    if (field == "secondName") return current->data.secondName;
    if (field == "streat") return current->data.streat;
    return "";
}
Node* insertNode(std::string data, Fields fields, std::string dataField, bool (*ptFuncE)(std::string, std::string), bool (*ptFuncS)(std::string, std::string))
{
    Node* current, * parent, * x;

    current = root;
    parent = 0;
    while (current != NIL) 
    {
        if (ptFuncE(data, getData(dataField, current)))
        {
            if (current->data.bdData.length() > 0)
            {
                current->data.bdData += ";" + fields.bdData;
            }
            else
            {
                current->data.bdData = fields.bdData;
            }
            return (current);
        }
        parent = current;
        current = ptFuncS(data, getData(dataField, current)) ? current->left : current->right;
    }

    if ((malloc(sizeof(*x))) == 0)
    {
        exit(1);
    }
    x = new Node;
    x->data = fields;
    x->parent = parent;
    x->left = NIL;
    x->right = NIL;
    x->color = RED;

    if (parent) 
    {
        if (ptFuncS(data, getData(dataField, parent)))
            parent->left = x;
        else
            parent->right = x;
    }
    else 
    {
        root = x;
    }
    insertFixup(x);
    return(x);
}

std::vector<Node*> findNode(std::string data, std::string dataField, Node* current, bool (*ptFuncE)(std::string, std::string), bool (*ptFuncS)(std::string, std::string))
{
    std::vector<Node*> findNodes;
   
    while (current != NIL)
    {
        if (ptFuncE(data, getData(dataField, current)))
        {
            findNodes.push_back(current);
            current = current->right;
        }
            
        else
            current = ptFuncS(data, getData(dataField, current)) ? current->left : current->right;
    }

    return findNodes;
}
bool isNumEquel(std::string data, std::string currentData)
{
    std::stringstream first(data);
    std::stringstream second(currentData);
    int a, b;
    first >> a;
    second >> b;
    if (a == b)
    {
        return true;
    }
    else
        return false;
};
bool isFirstNumSmaller(std::string data, std::string currentData)
{
    std::stringstream first(data);
    std::stringstream second(currentData);
    int a, b;
    first >> a;
    second >> b;
    if (a < b)
    {
        return true;
    }
    else
        return false;
};
bool isStrEquel(std::string data, std::string currentData)
{
    return data == currentData;
};
bool isFirstStrSmaller(std::string data, std::string currentData)
{
    return data < currentData;
};

std::vector<std::string> parceText(std::string str, std::string delim)
{
    std::vector<std::string> arr;
    size_t prev = 0;
    size_t next;
    size_t delta = delim.length();

    while ((next = str.find(delim, prev)) != std::string::npos)
    {
        std::string tmp = str.substr(prev, next - prev);
        arr.push_back(str.substr(prev, next - prev));
        prev = next + delta;
    }
    std::string tmp = str.substr(prev);
    arr.push_back(str.substr(prev));

    return arr;
}

std::vector<std::string> parceTextWithStop(std::string str, std::string delim, int num)
{
    std::vector<std::string> arr;
    size_t prev = 0;
    int rowCount = 0;
    size_t next;
    size_t delta = delim.length();

    while ((next = str.find(delim, prev)) != std::string::npos && rowCount < num)
    {
        std::string tmp = str.substr(prev, next - prev);
        arr.push_back(str.substr(prev, next - prev));
        rowCount++;
        prev = next + delta;
    }
    std::string tmp = str.substr(prev);
    arr.push_back(str.substr(prev));
    pageIndex += prev;
    return arr;
}
Fields getFields(std::string str)
{
    std::vector<std::string> arr;
    arr = parceText(str, ",");
    Fields item;
    item.phone = arr[0];
    item.secondName = arr[1];
    item.firstname = arr[2];
    item.thridName = arr[3];
    item.streat = arr[4];
    item.houseNumber = arr[5];
    item.corpus = arr[6];
    item.homeNumber = arr[7];
    item.bdData = str;
    

    return item;
}

void Index(const wchar_t* path)
{
    setlocale(LC_ALL, "");
    sentinel.left = NIL;
    sentinel.right = NIL;
    sentinel.parent = 0;
    sentinel.color = BLACK;

    std::vector<Fields> userData;
    std::string line;

    std::ifstream myfile(path);
    std::vector<std::string> file;
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            file.push_back(line);
        }
        myfile.close();

    }

    for (int i = 0; i < file.size(); i++)
    {
        userData.push_back(getFields(file[i]));
    }

    roots.clear();

    for (int i = 0; i < userData.size(); i++)
    {
        insertNode(userData[i].phone, userData[i], "phone", isNumEquel, isFirstNumSmaller);
    }
    roots.push_back(root);
    root = new Node;
    root = NIL;

    for (int i = 0; i < userData.size(); i++)
    {
        insertNode(userData[i].secondName, userData[i], "secondName", isStrEquel, isFirstStrSmaller);
    }
    roots.push_back(root);
    root = new Node;
    root = NIL;

    for (int i = 0; i < userData.size(); i++)
    {
        insertNode(userData[i].streat, userData[i], "streat", isStrEquel, isFirstStrSmaller);
    }
    roots.push_back(root);
    root = new Node;
    root = NIL;
}

std::string ReadFileMem(std::wstring fileName, std::string findWord)
{
    std::string result("");
    HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (hFile == INVALID_HANDLE_VALUE) 
    {
        return result;
    }

    DWORD dwFileSize = GetFileSize(hFile, nullptr);
    if (dwFileSize == INVALID_FILE_SIZE) 
    {
        CloseHandle(hFile);
        return result;
    }

    HANDLE hMapping = CreateFileMapping(hFile, nullptr, PAGE_READONLY,
        0, 0, nullptr);
    if (hMapping == nullptr) 
    {
        CloseHandle(hFile);
        return result;
    }

    unsigned char* dataPtr = (unsigned char*)MapViewOfFile(hMapping,
        FILE_MAP_READ,
        0,
        0,
        dwFileSize);
    if (dataPtr == nullptr) 
    {
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return result;
    }

    FileMapping* mapping = (FileMapping*)malloc(sizeof(FileMapping));
    if (mapping == nullptr)
    {
        UnmapViewOfFile(dataPtr);
        CloseHandle(hMapping);
        CloseHandle(hFile);
        return result;
    }

    mapping->hFile = hFile;
    mapping->hMapping = hMapping;
    mapping->dataPtr = dataPtr;
    mapping->fsize = (size_t)dwFileSize;

    std::string text(reinterpret_cast<char*>(mapping->dataPtr));
    std::vector<std::string> data = parceTextWithStop(text.substr(pageIndex, text.size()-pageIndex), "\r\n", pageStep);
    
    for (int i = 0; i < pageStep; i++)
    {
        if (strstr(data[i].c_str(), findWord.c_str()))
        {
            result += data[i] + ";";
        }
    }

    UnmapViewOfFile(mapping->dataPtr);
    CloseHandle(mapping->hMapping);
    CloseHandle(mapping->hFile);
    free(mapping);

    return result;
}
std::string Find(std::string data, std::string field)
{
    int indexTreeNumber;
    std::vector<Node*> nodes;
    if (field == "phone")
    {
        indexTreeNumber = 0;
        nodes = findNode(data, field, roots[indexTreeNumber], isNumEquel, isFirstNumSmaller);
    }
    else if (field == "secondName")
    {
        indexTreeNumber = 1;
        nodes = findNode(data, field, roots[indexTreeNumber], isStrEquel, isFirstStrSmaller);
    }
    else if (field == "streat")
    {
        indexTreeNumber = 2;
        nodes = findNode(data, field, roots[indexTreeNumber], isStrEquel, isFirstStrSmaller);
    }
    else return "";

    std::string result = "";
    
    for (int i = 0; i < nodes.size(); i++)
    {
        result = nodes[i]->data.bdData;  
    }
    return result;
}
std::string Next(std::wstring fileName, std::string findWord)
{
    page += pageStep;
    return ReadFileMem(fileName, findWord);
}
std::string Prev(std::wstring fileName, std::string findWord)
{
    if (page - pageStep > 0)
        page -= pageStep;
    return ReadFileMem(fileName, findWord);
}