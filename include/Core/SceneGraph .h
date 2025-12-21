#pragma once
#include "GameObject.h"

struct Node {
    GameObject* parent;
    std::vector<Node*> children;
};

class SceneGraph {
    std::vector<Node*> rootNodes;

    void AddNode(Node* node) {
        rootNodes.push_back(node);
    }

    static void AddChildren(Node* parent , Node* child ) {
        parent->children.push_back(child);
    }
};

