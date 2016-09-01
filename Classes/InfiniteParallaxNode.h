//
//  InfiniteParallaxNode.h
//

#ifndef __PhysicsDemo__InfiniteParallaxNode__
#define __PhysicsDemo__InfiniteParallaxNode__

#include <stdio.h>
#include "cocos2d.h"

class InfiniteParallaxNode : public cocos2d::ParallaxNode
{
public:
    static InfiniteParallaxNode* create();
    void updatePosition(cocos2d::Vec2 diff);
    void rePositionNode(Node *node, float diff);
};

#endif /* defined(__PhysicsDemo__InfiniteParallaxNode__) */
