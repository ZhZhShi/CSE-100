#ifndef KDT_HPP
#define KDT_HPP

#include <math.h>     // pow, abs
#include <algorithm>  // sort, max, min
#include <iostream>   // cout, cin, and cerr
#include <limits>     // numeric_limits<type>::max()
#include <vector>     // vector<typename>
#include "BST.hpp"

using namespace std;

class Point {
   private:
    double delta = 0.00005;

   public:
    /** x and y point coordinates */
    double x, y;

    /** Default constructor: point at (0, 0) */
    Point() : x(0.0), y(0.0) {}

    /** constructor that defines point location */
    Point(double x, double y) : x(x), y(y) {}

    /** Equals operator */
    bool operator==(const Point &other) const {
        return abs(x - other.x) < delta && abs(y - other.y) < delta;
    }

    /** Not-equals operator */
    bool operator!=(const Point &other) const {
        return abs(x - other.x) > delta || abs(y - other.y) > delta;
    }

    /** Less than operator */
    bool operator<(const Point &other) const {
        cerr << "This operator should not be called, you will need "
             << "check the x and y components each time" << endl;
        return false;
    }

    /** Return the squre of the Euclidean distance between two points p1 and p2
     */
    static double squareDistance(const Point &p1, const Point &p2) {
	return pow((p1.x-p2.x),2)+pow((p1.y-p2.y),2);
    }
};

std::ostream &operator<<(std::ostream &out, const Point &data) {
    out << "(" << data.x << ", " << data.y << ")";
    return out;
}

bool xLessThan(const Point &p1, const Point &p2) { return p1.x < p2.x; }

bool yLessThan(const Point &p1, const Point &p2) { return p1.y < p2.y; }

class KDT : public BST<Point> {
   public:
    /** Given a vector of Point references, insert a copy into the BST.
     *  Return the number of elements that the fully constructed tree contains
     *  Note. Because of the nature of the KDTree, a simple < operator
     *        will not be sufficient.  You will need to check the x and y
     *        coordinates individually.  Two functions have been provided
     *        for your convenience above, (xLessThan and yLessThan). You may
     *        find them useful for the sort function from #include<algorithm>.
     *  Note. Since KDT inherits from BST, you still have access to the
     *        root, isize, and iheight instance variables and should modify
     *        them accordingly.
     *  Note. We have also provided a private helper method for the build
     *        function called buildSubset defined below.
     */
    virtual unsigned int build(vector<Point> &items) {
	if(items.empty()){
	    iheight=0;
	    isize=0;
	    return 0;
       }
	sort(items.begin(),items.end(),xLessThan);
	size_t size = items.size();
	unsigned int mid = 0;
	if(size%2 == 1)
	    mid = (size-1)/2;
	else {
	    while(size%2 == 0)
		size /= 2;
	    mid = (size-1)/2; 
	}
	root = new BSTNode<Point>(items[mid]);
	isize++;
	iheight = 1;
	root->parent = 0;
	root->left = buildSubset(items,0,mid,1,1);
	root->right = buildSubset(items,mid+1,items.size(),1,1);
	return isize;
    }

    /** Find the nearest neighbor to a given point
     *  Returns the point in the kd-tree nearest to the parameter item.
     *  Note. THIS IS DIFFERENT THAN HOW THE PREVIOUS FIND FUNCTION WORKS
     *  Note. Since KDT inherits from BST, you still have access to the
     *        root, isize, and iheight member variables.
     *  Note. The dimension ordering you used to build the tree must be used
     *        to search through it.
     *  Note. We have added a private helper method for your convenience called
     *        findNNHelper defined below.
     */
    // TODO
    virtual iterator findNearestNeighbor(const Point &item) const {
	if (isize==0) return 0;
	double dis = 0;
	BSTNode<Point> *ret = 0;
	findNNHelper(root,item,&dis,&ret,0);
	return iterator(ret);
    }

    /** For the kd-tree, the find method should not be used.  Use the function
     *  findNearestNeighbor instead.
     */
    virtual iterator find(const Point &item) const override {
        cerr << "The KD Tree find method should not be called" << endl;
        cerr << "Use findNearestNeighbor instead" << endl;
        return 0;
    }

    /** For the kd-tree, the insert method should not be used.  The tree is
     *  built all at once.  There should be no scenario where you add individual
     * points.
     */
    virtual bool insert(const Point &item) override {
        cerr << "The KD Tree insert method should not be called" << endl;
        return false;
    }

