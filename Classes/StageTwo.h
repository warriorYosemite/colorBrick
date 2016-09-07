#ifndef __STAGE_TWO_H__
#define __STAGE_TWO_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "GridImage.hpp"

using namespace cocos2d::ui;
USING_NS_CC;

class StageTwo : public cocos2d::Layer
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

    Label* m_playerKillName;
    Layer* popUpLayerBase;
    
    bool isLevelUp;
    int m_countThreshold;
    
    bool isTouched;
    bool isGameOver;
    bool isCharacterSelected;
    
//    EventListenerTouchOneByOne* touchListener;
    
    Scale9Sprite* popUpLayer;
    Scale9Sprite* homeButtonBg;
    Scale9Sprite* m_characterSelection;
    
    std::vector<GridImage*> m_colorVector;
    std::vector<Node*> m_gridVector;
    std::string m_selectedPlayerName;
    std::vector<std::string> m_deadCharacterVector;
    std::vector<GridImage*> m_aliveCharacterVector;
    
    void createTopHeader();
    void setNumberOfGrids(int count);
    void createGameGrids();
    void getGridDimensions();
    void fillImageVector();
    void updateColors(float dt);
    void resetAllGridColors();
    void handleGameOver();
    void handleHighScore();
    void selectCharacterToSave(int buttonCount);
    
    void appendCubicBezier(int startPoint, std::vector<Vec2>& verts, const Vec2& from, const Vec2& control1, const Vec2& control2, const Vec2& to, uint32_t segments);
    Node* createRoundedRectMaskNode(Size size, float radius, float borderWidth, int cornerSegments);
    
    
    
    
    void restartGameScenario(Ref* pSender);
    void homeButtonCallback(Ref* pSender);
    
    void okButtonCallback(Ref* pSender);
    void cancelButtonCallback(Ref* pSender);
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    virtual bool onTouchBegan(Touch *touch, Event *unused_event);
    virtual void onTouchEnded(Touch *touch, Event *unused_event);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(StageTwo);
};

#endif // __STAGE_TWO_H__
