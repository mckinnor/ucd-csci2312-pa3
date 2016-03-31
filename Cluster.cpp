//
// Created by Ryan on 3/15/2016.
//
#include <iostream>
#include <cassert>
#include <limits>
#include <sstream>
#include <iomanip>
#include "Cluster.h"
#include "Exceptions.h"
#include "Point.h"

using namespace std;

namespace Clustering {
    unsigned int Cluster::__idGenerator = 0;

    LNode::LNode(const Point &p, LNodePtr n):point(p) {
        Point point = p;
        LNodePtr next = n;
    }

    Cluster::Centroid::Centroid(unsigned int d, const Cluster &c): __p(d), __c(c){
         __dimensions = d;  // how many dimensions the centroid point needs to occupy, 2, 3, 50 etc
         __valid = false;
    }

    void Cluster::Centroid::set(const Clustering::Point &p){ // sets to valid{
         __p = p;
         __valid = true;
    }

    void Cluster::Centroid::compute(){
        if(__c.__size == 0)
            toInfinity();
        else {
            LNodePtr roaches = __c.__points;
            double avg, rawSum = 0;
            unsigned int count = 0;


            for (int i = 0; i < __dimensions; i++) {
                while (roaches != NULL) {
                    rawSum += roaches->point[i];
                    count++;
                    roaches = roaches->next;
                }
                avg = rawSum / count;
                __p[i] = avg;
            }
            setValid(true);
        }
    }


    bool Cluster::Centroid::equal(const Point &zealot) const{
        for(int i = 0; i < __dimensions; i++){
            if(__p[i] != zealot[i])
                return false;
        }
        return true;
    }


    void Cluster::Centroid::toInfinity(){
        for(int i = 0; i<__dimensions; i++)
            __p[i] = numeric_limits<double>::max();
        setValid(true);
    }


    Cluster::Cluster(unsigned int d):centroid(d, *this){
        __id = __idGenerator++;
        __dimensionality = d;
        __size = 0;
        __points = nullptr;
    }

    // The big three: cpy ctor, overloaded operator=, dtor

    Cluster::Cluster(const Cluster &rhs) : centroid(rhs.__dimensionality, *this){
        /*__size = rhs.__size;
        __id = rhs.__id;
        __size = rhs.__size;
        __points = rhs.__points;
        __dimensionality = rhs.__dimensionality;*/
        __dimensionality = rhs.__dimensionality;
        __points = NULL;
        __size = 0;
        for(int i = 0; i < rhs.__size; i++)
            add(rhs[i]);
        __id = rhs.__id;

    }


    Cluster &Cluster::operator=(const Cluster &rhs){
        if(__dimensionality != rhs.__dimensionality)
            throw DimensionalityMismatchEx(__dimensionality, rhs.__dimensionality);
        else {
            if (this == &rhs)
                return *this;
            else {
                if (__points != NULL) {
                    LNodePtr cursor = __points;
                    LNodePtr nextPtr;
                    while (cursor != NULL) {
                        nextPtr = cursor->next;
                        delete cursor;
                        cursor = nextPtr;

                    }
                    __size = 0;
                    __points = NULL;
                }
                for(int i = 0; i < rhs.__size; i++)
                    add(rhs[i]);
            }


            /*LNodePtr rhsPtr = rhs.__points;
            for (int i = 0; i < rhs.getSize(); i++) {
                add(rhsPtr->point);
                if (rhsPtr->next != NULL)
                    rhsPtr = rhsPtr->next;
            }*/
            __id = rhs.__id;
            return *this;
        }

    }

    Cluster::~Cluster(){
        __points = NULL;
        __size = 0;
    }


    void Cluster::__del(){
        if (__size != 0)
        {
            LNodePtr cursor = __points;
            delete cursor;
        }
        __points = NULL;
        __size = 0;
    }


    void Cluster:: __cpy(LNodePtr pts){
        LNodePtr cursor = pts;
        this->__size = 0;

        while (cursor != NULL){
            this->add(cursor->point);
            cursor = cursor->next;
            __size++;
        }
    }


    bool Cluster::__in(const Point &p) const{
    }

