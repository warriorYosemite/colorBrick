//
//  InfiniteParallaxNode.cpp
//


#include "InfiniteParallaxNode.h"
USING_NS_CC;

class PointObject : public Ref
{
public:
    inline void setRation(Point ratio) {_ratio = ratio;}
    inline void setOffset(Point offset) {_offset = offset;}
    inline void setChild(Node *var) {_child = var;}
    inline Point getOffset() const {return _offset;}
	inline Point getRation() const {return _ratio;}
    inline Node* getChild() const {return _child;}
private:
    Point _ratio;
    Point _offset;
    Node* _child;
};

InfiniteParallaxNode* InfiniteParallaxNode::create()
{
    // Create an instance of InfiniteParallaxNode
    InfiniteParallaxNode* node = new InfiniteParallaxNode();
    if(node) {
        // Add it to autorelease pool
        node->autorelease();
    } else {
        // Otherwise delete
        delete node;
        node = 0;
    }
    return node;
}


void InfiniteParallaxNode::updatePosition(Vec2 diff)
{
    float xDiff = diff.x;
    
    int safeOffset = -1;
    // Get visible size
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 1. For each child of an parallax node
    for(int i = 0; i < _children.size(); i++)
    {
        auto node = _children.at(i);
        // 2. We check whether it is out of the left side of the visible area
        if(xDiff < 0 && convertToWorldSpace(node->getPosition()).x + node->getContentSize().width < safeOffset)
        {
//            CCLOG("node->getPosition()).x : %f ", node->getPosition().x);
            rePositionNode(node, xDiff);
        }
        // 2. We check whether it is out of the right side of the visible area
        else if (xDiff > 0 && convertToWorldSpace(node->getPosition()).x > visibleSize.width)
        {
//            CCLOG("node->getPosition()).x : %f ", node->getPosition().x);
            rePositionNode(node, xDiff);
        }
    }
    
}

void InfiniteParallaxNode::rePositionNode(Node *node, float xDiff)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    // 3. Find PointObject that corresponds to current node
    for(int i = 0; i < _parallaxArray->num; i++)
    {
        auto po = (PointObject*)_parallaxArray->arr[i];
        // If yes increase its current offset on the value of visible width
        if(po->getChild() == node)
        {
//            CCLOG("tag :  %d  ::  %f", node->getTag(), node->getContentSize().width*2);
            Point p = po->getOffset();
            
            int diffX = ((xDiff > 0) ? -1 : 1 );
            
            int diffScreenWidthAndParallaxLayerWidth = node->getContentSize().width - visibleSize.width;
            
            po->setOffset(po->getOffset() +
                          Point(diffX*(visibleSize.width + node->getContentSize().width + diffScreenWidthAndParallaxLayerWidth),0));
        }
    }
    

}