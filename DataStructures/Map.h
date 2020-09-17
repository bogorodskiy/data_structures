#pragma once
// Map container based on Left-leaning red-black BST
// based on work of Robert Sedgewick and Kevin Wayne
// deletion is not supported
#include <stdexcept>  
#include <string>

namespace ds
{

template <typename Key, typename Value>
class Map
{

public:
    Map() = default;

    // copy is disabled for simplicity
    Map(const Map&) = delete;
    Map& operator = (const Map&) = delete;
  
    Map(const Map&& target)
    {
        _root = target._root;
        target._root = nullptr;
    }

    Map& operator = (const Map&& target)
    {
        removeAll();
        _root = target._root;
        target._root = nullptr;
    }

    Value operator [](const Key& key) const
    {
        auto node = find(key);
        if (node != nullptr)
        {
            return node->value;
        }
        throw std::runtime_error(std::string("ds::Map : Unable to find element, key does not exist"));
    }

    Value& operator [](const Key& key)
    {
        auto node = find(key);
        if (node != nullptr)
        {
            return node->value;
        }
        throw std::runtime_error(std::string("ds::Map : Unable to set element, key does not exist"));
    }

    ~Map()
    {
        removeAll();
    }

    bool isEmpty() const 
    {
        return (_root == nullptr);
    }

    bool contains(const Key& key) const
    {
        return (find(key) != nullptr);
    }

    void add(const Key& key, const Value& value)
    {
        _root = recursiveInsertion(_root, key, value);
        _root->isBlack = true;
    }
 
    void removeAll()
    {
        deleteNodeWithChildren(_root);
        _root = nullptr;
    }

    void printSchematic() const
    {
        if (_root == nullptr)
        {
            std::cout << "empty" << std::endl;
        }

        auto result = std::string{ "" };
        result.append(std::to_string(_root->key));

        auto pointerRight = std::string{ "^---" };
        auto pointerLeft = (_root->rightChild != nullptr) ? std::string{ "|---" } : std::string{ "^---" };

        auto leftPadding = std::string{ "" };
        auto rightPadding = std::string{ "" };
        printTreeTraversal(result, leftPadding, pointerLeft, _root->leftChild, _root->rightChild != nullptr);
        printTreeTraversal(result, rightPadding, pointerRight, _root->rightChild, false);

        std::cout << result << std::endl;
    }

    void printRaw() const
    {
        std::cout << " ------------------------ " << std::endl;
        printRaw(_root);
    }

private:
    struct RedBlackNode
    {
        Key key;
        Value value;
        RedBlackNode* leftChild = nullptr;
        RedBlackNode* rightChild = nullptr;
        bool isBlack = false; // red by default
    };

    RedBlackNode* _root = nullptr;

    bool isRed(RedBlackNode* node) const
    {
        return (node != nullptr && !node->isBlack);
    }

    RedBlackNode* recursiveInsertion(RedBlackNode* node, const Key& key, const Value& value)
    {
        if (node == nullptr)
        {
            return createNode(key, value);
        }
    
        if (key < node->key)
        {
            node->leftChild = recursiveInsertion(node->leftChild, key, value);
        }
        else if (key > node->key)
        {
            node->rightChild = recursiveInsertion(node->rightChild, key, value);
        }
        else 
        {
            node->value = value;
        }

        // fix right leaning
        if (isRed(node->rightChild) && !isRed(node->leftChild))
        {
            node = rotateLeftWithRecoloring(node);
        }
        // fix double red
        if (isRed(node->leftChild) && isRed(node->leftChild->leftChild))
        {
            node = rotateRightWithRecoloring(node);
        }
        // fix both red children
        if (isRed(node->leftChild) && isRed(node->rightChild))
        {
            flipColors(node);
        }

        return node;
    }

