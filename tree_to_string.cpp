/*        10
         /  \
        5    13
       / \  /  \
      1  7 8    19
                  \
                   20
*/
// "nodeid<id>,value<int_to_str>,left<id>,rigth<id>;"
#include "pch.h"

#include <string>

const std::string valueSeparator(",");
const std::string nodeSeparator(";");
int nodeIdInt = 0;
struct Node
{
    Node* left;
    Node* right;
    int value;
};

std::string treeToString(Node* node, std::string& nodeId)
{
    std::string strRepresentation;
    if (node == nullptr)
        return strRepresentation;
    nodeId = std::to_string(++nodeIdInt);
    std::string leftmp;
    std::string rightmp;
    std::string leftId;
    std::string rightId;
    if (node->left != nullptr)
    {
        leftmp = treeToString(node->left, leftId);
    }
    if (node->right != nullptr)
    {
        rightmp = treeToString(node->right, rightId);
    }
    if (node->right == nullptr && node->left == nullptr)
    {
        strRepresentation += "nodeid";
        strRepresentation += nodeId;
        strRepresentation += valueSeparator;
        strRepresentation += "value";
        strRepresentation += std::to_string(node->value);
        strRepresentation += valueSeparator;
        strRepresentation += "left";
        strRepresentation += "0";
        strRepresentation += valueSeparator;
        strRepresentation += "right";
        strRepresentation += "0";
        strRepresentation += nodeSeparator;
    }
    else
    {
        strRepresentation += "nodeid";
        strRepresentation += nodeId;
        strRepresentation += valueSeparator;
        strRepresentation += "value";
        strRepresentation += std::to_string(node->value);
        strRepresentation += valueSeparator;
        if (!leftId.empty())
        {
            strRepresentation += "left";
            strRepresentation += leftId;
            strRepresentation += valueSeparator;
        }
        else
        {
            strRepresentation += "left";
            strRepresentation += "0";
            strRepresentation += valueSeparator;
        }
        if (!rightId.empty())
        {
            strRepresentation += "right";
            strRepresentation += rightId;
            strRepresentation += nodeSeparator;
        }
        else
        {
            strRepresentation += "right";
            strRepresentation += "0";
            strRepresentation += nodeSeparator;
        }
        if (!leftId.empty())
        {
            strRepresentation += leftmp;
        }
        if (!rightId.empty())
        {
            strRepresentation += rightmp;
        }
    }
    return strRepresentation;
}

// "nodeid<id>,value<int_to_str>,left<id>,rigth<id>;"
std::string getValue(const std::string& subtree, const std::string& tree)
{
    size_t valuebegin = subtree.find("value");
    if (valuebegin == std::string::npos)
        return "";
    size_t valueend = subtree.find(valueSeparator, valuebegin + strlen("value"));
    if (valueend == std::string::npos)
    {
        return "";
    }

    return subtree.substr(valuebegin + strlen("value"), valueend - (valuebegin + strlen("value")));
}

size_t get(const std::string& itemName, const std::string& tree)
{
    size_t pos = tree.find(itemName);
    if (pos == std::string::npos)
        return pos;
    size_t sep = std::string::npos;
    if (itemName == "left")
        sep = tree.find(valueSeparator, pos);
    if (itemName == "right")
        sep = tree.find(nodeSeparator, pos);
    if (sep == std::string::npos)
        return sep;
    std::string findnodeid = "nodeid" + tree.substr(pos + itemName.size(), sep - (pos + itemName.size()));
    return tree.find(findnodeid);
}

Node CreateNode(int value, Node* left, Node* right)
{
    Node ret;
    ret.value = value;
    ret.left = left;
    ret.right = right;
    return ret;
}

Node* stringToTree(const std::string& subtree, const std::string& root)
{
    Node* node;
    if (root.empty())
        return nullptr;
    Node* leftmp = nullptr;
    Node* rightmp = nullptr;
    size_t leftId = get("left", subtree);
    if (leftId != std::string::npos)
    {
        leftmp = stringToTree(subtree.substr(leftId), root);
    }
    size_t rightId = get("right", subtree);
    if (rightId != std::string::npos)
    {
        rightmp = stringToTree(subtree.substr(rightId), root);
    }
    if (leftId == rightId) // both is npos
    {
        // It is leaf
        Node* leaf = new Node;
        if (leaf == nullptr)
            return nullptr;
        leaf->left = nullptr;
        leaf->right = nullptr;
        std::string stringvalue = getValue(subtree, root);
        leaf->value = atoi(stringvalue.c_str());
        node = leaf;
    }
    else
    {
        //
        // it is normal node, not leaf, process current node, left and right subtrees
        //
        node = new Node;
        if (leftId != std::string::npos)
        {
            node->left = leftmp;
        }
        else
        {
            node->left = nullptr;
        }
        if (rightId != std::string::npos)
        {
            node->right = rightmp;
        }
        else
        {
            node->right = nullptr;
        }
        node->value = atoi(getValue(subtree, root).c_str());
    }
    return node;
}

int main()
{
    std::string nodeId;
    Node one = CreateNode(1, nullptr, nullptr);
    Node seven = CreateNode(7, nullptr, nullptr);
    Node five = CreateNode(5, &one, &seven);
    Node eight = CreateNode(8, nullptr, nullptr);
    Node twenty = CreateNode(20, nullptr, nullptr);
    Node nineteen = CreateNode(19, nullptr, &twenty);
    Node thirteen = CreateNode(13, &eight, &nineteen);
    Node ten = CreateNode(10, &five, &thirteen);
    std::string stringTree = treeToString(&ten, nodeId);
    nodeIdInt = 0;
    Node* tree = stringToTree(stringTree, stringTree);
    printf("%s\n", stringTree.c_str());
    std::string nodeIdTmp;
    printf("new %s\n", treeToString(tree, nodeIdTmp).c_str());
    return 0;
}
