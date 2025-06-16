#pragma once
#include "../geometry/geometry.h"
#include <limits>
#include <cmath>

class DNN {
public:
    struct KDNode {
        Point pt;
        KDNode* left;
        KDNode* right;
        int depth;

        KDNode(const Point& p, int d)
                : pt(p), left(nullptr), right(nullptr), depth(d) {}
    };

    KDNode* root;

    DNN() : root(nullptr) {}

    void insert(const Point& p) {
        root = insertRec(root, p, 0);
    }

    Point nearest(const Point& p) {
        Point best;
        double bestDist = std::numeric_limits<double>::max();
        nearestRec(root, p, best, bestDist);
        return best;
    }

private:
    KDNode* insertRec(KDNode* node, const Point& p, int depth) {
        if (!node) return new KDNode(p, depth);

        int axis = depth % 2;
        if ((axis == 0 && p.x < node->pt.x) || (axis == 1 && p.y < node->pt.y)) {
            node->left = insertRec(node->left, p, depth + 1);
        } else {
            node->right = insertRec(node->right, p, depth + 1);
        }
        return node;
    }

    void nearestRec(KDNode* node, const Point& target, Point& best, double& bestDist) {
        if (!node) return;

        double d = (node->pt.x - target.x)*(node->pt.x - target.x) +
                   (node->pt.y - target.y)*(node->pt.y - target.y);

        if (d < bestDist) {
            bestDist = d;
            best = node->pt;
        }

        int axis = node->depth % 2;
        double diff = (axis == 0) ? (target.x - node->pt.x) : (target.y - node->pt.y);

        KDNode* nearChild = (diff < 0) ? node->left : node->right;
        KDNode* farChild  = (diff < 0) ? node->right : node->left;

        nearestRec(nearChild, target, best, bestDist);

        if (diff*diff < bestDist) {
            nearestRec(farChild, target, best, bestDist);
        }
    }
};
