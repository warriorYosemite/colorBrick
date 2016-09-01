//
//  MainScene.hpp
//  newGame
//
//  Created by Machintosh on 30/08/16.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "InfiniteParallaxNode.h"

using namespace cocos2d::ui;
USING_NS_CC;


class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onEnter();

    Size visibleSize;
    Sprite* backGround;
    Sprite* backGround2;
    InfiniteParallaxNode *parallaxNode;
    Label* highScoreStr;
    
    
    void createBackgroundWithAnimation();
    void createTextItems();
    void createPlayButton();
    void updateHighScoreAfterGame();
    
    void updateBgPosition(float dt);
    
    void playButtonCallback(Ref* pSender);
    void playStageTwoCallback(Ref* pSender);
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif /* MainScene_hpp */
