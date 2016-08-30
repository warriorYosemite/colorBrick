#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
using namespace cocos2d::ui;
USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    virtual void onEnter();
//    virtual void onExit();
    
    Size visibleSize;
    LayerColor* m_topHeader;
    int m_Score;
    int m_highScore;
    
    float m_gridWidth;
    float m_gridHeight;
    Label* m_scoreLabel;
    int m_gridCount;
    int m_previousNumber;
    Label* playerScore;
    
    bool isTouched;
    bool isGameOver;
    
    Scale9Sprite* popUpLayer;
    
    std::vector<Color3B> m_colorVector;
    std::vector<LayerColor*> m_gridVector;
    
    
    void createTopHeader();
    void setNumberOfGrids(int count);
    void createGameGrids();
    void getGridDimensions();
    void fillColorVector();
    void updateColors(float dt);
    void resetAllGridColors();
    void handleGameOver();
    void handleHighScore();
    
    void restartGameScenario(Ref* pSender);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
