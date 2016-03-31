//
// Created by Ryan on 3/15/2016.
//
#include <iostream>
#include <sstream>
#include <cmath>
#include <fstream>
#include <bits/stl_algo.h>
#include "Point.h"
#include "Cluster.h"
#include "Exceptions.h"

using namespace std;

namespace Clustering {

    unsigned int Point::__idGen = 0; // ID generator

    void Point:: rewindIdGen() {
        __idGen--;
    }

    Point::Point(unsigned int size){
        if (size <= 0)
            throw ZeroDimensionsEx();
        __id = __idGen++;
        __dim = size;
        __values = new double[size];
        for (int i = 0; i < size; i++)
             __values[i] = 0;
    }

    // Big three: cpy ctor, overloaded operator=, dtor
    Point::Point(const Point &rhs):__dim(rhs.__dim){
         //__id = __idGen++;
        __id = rhs.__id;
        __idGen++;
        __values = new double[__dim];
        for (int i = 0; i < __dim; i++)
            __values[i] = rhs.__values[i];
    }

     Point &Point::operator=(const Point &rhs)
     {
         //__id = __idGen++;
         __id = rhs.__id;
         __idGen++;
         if (__dim > rhs.__dim)
             throw DimensionalityMismatchEx(__dim, rhs.__dim);
         if (this == &rhs)
             return *this;
         else
         {
             this->__dim = rhs.__dim;
             this->__values = new double[__dim];
             for (int i = 0; i < __dim; i++) {
                 this->__values[i] = rhs.__values[i];
             }
         }
         return *this;
     }


     Point::~Point(){
         delete[] __values;
         rewindIdGen();
     }

     void Point::setValue(unsigned int i, double num){
         if (i >= __dim || i < 0)
             throw OutOfBoundsEx(__dim, i);
         else
            __values[i] = num;
     }

     double Point::getValue(unsigned int i) const{
         if (i >= __dim || i < 0)
             throw OutOfBoundsEx(__dim, i);
         else
            return __values[i];
     }


// Functions
     double Point::distanceTo(const Point &rhs) const{
         if (__dim != rhs.__dim)
             throw DimensionalityMismatchEx(__dim, rhs.__dim);
         double distance = 0;
         double squared;
         int size;
         size = rhs.__dim;
         for (int i=0; i<size; i++)
         {
             if (rhs.__values[i] == __values[i])
             {
                 distance += 0;
             }
             else
             {
                 squared = rhs.__values[i] - __values[i];
                 distance += squared * squared;
             }
         }
         distance = sqrt(distance);
         return distance;
     }

    //OVERLOADED OPERATORS
     Point &Point::operator*=(double num){// p *= 6; p.operator*=(6);
        int size = __dim;
        for (int i=0; i < size; i++){
             __values[i] = __values[i] * num;
        }
     }


     Point &Point::operator/=(double num){
         int size = __dim;
         for (int i=0; i < size; i++){
             __values[i] /= num;
         }
     }


     const Point Point::operator*(double num) const{// prevent (p1 * 2) = p2;
         unsigned int size = __dim;
         Point p(size);


         for (int i=0; i<__dim; i++)
         {
             p.__values[i] = __values[i] * num;
         }
         return p;
     }


     const Point Point::operator/(double num) const // p3 = p2 / 2;
     {
         unsigned int size = __dim;
         Point p(size);


         for (int i=0; i<__dim; i++)
         {
             p.__values[i] = __values[i] / num;
         }
         return p;
     }


     double &Point::operator[](unsigned int index){
         if (index >= __dim)
             throw OutOfBoundsEx(index, __dim);
         else
            return __values[index];
     }


     const double &Point::operator[](unsigned int index) const{
         if (index >= __dim)
             throw OutOfBoundsEx(index, __dim);
         else
            return __values[index];
     }

    //FRIENDS
    Point &operator+=(Point &lhs, const Point &rhs)
    {
        if (lhs.__dim != rhs.__dim)
             throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
        for (int i=0; i <rhs.__dim; i++){
            lhs.__values[i] += rhs.__values[i];
        }
        return lhs;
    }


     Point &operator-=(Point &lhs, const Point &rhs){
         if (lhs.__dim != rhs.__dim)
             throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
         int size = lhs.__dim;
         for (int i=0; i < size; i++)
         {
             lhs.__values[i] -= rhs.__values[i];
         }
         return lhs;
     }

     const Point operator+(const Point &lhs, const Point &rhs){
         if (lhs.__dim != rhs.__dim)
             throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
         int size = (lhs.__dim + rhs.__dim);
         Point p(size);
         for (int i=0; i < size; i++)
         {
             p.__values[i] = lhs.__values[i] + rhs.__values[i];
         }
         return p;
     }


     const Point operator-(const Point &lhs, const Point &rhs){
         if (lhs.__dim != rhs.__dim)
             throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
         int size = (lhs.__dim + rhs.__dim);
         Point p(size);
         for (int i=0; i < size; i++)
         {
             p.__values[i] = lhs.__values[i] - rhs.__values[i];
         }
         return p;
     }


     bool operator==(const Point &lhs, const Point &rhs){
         bool answer = false;
         if (lhs.getDims() == rhs.getDims()){
             int dimensions = lhs.getDims();
             answer = true;
             for (int i=0; i < dimensions; i++)
             {
                 if (lhs.__values[i] != rhs.__values[i])
                 {
                     answer = false;
                     return answer;
                 }
             }
         }
         return answer;
     }


     bool operator!=(const Point &lhs, const Point &rhs){
         return(!(lhs ==rhs));
     }


     bool operator<(const Point &lhs, const Point &rhs){
         if (lhs.__dim != rhs.__dim)
             throw DimensionalityMismatchEx(lhs.__dim, rhs.__dim);
         for (int i=0; i < lhs.__dim; i++){
             if ( lhs.__values[i] != rhs.__values[i]){
                 if(lhs.__values[i] < rhs.__values[i])
                     return true;
                 else return false;
             }
         }
     }

     bool operator>(const Point &lhs, const Point &rhs){
         return (!(lhs < rhs) && lhs != rhs);
     }

     bool operator<=(const Point &lhs, const Point &rhs){
         return(lhs < rhs || lhs == rhs);
     }

     bool operator>=(const Point &lhs, const Point &rhs){
         return( lhs > rhs || lhs == rhs);
     }


     std::ostream &operator<<(std::ostream &os, const Point &p)
     {
         int i=0;
         for (int index = 0; index < p.__dim-1; index++ )
         {
             os << p.__values[i] << p.POINT_VALUE_DELIM;
             i++;
         }
         os << p.__values[i];
         return os;
     }


     std::istream  &operator>>(std::istream &is, Clustering::Point &p) {
         double d;
         unsigned int count = 0;
         string value;
         unsigned int i = 0;

         while (getline(is, value, p.POINT_VALUE_DELIM)){
             d = std::stod(value);
             if ( i < p.__dim){
                 p.setValue(i++, d);
             }
             count++;
         }
         if (count != p.__dim)
             throw Clustering::DimensionalityMismatchEx(i, p.__dim);
         return is;
    }
}