 // Set functions: They allow calling c1.add(c2.remove(p));
    void Cluster::add(const Point &p){ // TODO add asc order to the requirements
         LNodePtr newPtr = new LNode(p, NULL);
         LNodePtr cursor = __points;
         LNodePtr prev;
         newPtr->point = p;


        // Check to see if this will be the first node in the list
        if (__points == NULL) {
            assert(__size == 0);
            __points = newPtr;
            newPtr->next = NULL;
        }

        // Replace the head point if p is smaller
        else if (__points->point >= p) {
            cursor = __points;
            __points = newPtr;
            __points->next = cursor;
        }

        // Or add another node to the existing list
        else {
            cursor = __points;
            prev = NULL;
            while(cursor != NULL && cursor->point < p && cursor->point != p){
                prev = cursor;
                cursor = cursor->next;
            }
            if(cursor == NULL){
                prev->next = newPtr;
                newPtr->next = NULL;
            }
            else if(cursor->point == p){
                prev = cursor;
                cursor = cursor->next;
                prev->next = newPtr;
                newPtr->next = cursor;
            }
            else if(cursor->point > p){
                prev->next = newPtr;
                newPtr->next = cursor;
            }

        }
        __size++;
        centroid.setValid(false);
    }


     const Point &Cluster::remove(const Point &p)
     {
         LNodePtr cursor;
         LNodePtr prevPtr = __points;


         // empty list
         if (__points == NULL)
             return p;


         // head of list
         if (__points->point == p)
         {
             cursor = __points->next;
             delete __points;
             __points = cursor;
         }


             // anywhere else
         else{
             cursor = __points;
             while (cursor != NULL && cursor->point != p){
                 prevPtr = cursor;
                 cursor = cursor->next;
             }
             if (cursor->point == p){
                 prevPtr->next = cursor->next;
                 delete cursor;
             }
             else{
                 cout << "That point wasn't found in the linked list" << endl;
             }
         }
         __size--;
         centroid.setValid(false);
         return p;
     }


     bool Cluster::contains(const Point &p) const{
         bool answer = false;
         LNodePtr cursor = __points;
         if (__dimensionality != p.getDims())
             throw DimensionalityMismatchEx(__dimensionality, p.getDims());

         for (cursor = __points; cursor != NULL; cursor = cursor->next){
             if (cursor->point == p)
                 return true;
         }
         return false;
     }


     void Cluster::pickCentroids(unsigned int k, Point **pointArray){
         if(k >= __size){
            for(int i = 0; i < __size; i++){
                *pointArray[i] = (*this)[i];
            }
            if(k > __size){
                for(unsigned int i = __size; i < k; i++){
                    for(unsigned int j = 0; j <__dimensionality; j++)
                        pointArray[i]->setValue(j, numeric_limits<double>::max());
                }
            }
         }
         else if(k < __size){
             for(int i = 0; i < k; i++){
                 *pointArray[i] = (*this)[i];
             }
         }
     }
 // Overloaded operators


// Members: Subscript
     const Point &Cluster::operator[](unsigned int index) const{ // notice: const
         LNodePtr cursor = __points;
         if (__size == 0)
             throw EmptyClusterEx();
         if (index >= __size)
             throw OutOfBoundsEx(index, __size);
         if (cursor != NULL)
         {
             for (int i=0; i<index; i++){
                cursor = cursor->next;
             }
         }
         return cursor->point;
     }

    Cluster &Cluster::operator+=(const Point &more) {
        if(__dimensionality != more.getDims())
            throw DimensionalityMismatchEx(__dimensionality, more.getDims());
        else {
            add(more);
            this->centroid.setValid(false);
        }
    }


    Cluster &Cluster::operator-=(const Point &less){
        if(__dimensionality != less.getDims())
            throw DimensionalityMismatchEx(__dimensionality, less.getDims());
        else {
            remove(less);
            this->centroid.setValid(false);
        }
    }

    //CLUSTER OPERATIONS
    //Cluster Union
    Cluster &Cluster::operator+=(const Cluster &second){
        if(__dimensionality != second.__dimensionality)
            throw DimensionalityMismatchEx(__dimensionality, second.__dimensionality);
        else {
            LNodePtr cursor  = second.__points;
            LNodePtr compCursor = __points;
            bool runTruth = false;
            if(*this != second || this->getId() != second.getId()){
                while(cursor != NULL){
                    while(compCursor != NULL){
                        if(cursor->point != compCursor->point || cursor->point.getId() != compCursor->point.getId())
                            runTruth = false;
                        else {
                            runTruth = true;
                            break;
                        }
                        compCursor = compCursor->next;
                    }
                    if(runTruth == false) {
                        add(cursor->point);
                    }
                    cursor = cursor->next;
                    compCursor = __points;
                }
            }

        }
    }

