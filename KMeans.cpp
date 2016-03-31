//
// Created by Ryan on 3/27/2016.
//
#include<iostream>
#include <cassert>
#include "KMeans.h"
#include "Cluster.h"
#include "Point.h"
#include "Exceptions.h"


using namespace std;


namespace Clustering {


    KMeans::KMeans(unsigned int dim, unsigned int k, std::string filename, unsigned int maxIter) {
        if (dim == 0)
            throw ZeroDimensionsEx();
        if (k == 0)
            throw ZeroClustersEx();

        __dimensionality = dim;
        __k = k;
        __maxIter = maxIter;
        __numIter = 0;
        __numNonempty = 0;
        __numMovesLastIter = 0;


        __clusters = new Cluster *[k];
        for (int i = 0; i < k; i++)
            __clusters[i] = new Cluster(dim);


        ifstream inFile;
        inFile.open(filename);
        if (inFile == NULL)
            throw DataFileOpenEx(filename);
        else
            inFile >> *(__clusters[0]);
        inFile.close();

        __initCentroids = new Point*[k];
        for (int i = 0; i < k; i++)
            __initCentroids[i] = new Point(__dimensionality);

        __clusters[0]->pickCentroids(k, __initCentroids);

        for (int i = 0; i < k; i++)
            __clusters[i]->centroid.set(*__initCentroids[i]);

    }

    KMeans::~KMeans() {
        for (int i = 0; i < __k; i++) {
            delete __clusters[i];
            delete __initCentroids[i];
        }
        delete[] __clusters;
        delete[] __initCentroids;
    }

    // element access (for testing, no bounds checking)
    Cluster &KMeans::operator[](unsigned int u) {
        return *__clusters[u];
    }

    const Cluster &KMeans::operator[](unsigned int u) const {
        if (u >= __k)
            throw OutOfBoundsEx(__k, u);
        else
            return *__clusters[u];
    }

    // write out the results to a file
    std::ostream &operator<<(std::ostream &os, const KMeans &kmeans) {


    }


    // clustering functions
    void KMeans::run() {
        /*unsigned int moves = 100, iter = 0;
        int shortCentroid = 0;
        bool isMoving = false;
        while(moves > 0 && iter < __maxIter){
            moves = 0;
            for(int clusterCount = 0; clusterCount < __k; clusterCount++){
                for(int pointCount = 0; pointCount < __k; pointCount++){
                    shortCentroid = clusterCount;
                    for(int centroidCount = 0; centroidCount < __k; centroidCount++){
                        if( shortCentroid!=centroidCount &&
                                (((*__clusters)[clusterCount])[pointCount].distanceTo(((*__clusters)[clusterCount]).centroid.get()))
                                < (((*__clusters)[clusterCount])[pointCount].distanceTo(((*__clusters)[shortCentroid]).centroid.get()))) {
                            shortCentroid = clusterCount;
                            isMoving = true;
                        }
                    }
                    if(isMoving){
                        Cluster::Move move(((*__clusters)[clusterCount])[pointCount], (*__clusters)[clusterCount],
                            (*__clusters)[shortCentroid]);
                        move.perform();
                        moves++;
                        pointCount--;
                        isMoving = false;

                    }
                }
            }
            for(int i = 0; i < __k; i++){
                if(!(*__clusters)[i].centroid.isValid())
                    (*__clusters)[i].centroid.compute();
            }
            iter++;
        }
        __numNonempty = 0;
        for(int i = 0; i <__k; i++){
            if((*__clusters)[i].getSize() > 0)
                __numNonempty++;
        }
        __numIter = iter;
        __numMovesLastIter = moves;*/
    }
}