   private:
    /** Recursively add a subset of the array to the KD tree
     *  Parameters items: the list of points that are used to build the kd tree
     *             start: the starting index for this subsection of the tree
     *             end: the non-inclusive ending index for this subtree
     *             dimension: 0 if sorting along the x-axis, and 1 if sorting
     *                        along the y-axis
     *  PostConditions: the parent node should have its left and right children
     *                  set if there are any available points and recursive
     *                  calls should be made for the left and right subtree
     *                  (again, if there are any remaining points).
     *  NOTE: We gave you two methods: xLessThan and yLessThan.  We expect you
     *        that you will find these methods very helpful.  For your sake,
     *        please keep this in mind.
     */
    BSTNode<Point> *buildSubset(vector<Point> items, unsigned int start,
                                unsigned int end, unsigned int dimension,
                                unsigned int height) {
	unsigned int mid;
	BSTNode<Point> *curr;
	// No element
	if(start == end) return 0;	
	// Sort the vectoer
	if(end-start != 1) {
	    if(dimension == 0) sort(items.begin()+start, items.begin()+end, xLessThan);
	    else sort(items.begin()+start, items.begin()+end, yLessThan);
	}
	// Find mid
	if((end-start)%2 == 0) {
	    mid = end-start;
	    while(mid%2 == 0)
		mid /= 2;
	    mid = start+(mid-1)/2;
	}
	else mid = (end+start-1)/2;
        // Insert		
	curr = new BSTNode<Point>(items[mid]);
	isize++;
	height++;
	curr->left = buildSubset(items,start,mid,(dimension+1)%2,height);
	if(curr->left) curr->left->parent = curr;
	curr->right = buildSubset(items,mid+1,end,(dimension+1)%2,height);
    	if(curr->left) curr->right->parent = curr;
	if(height > iheight) iheight = height;
	return curr;
    }

    /** Find the node in the subtree with that is closest to the given point p
     *  Parameters: node the root of the subtree being searched
     *              queryPoint the point whose nearest neighbor should be found
     *              smallestSquareDistance the smallest distance value found in
     *                                     the tree so far which can be updated
     *              closestPoint the node corresponding to the current closest
     *                           node in the tree which can be udpated
     *              dimension flags whether the node parameter splits remaining
     *                        nodes in its subtree by the x or y dimension
     *                        (has the value of either 0 or 1)
     *  Return: a BSTNode object that represents the point in the subtree
     *          closest to the given point p (including the root node)
     */
    // TODO
    void findNNHelper(BSTNode<Point> *node, const Point &queryPoint,
                      double *sdd,
                      BSTNode<Point> **closestPoint,
                      unsigned int dimension) const {
	if(!node){
	    return;
	}
	if(!dimension){
	    if(xLessThan(queryPoint,node->data)){
		findNNHelper(node->left,queryPoint,sdd,closestPoint,1);
	    } else{
    	        findNNHelper(node->right,queryPoint,sdd,closestPoint,1);
	    }
	} else{
	    if(yLessThan(queryPoint,node->data)){
		findNNHelper(node->left,queryPoint,sdd,closestPoint,0);
	    } else{
		findNNHelper(node->right,queryPoint,sdd,closestPoint,0);
	    }
	}
	double curr = Point::squareDistance(queryPoint,node->data);
	if(!*closestPoint || curr<*sdd){
	    *closestPoint = node;
	    *sdd = curr;
	}
	if(!dimension){
	    if(xLessThan(queryPoint,node->data)){
		if(pow((node->data.x-queryPoint.x),2)<*sdd)
		    findNNHelper(node->right,queryPoint,sdd,closestPoint,1);
	    } else{
		if(pow((node->data.x-queryPoint.x),2)<*sdd)
		    findNNHelper(node->left,queryPoint,sdd,closestPoint,1);
	    }
	}else{
	    if(yLessThan(queryPoint,node->data)){
		if(pow((node->data.y-queryPoint.y),2)<*sdd)
		    findNNHelper(node->right,queryPoint,sdd,closestPoint,1);
	    } else{
		if(pow((node->data.y-queryPoint.y),2)<*sdd)
		    findNNHelper(node->left,queryPoint,sdd,closestPoint,1);
	    }
	}
    }
};

#endif  // KDT_HPP
