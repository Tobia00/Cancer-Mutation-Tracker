#pragma once
#include <string>
using namespace std;

// Basic dynamic string array functions
string* createStringArray(int size) {
    return new string[size];
}

void addStringToArray(string*& arr, int& count, int& capacity, const string& value) {
    if (count == capacity) {
        capacity *= 2;
        string* newArr = new string[capacity];
        for (int i = 0; i < count; ++i)
            newArr[i] = arr[i];
        delete[] arr;
        arr = newArr;
    }
    arr[count++] = value;
}

void deleteStringArray(string* arr) {
    delete[] arr;
}

// 2D char matrix for Levenshtein
char** createCharMatrix(int rows, int cols) {
    char** matrix = new char*[rows];
    for (int i = 0; i < rows; ++i)
        matrix[i] = new char[cols];
    return matrix;
}

void deleteCharMatrix(char** matrix, int rows) {
    for (int i = 0; i < rows; ++i)
        delete[] matrix[i];
    delete[] matrix;
}

// 2D int matrix (if needed)
int** createIntMatrix(int rows, int cols) {
    int** matrix = new int*[rows];
    for (int i = 0; i < rows; ++i)
        matrix[i] = new int[cols];
    return matrix;
}

void deleteIntMatrix(int** matrix, int rows) {
    for (int i = 0; i < rows; ++i)
        delete[] matrix[i];
    delete[] matrix;
}

template <typename T>
class DynamicArray {
private:
    T* data;
    int capacity;
    int length;

public:
    DynamicArray(int initialCapacity = 10) {
        capacity = initialCapacity;
        length = 0;
        data = new T[capacity];
    }

    void push_back(const T& value) {
        if (length == capacity) {
            capacity *= 2;
            T* newData = new T[capacity];
            for (int i = 0; i < length; ++i)
                newData[i] = data[i];
            delete[] data;
            data = newData;
        }
        data[length++] = value;
    }

    int size() const {
        return length;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    ~DynamicArray() {
        delete[] data;
    }
};
