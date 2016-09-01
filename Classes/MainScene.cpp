//
//  MainScene.cpp
//  newGame
//
//  Created by Machintosh on 30/08/16.
//
//

#include "MainScene.hpp"
#include "HelloWorldScene.h"
#include "extensions/cocos-ext.h"
#include "constants.h"
#include "StageTwo.h"

USING_NS_CC;

using namespace std;

Vec2 infiniteMoveBy = Vec2(-2,0);

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
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

void MainScene::onEnter()
{
    
    CCLOG("In on enter main Scene");
    createBackgroundWithAnimation();

    createTextItems();
    createPlayButton();
    Node::onEnter();
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::updateBgPosition), 0.1);
}


void MainScene::createTextItems(){

    Label* gameName = Label::createWithTTF("COLOR BRICKS", GAME_FONT_HEADER, 75);
    gameName->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height - gameName->getContentSize().height * 0.85));
    this->addChild(gameName);
    
    UserDefault* userDef =  UserDefault::getInstance();
    int value = userDef->getIntegerForKey(GAME_HIGH_SCORE_KEY, 0);

    std::string highScore = "HIGH SCORE " + std::to_string(value);
    highScoreStr = Label::createWithTTF(highScore, GAME_FONT_HEADER, 50);
    highScoreStr->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.25));
    this->addChild(highScoreStr);
    
    std::string textStr = "Game Objective \n Touch the white color within 1 sec";
    Label* ruleStr = Label::createWithTTF(textStr, GAME_FONT_HEADER, 30);
    ruleStr->setAlignment(TextHAlignment::CENTER);
    ruleStr->setPosition(Vec2(visibleSize.width * 0.5, 50));
    this->addChild(ruleStr);

}

void MainScene::updateHighScoreAfterGame(){


    UserDefault* userDef =  UserDefault::getInstance();
    int value = userDef->getIntegerForKey(GAME_HIGH_SCORE_KEY, 0);
    std::string highScore = "HIGH SCORE " + std::to_string(value);
    highScoreStr->setString(highScore);
}

void MainScene::createPlayButton(){

    Label* playLabel = Label::createWithTTF("PLAY WORLD 1", GAME_FONT_HEADER, 60);
    MenuItemLabel* playButton = MenuItemLabel::create(playLabel, CC_CALLBACK_1(MainScene::playButtonCallback, this));
    playButton->setAnchorPoint(Vec2(0.5,0.5));
    auto playMenu = Menu::create(playButton, NULL);
    playMenu->setPosition(Vec2(visibleSize.width* 0.5, visibleSize.height * 0.55));
    this->addChild(playMenu);
    
    
    Label* playLabelStageTwo = Label::createWithTTF("PLAY WORLD 2", GAME_FONT_HEADER, 60);
    MenuItemLabel* playStageTwo = MenuItemLabel::create(playLabelStageTwo, CC_CALLBACK_1(MainScene::playStageTwoCallback, this));
    playStageTwo->setAnchorPoint(Vec2(0.5,0.5));
    auto playMenuStageTwo = Menu::create(playStageTwo, NULL);
    playMenuStageTwo->setPosition(Vec2(visibleSize.width* 0.5, visibleSize.height * 0.45));
    this->addChild(playMenuStageTwo);
    
}


void MainScene::playStageTwoCallback(Ref* pSender){

    StageTwo* gameLayer = StageTwo::create();
    this->addChild(gameLayer, 10);

}

void MainScene::playButtonCallback(Ref* pSender){

    HelloWorld* gameLayer = HelloWorld::create();
    this->addChild(gameLayer, 10);
    
}

void MainScene::createBackgroundWithAnimation(){

    backGround = Sprite::create("background.jpg");
    backGround2 = Sprite::create("background.jpg");
   
    parallaxNode = InfiniteParallaxNode::create();
    backGround->setAnchorPoint(Point::ZERO);
    backGround2->setAnchorPoint(Point::ZERO);
    
    parallaxNode->addChild(backGround, 1, Vec2(1.0f, 0.0f),
                           Vec2(0.0f, visibleSize.height - backGround->getContentSize().height));
    parallaxNode->addChild(backGround2, 1, Vec2(1.0f, 0.0f),
                           Vec2(backGround->getContentSize().width,
                                visibleSize.height - backGround->getContentSize().height));
    
    addChild(parallaxNode);
}

void MainScene::updateBgPosition(float dt){

    parallaxNode->setPosition(parallaxNode->getPosition() + infiniteMoveBy);
    parallaxNode->updatePosition(infiniteMoveBy);
}

void MainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
