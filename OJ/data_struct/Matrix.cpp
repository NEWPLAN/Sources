//  Matrix.cpp
//  Proj1
//
//  Created by Guangyu Zou on 10/25/14.
//  Copyright (c) 2014 Guangyu Zou. All rights reserved.
//

#include <stdio.h>
#include <string.h>

class Array1D
{
private:
    int size;
    int* elements;

public:
    Array1D(int size = 0)
    {
        if (size > 0) elements = new int[size];
        else elements = 0;
        this->size = size;
    }

    virtual ~Array1D()
    {
        if (elements != 0)
            delete[] elements;
    }

    // Return the size of the array
    int Size() const
    {
        return size;
    }

    // Resize the array and copy the original elements into the new array
    void Resize(int size)
    {
        int* newArray = new int[size];
        if (elements)
        {
            memcpy(newArray, elements, this->size*sizeof(int));
            delete[] elements;
        }
        elements = newArray;
        this->size = size;
    }

    // override operator [] to access the array
    int& operator[](int i)
    {
        if (i >= size)
            printf("error index of array1D");
        return elements[i];
    }

    // Print out the elements in the array
    void print()
    {

    }

    // Add a number to the vector
    void add(int num)
    {

    }

    // Multiply the vector by a num
    void multiply(int num)
    {

    }

    // Multiply with another vector
    int multiply(Array1D array)
    {
        return 0;
    }
};

struct Array2DSize
{
    int rows;
    int cols;
};

class Array2D
{
private:
    Array2DSize size;
    Array1D* elements;

public:
    Array2D(int rows, int cols)
    {
        if (rows > 0) elements = new Array1D[rows];
        for (int i = 0; i < rows; ++i)
            elements[i].Resize(cols);
        size.rows = rows;
        size.cols = cols;
    }

    Array2D(const Array2D& array)
    {
        size = array.size;
        elements = new Array1D[size.rows];
        for (int i = 0; i < size.rows; ++i)
            elements[i] = array.elements[i];
    }

    virtual ~Array2D()
    {
        if (elements != 0)
            delete[] elements;
    }

    Array1D& operator[] (int i)
    {
        if (i >= size.rows)
            printf("error index of array2D");
        return elements[i];
    }

    void multiply (Array2D& array)
    {

    }

    Array2D add (Array2D& array)
    {

    }

    void transpose()
    {

    }

    Array2DSize Size()
    {
        return size;
    }

    void print()
    {

    }
};

void testArray()
{
    Array2D array2d1(2, 2);
    int i = 0;
    for (; i < array2d1.Size().rows; i++)
        for (int j = 0; j < array2d1.Size().cols; j++)
            array2d1[i][j] = i + j;
    printf("array2d1 is:\n");
    array2d1.print();

    Array2D array2d2(2, 2);
    for (i = 0; i < array2d2.Size().rows; i++)
        for (int j = 0; j < array2d2.Size().cols; j++)
            array2d2[i][j] = i + j;
    printf("array2d2 is:\n");
    array2d2.print();

    array2d1.multiply(array2d2);
    printf("array2d1 * array2d2 =\n");
    array2d1.print();
}


int main(int argc, char* argv[])
{
    testArray();
    return 0;
}
