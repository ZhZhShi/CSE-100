#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <vector>
#include "../src/KDT.hpp"

using namespace std;

/**
 * A simple test driver for the BST class template.
 */
int main() {
    /* test Point.distance() */
    double distance = Point::squareDistance(Point(1.0, 2.0), Point(1.0, 3.0));
    if (distance != 1.0) {
        cout << "Incorrect value for Point.distance" << endl
             << "\texpected: " << 1.0 << endl
             << "\tactual: " << distance << endl;
        return -1;
    }

    distance = Point::squareDistance(Point(-1.0, 4.0), Point(1.0, 4.0));
    if (distance != 4.0) {
        cout << "Incorrect value for Point.distance" << endl
             << "\texpected: " << 4.0 << endl
             << "\tactual: " << distance << endl;
        return -1;
    }

    distance = Point::squareDistance(Point(-1.0, 2.0), Point(0.0, 1.0));
    if (distance != 2.0) {
        cout << "Incorrect value for Point.distance" << endl
             << "\texpected: " << 2.0 << endl
             << "\tactual: " << distance << endl;
        return -1;
    }
    /* test build */
    vector<Point> v;
    v.push_back(Point(1, 8));
    v.push_back(Point(2, 4));
    v.push_back(Point(2, 9));
    v.push_back(Point(3, 3));
    v.push_back(Point(5, 3));
    v.push_back(Point(6, 7));

    KDT kdtree;
    unsigned int num_added = kdtree.build(v);
    if (num_added != v.size()) {
        cout << "Incorrect number of elements added to the kd tree" << endl
             << "\texpected: " << v.size() << endl
             << "\tactual: " << num_added << endl;
        return -1;
    }
    /* Test size. */

    cout << "Size is: " << kdtree.size() << endl;
    if (kdtree.size() != v.size()) {
        cout << "... which is incorrect." << endl;
        return -1;
    }

    /* Test height */
    cout << "Height is: " << kdtree.height() << endl;
    if (kdtree.height() != 4) {
        kdtree.inorder();
        cout << "... which is incorrect." << endl;
        return -1;
    }
    /* Test find */

    vector<Point>::iterator vit = v.begin();
    vector<Point>::iterator ven = v.end();

    vit = v.begin();
    for (; vit != ven; ++vit) {
        if (*(kdtree.findNearestNeighbor(*vit)) != *vit) {
            cout << "Incorrect return value when finding " << *vit << endl;
            return -1;
        }
    }

    vector<Point> nearby_points;
    nearby_points.push_back(Point(1, 8));
    nearby_points.push_back(Point(2, 4));
    nearby_points.push_back(Point(2, 9));
    nearby_points.push_back(Point(3, 3));
    nearby_points.push_back(Point(5, 3));   
    nearby_points.push_back(Point(6, 7)); 
      

    vector<Point>::iterator nvit = nearby_points.begin();
    vit = v.begin();
    for (; nvit != nearby_points.end() && vit != ven; nvit++, vit++) {
        if (*(kdtree.findNearestNeighbor(*nvit)) != *vit) {
            cout << "Incorrect return value when finding " << *vit << endl;
            return -1;
        }
    }

    vector<Point> own;
    own.push_back(Point(4,7));
    vector<Point>::iterator ovit = own.begin();
    cerr<<*kdtree.findNearestNeighbor(*ovit);

    cout << "OK" << endl;

    return 0;
}