    RedBlackNode* createNode(const Key& key, const Value& value)
    {
        auto result = new RedBlackNode;
        try 
        {
            result->key = key;
            result->value = value;
        }
        catch(...)
        {
            // copy ctor throw
            delete result;
            throw;
        }
        return result;
    }

    void recolorRotatedNodes(RedBlackNode* oldTop, RedBlackNode* newTop)
    {
        newTop->isBlack = oldTop->isBlack;
        oldTop->isBlack = false;
    }

    //
    //            A                   B
    //          /   \     ->        /   \
    //        B      Y            X      A
    //      /   \                      /   \
    //     X     C                    C     Y
    //
    RedBlackNode* rotateRight(RedBlackNode* a)
    {
        auto b = a->leftChild;
        auto c = b->rightChild;
     
        b->rightChild = a;
        a->leftChild = c;
    
        return b;
    }

    //
    //          A                            B
    //        /   \                         / \
    //       Y     B             ->       A    X
    //           /  \                   /  \
    //          C    X                 Y    C
    //
    RedBlackNode* rotateLeft(RedBlackNode* a)
    {
        auto b = a->rightChild;
        auto c = b->leftChild;

        b->leftChild = a;
        a->rightChild = c;
    
        return b;
    }

    RedBlackNode* rotateRightWithRecoloring(RedBlackNode* node)
    {
        auto newTop = rotateRight(node);
        recolorRotatedNodes(node, newTop);
        node = newTop;
        return node;
    }

    RedBlackNode* rotateLeftWithRecoloring(RedBlackNode* node)
    {
        auto newTop = rotateLeft(node);
        recolorRotatedNodes(node, newTop);
        node = newTop;
        return node;
    }

    void flipColors(RedBlackNode* node)
    {
        node->isBlack = !node->isBlack;
        node->leftChild->isBlack = !node->leftChild->isBlack;
        node->rightChild->isBlack = !node->rightChild->isBlack;
    }

    RedBlackNode* find(const Key& key) const
    {
        auto node = _root;
        while (node != nullptr)
        {
            if (key < node->key)
            {
                node = node->leftChild;
            }
            else if (key > node->key)
            {
                node = node->rightChild;
            }
            else
            {
                return node;
            }
        }

        return nullptr;
    }

    void deleteNodeWithChildren(RedBlackNode* node)
    {
        if (node == nullptr)
        {
            return;
        }

        deleteNodeWithChildren(node->leftChild);
        deleteNodeWithChildren(node->rightChild);

        delete node;
    }

    void printRaw(RedBlackNode* node) const
    {
        if (node == nullptr)
        {
            return;
        }
        auto L = node->leftChild == nullptr ? std::string{ "null" } : std::to_string(node->leftChild->key);
        auto R = node->rightChild == nullptr ? std::string{ "null" } : std::to_string(node->rightChild->key);
        auto color = isRed(node) ? " Red" : " Black";
        std::cout << node->key << color << "(Left = " << L << ", Right = " << R << ")" << std::endl;
        printRaw(node->leftChild);
        printRaw(node->rightChild);
    }

    void printTreeTraversal(std::string& result, std::string& padding, std::string& pointer, RedBlackNode* node, bool hasRightSibling) const
    {
        if (node == nullptr)
        {
            return;
        }

        result.append("\n");
        result.append(padding);
        result.append(pointer);
        result.append(std::to_string(node->key));

        auto paddingBuilder = std::string{ padding };

        if (hasRightSibling)
        {
            paddingBuilder.append("|   ");
        }
        else 
        {
            paddingBuilder.append("    ");
        }

        auto paddingForBoth = paddingBuilder;
        auto pointerRight = std::string{ "^---" };
        auto pointerLeft = (node->rightChild != nullptr) ? std::string{ "|---" } : std::string{ "^---" };

        printTreeTraversal(result, paddingForBoth, pointerLeft, node->leftChild, node->rightChild != nullptr);
        printTreeTraversal(result, paddingForBoth, pointerRight, node->rightChild, false);
    }
};
}