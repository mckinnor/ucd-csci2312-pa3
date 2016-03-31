//
// Created by Ryan on 3/27/2016.
//

#include<iostream>
#include "Exceptions.h"
#include "Point.h"
#include "Cluster.h"
#include "KMeans.h"

namespace Clustering{
    // Out of Bounds Exceptions
    OutOfBoundsEx::OutOfBoundsEx(unsigned int c, int r) {
     __current = c;
     __rhs = r;
     __name = "OutOfBoundsEx";
    }


    unsigned int OutOfBoundsEx::getCurrent() const {
         return __current;
    }


    int OutOfBoundsEx::getRhs() const {
         return __rhs;
    }


    std::string OutOfBoundsEx::getName() const {
        return __name;
    }


     std::ostream &operator<<(std::ostream &os, const OutOfBoundsEx &ex) {
         os << "Current side of " << ex.__current << " is out of bounds of the size " << ex.__rhs << std::endl;
         return os;
     }


     // Dimensionality exceptions
     DimensionalityMismatchEx::DimensionalityMismatchEx(unsigned int c, unsigned int r){
         __current = c;
         __rhs = r;
         __name = "DimensionalityMismatchEx";
     }


     unsigned int DimensionalityMismatchEx::getCurrent() const{
         return __current;
     }


     unsigned int DimensionalityMismatchEx::getRhs() const{
         return __rhs;
     }


     std::string DimensionalityMismatchEx::getName() const{
         return __name;
     }


     std::ostream &operator<<(std::ostream &os, const DimensionalityMismatchEx &ex){
         os << "Current size of " << ex.__current << " doesn't match the other side of " << ex.__rhs << std::endl;
         return os;
     }


     //Zero Clusters Exceptions
     ZeroClustersEx::ZeroClustersEx(){
         __name = "ZeroClustersEx";
     }

     std::ostream &operator<<(std::ostream &os, const ZeroClustersEx &ex){
        os << "There are no clusters to be made" << std::endl;
        return os;
     }


     // Data file open exceptions
     DataFileOpenEx::DataFileOpenEx(std::string filename) {
         __filename = filename;
         __name = "DataFileOpenEx";
     }

     std::ostream &operator<<(std::ostream &os, const DataFileOpenEx &ex){
         os << "The file name could not be found or the file was otherwise unable to be opened." << std::endl;
         return os;
     }


     //zero Dimensions Exceptions
     ZeroDimensionsEx::ZeroDimensionsEx(){
         __name = "ZeroDimensionsEx";
     }


     std::string ZeroDimensionsEx::getName() const{
         return __name;
     }


     std::ostream &operator<<(std::ostream &os, const ZeroDimensionsEx &ex)
     {
         os << "The current point has no dimensions" << std::endl;
         return os;
     }


//Empty Cluster Exceptions
     EmptyClusterEx::EmptyClusterEx()
     {
         __name = "EmptyClusterEx";
     }


     std::string EmptyClusterEx::getName() const
     {
         return __name;
     }


     std::ostream &operator<<(std::ostream &os, const EmptyClusterEx &ex)
     {
         os << "The cluster contains no points" << std::endl;
         return os;
     }


}