    // (asymmetric) difference
    Cluster &Cluster::operator-=(const Cluster &second){
        if(__dimensionality != second.__dimensionality)
            throw DimensionalityMismatchEx(__dimensionality, second.__dimensionality);
        else {
            LNodePtr cursor = second.__points;
            LNodePtr compCursor = __points;
            bool runTruth = false;
            while (cursor != NULL) {
                while (compCursor != NULL) {
                    if (compCursor->point == cursor->point && compCursor->point.getId() == cursor->point.getId()){
                        runTruth = true;
                        break;
                    }
                    compCursor = compCursor->next;
                }
                if(runTruth == true)
                    remove(cursor->point);
                cursor = cursor->next;
                compCursor = __points;
            }
        }
    }


 // Friends: IO
    std::ostream &operator<<(std::ostream &out, const Cluster &c){
        LNodePtr cursor = c.__points;
        while(cursor != NULL){
            out << std::setprecision(2);
            out << cursor->point << ' ' << Clustering::Cluster::POINT_CLUSTER_ID_DELIM << c.__id;
            out << endl;
            cursor = cursor->next;
        }
    }


     std::istream &operator>>(std::istream &in, Clustering::Cluster &c){
         while(!(in.eof())){
             string string1;
             getline(in, string1);
             if(string1.length() > c.__dimensionality){
                 Clustering::Point newPoint(c.__dimensionality);
                 stringstream stringstream1(string1);

                 try{
                     stringstream1 >> newPoint;
                     c.add(newPoint);
                 }
                 catch(Clustering::DimensionalityMismatchEx &ex){
                     cerr << ex << endl;
                     newPoint.rewindIdGen();
                 }
                 /*catch(Clustering::DimensionalityMismatchEx &ex){
                     cerr << ex << endl;
                     newPoint.rewindIdGen();
                 }*/
             }
         }
        return in;
     }


 // Friends: Comparison
     bool operator==(const Cluster &lhs, const Cluster &rhs){
         bool answer = true;
         if (lhs.__size != rhs.__size)
             return false;
         LNodePtr Lcursor = lhs.__points;
         LNodePtr Rcursor = rhs.__points;
         while (Lcursor != NULL) {
             if (Lcursor->point != Rcursor->point) {
                 answer = false;
             }
             else {
                 Rcursor = Rcursor->next;
                 Lcursor = Lcursor->next;
             }
         }
         return answer;
     }



     bool operator!=(const Cluster &lhs, const Cluster &rhs){
         bool answer = false;

         if (lhs.__dimensionality != rhs.__dimensionality)
             throw DimensionalityMismatchEx(lhs.__dimensionality, rhs.__dimensionality);


         if (lhs.__size != rhs.__size){
             answer = true;
             return answer;
         }

         LNodePtr Lcursor = lhs.__points;
         LNodePtr Rcursor = rhs.__points;
         while (Lcursor != NULL && Rcursor != NULL){
             if (Lcursor->point == Rcursor->point){
                 Rcursor = Rcursor->next;
                 Lcursor = Lcursor->next;
             }
             else
                 answer = true;
         }
         return answer;
     }


 // Friends: Arithmetic (Cluster and Point)
    const Cluster operator+(const Cluster &c, const Point &p){
        Cluster newCluster(c);
        newCluster += p;
        return newCluster;
    }

    const Cluster operator-(const Cluster &c, const Point &p){
        Cluster newCluster(c);
        newCluster -= p;
        return newCluster;
    }

 // Friends: Arithmetic (two Clusters)
    const Cluster operator+(const Cluster &one, const Cluster &two) {
        Cluster newCluster(one);
        newCluster += two;
        return newCluster;
    }


    const Cluster operator-(const Cluster &minuend, const Cluster &subtrahend){ // (asymmetric) difference
        Cluster newCluster(minuend);
        newCluster -= subtrahend;
        return newCluster;
    }

    void Cluster::Move::perform(){
        __to.add(__from.remove(__p));
    }

    Clustering::Cluster::Move::Move(const Clustering::Point &p, Clustering::Cluster &from, Clustering::Cluster &to): __p(p), __from(from), __to(to) {
    }
}