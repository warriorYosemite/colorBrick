//
//  SelectCharacter.cpp
//  newGame
//
//  Created by Machintosh on 01/09/16.
//
//

#include "SelectCharacter.hpp"
#include "extensions/cocos-ext.h"
#include "constants.h"
#include "MainScene.hpp"

USING_NS_CC;

using namespace std;

#define NUMBER_OF_GRIDS             8

#define TIME_DURATION               0.75

Scene* SelectCharacter::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SelectCharacter::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectCharacter::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    return true;
}

void SelectCharacter::onEnter()
{
    
}
