#include "HelloWorldScene.h"
#include "extensions/cocos-ext.h"
#include "constants.h"
#include "MainScene.hpp"

USING_NS_CC;

using namespace std;

#define GRID_BASE_TAG               100
#define NUMBER_OF_GRIDS             8



#define TIME_DURATION               0.75

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    m_gridVector.clear();
    m_colorVector.clear();
    
    return true;
}

void HelloWorld::onEnter()
{
 
    m_gridCount = 0;
    m_gridWidth = 0;
    m_gridHeight = 0;
    m_previousNumber = -1;
    m_Score = 0;
    m_highScore = 0;
    isTouched = true;
    isGameOver = false;
    
    
    LayerColor* m_bgLayer = LayerColor::create(Color4B(0,0,0, 255), visibleSize.width, visibleSize.height);
    m_bgLayer->setPosition(Vec2(0,0));
    this->addChild(m_bgLayer);
    
    createTopHeader();
    setNumberOfGrids(NUMBER_OF_GRIDS);
    fillColorVector();
    getGridDimensions();
    createGameGrids();
    handleHighScore();
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    Node::onEnter();
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateColors), TIME_DURATION);
}


void HelloWorld::handleHighScore(){

    UserDefault* userDef =  UserDefault::getInstance();
    int value = userDef->getIntegerForKey(GAME_HIGH_SCORE_KEY, 0);
    m_highScore = value;
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event)
{
    CCLOG("in on touch began");
    isTouched = true;
    
    Size locationInNodeBG;
    
    if (isGameOver)
    {
        CCLOG("game is over");
        return true;
    }
    
    for (int i=0; i < m_gridCount; i++)
    {
        Node* gridE = m_gridVector.at(i);
        
        Vec2 locationInNode = gridE->convertToNodeSpace(touch->getLocation());
        
        LayerColor* colorE = (LayerColor*)gridE->getChildByTag(GRID_BASE_TAG + i);
        locationInNodeBG = colorE->getContentSize();
        
        Rect rect = Rect(0, 0, locationInNodeBG.width, locationInNodeBG.height);
        
        if (rect.containsPoint(locationInNode))
        {
            Color3B currColor = colorE->getColor();
            if (currColor == Color3B::WHITE)
            {
                m_Score++;
                playerScore->setString(std::to_string(m_Score));
                break;
            }
            else
            {
//                m_Score--;
//                if (m_Score < 0)
//                {
                    CCLOG("game over !");
                    handleGameOver();
                    break;
                   
//                }
//                playerScore->setString(std::to_string(m_Score));
//                break;
            }
        }
        
    }
    return true;
}

void HelloWorld::handleGameOver(){

    this->unschedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateColors));
    
    if (m_Score > m_highScore)
    {
        m_highScore = m_Score;
        UserDefault::getInstance()->setIntegerForKey(GAME_HIGH_SCORE_KEY, m_highScore);
        UserDefault::getInstance()->flush();
    }
    
    popUpLayer = Scale9Sprite::create("board_9patch.png");
    popUpLayer->setContentSize(Size(visibleSize.width * 0.75, visibleSize.height * 0.35));
    popUpLayer->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.5));
    this->addChild(popUpLayer);
    
    Label* gameOverText = Label::createWithTTF("GAME OVER !", GAME_FONT_HEADER, 50);
    gameOverText->setColor(Color3B::WHITE);
    gameOverText->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, popUpLayer->getContentSize().height - gameOverText->getContentSize().height *0.9));
    popUpLayer->addChild(gameOverText);

    std::string yourScoreStr = "Your Score : " + std::to_string(m_Score);
    Label* yourScore = Label::createWithTTF(yourScoreStr, GAME_FONT_HEADER, 50);
    yourScore->setColor(Color3B::WHITE);
    yourScore->setAnchorPoint(Vec2(0.5,0.5));
    yourScore->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, popUpLayer->getContentSize().height * 0.4));
    popUpLayer->addChild(yourScore);

    
    std::string highScore = "High Score : " + std::to_string(m_highScore);
    Label* highScoreStr = Label::createWithTTF(highScore, GAME_FONT_HEADER, 50);
    highScoreStr->setColor(Color3B::WHITE);
    highScoreStr->setAnchorPoint(Vec2(0.5, 0.5));
    highScoreStr->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, popUpLayer->getContentSize().height * 0.6));
    popUpLayer->addChild(highScoreStr);
    
    
    Label* gameStart = Label::createWithTTF("PLAY AGAIN", GAME_FONT_HEADER, 55);
    MenuItemLabel* restartImage = MenuItemLabel::create(gameStart, CC_CALLBACK_1(HelloWorld::restartGameScenario, this));
    restartImage->setAnchorPoint(Vec2(0.5,0.5));
    auto restartMenu = Menu::create(restartImage, NULL);
    restartMenu->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, restartImage->getContentSize().height * 0.6));
    popUpLayer->addChild(restartMenu);
    
    isGameOver = true;
    
    
    homeButtonBg = Scale9Sprite::create("board_9patch.png");
    homeButtonBg->setContentSize(Size(visibleSize.width * 0.4, visibleSize.height * 0.1));
    homeButtonBg->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.15));
    this->addChild(homeButtonBg);
    
    Label* homeLabel = Label::createWithTTF("HOME", GAME_FONT_HEADER, 40);
    MenuItemLabel* homeStr = MenuItemLabel::create(homeLabel, CC_CALLBACK_1(HelloWorld::homeButtonCallback, this));
    homeStr->setAnchorPoint(Vec2(0.5,0.5));
    auto homeMenu = Menu::create(homeStr, NULL);
    homeMenu->setPosition(Vec2(homeButtonBg->getContentSize().width * 0.5, homeButtonBg->getContentSize().height * 0.5));
    homeButtonBg->addChild(homeMenu);

}

void HelloWorld::homeButtonCallback(Ref* pSender){

    CCLOG("home button pressed");
    unscheduleAllCallbacks();
    
    MainScene* mainScene = dynamic_cast<MainScene*>(Director::getInstance()->getRunningScene()->getChildren().at(1));
    if (mainScene != NULL){
    
        mainScene->updateHighScoreAfterGame();
    }
    
    this->removeFromParentAndCleanup(true);
    
}

void HelloWorld::restartGameScenario(Ref* pSender){
    CCLOG("restart game clicked");
    
    m_previousNumber = -1;
    m_Score = 0;
    isTouched = true;
    isGameOver = false;
    playerScore->setString(std::to_string(m_Score));
    
    resetAllGridColors();
    
    popUpLayer->removeFromParentAndCleanup(true);
    homeButtonBg->removeFromParentAndCleanup(true);
    this->schedule(CC_SCHEDULE_SELECTOR(HelloWorld::updateColors), TIME_DURATION);
    
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event)
{
    CCLOG("in on touch ended");
}

void HelloWorld::setNumberOfGrids(int count)
{
    m_gridCount = count;
}

void HelloWorld::fillColorVector()
{
    Color3B elem1 = Color3B(22,141,129);
    m_colorVector.push_back(elem1);
    
    Color3B elem2 = Color3B(0,51,102);
    m_colorVector.push_back(elem2);
    
    Color3B elem3 = Color3B(241,196,15);
    m_colorVector.push_back(elem3);
    
    Color3B elem4 = Color3B(148,49,38);
    m_colorVector.push_back(elem4);
    
    Color3B elem5 = Color3B(141,84,172);
    m_colorVector.push_back(elem5);
    
    Color3B elem6 = Color3B(202,111,30);
    m_colorVector.push_back(elem6);
    
    Color3B elem7 = Color3B(102,205,170);
    m_colorVector.push_back(elem7);
    
    Color3B elem8 = Color3B(109,155,43);
    m_colorVector.push_back(elem8);
    
}

void HelloWorld::createTopHeader()
{
    m_topHeader = LayerColor::create(Color4B(Color3B(166,166,208)), visibleSize.width, visibleSize.height * 0.1);
    m_topHeader->setPosition(Vec2(0, visibleSize.height - visibleSize.height * 0.1));
    this->addChild(m_topHeader,1);
    
    Label* yourScroreText = Label::createWithTTF("SCORE : ", GAME_FONT_HEADER, 60);
    yourScroreText->setPosition(Vec2(m_topHeader->getContentSize().width * 0.4, m_topHeader->getContentSize().height * 0.475));
    yourScroreText->setColor(Color3B::BLACK);
    m_topHeader->addChild(yourScroreText);
    
    playerScore = Label::createWithTTF(std::to_string(m_Score), GAME_FONT_HEADER, 70);
    playerScore->setPosition(Vec2(m_topHeader->getContentSize().width * 0.75, m_topHeader->getContentSize().height * 0.4));
    playerScore->setColor(Color3B::BLACK);
    m_topHeader->addChild(playerScore);
}

void HelloWorld::updateColors(float dt)
{
    CCLOG("updating layer colors to White");
    if (!isTouched){
        
//        m_Score--;
//        if (m_Score < 0)
//        {
            CCLOG("game over !");
            handleGameOver();
            return;
//        }
//        playerScore->setString(std::to_string(m_Score));
        
    }
    
    resetAllGridColors();
    
    int randNo;
    
    do {
        randNo = random(0, m_gridCount - 1);                // to avoid same color to be WHITE continously
    } while (randNo == m_previousNumber);
    
    Node* currE = m_gridVector.at(randNo);
    
    LayerColor* layerE = (LayerColor*)currE->getChildByTag(GRID_BASE_TAG + randNo);
    layerE->setColor(Color3B::WHITE);
    
    m_previousNumber = randNo;
    isTouched = false;
}

void HelloWorld::resetAllGridColors(){
    
    CCLOG("all grid colors reset");
    for (int i=0; i < m_gridCount; i++)
    {
        Node* tempE = m_gridVector.at(i);
        LayerColor* layerE = (LayerColor*)tempE->getChildByTag(GRID_BASE_TAG + i);
        Color3B originalColor = m_colorVector.at(i);
        layerE->setColor(originalColor);
    }
}


void HelloWorld::getGridDimensions()
{
    CCLOG("into creating grids");
    
    //we will divide the whole screen depending upon the number of grids we want
    
    float availableHeight = visibleSize.height - visibleSize.height * 0.1;
    m_gridWidth = visibleSize.width / 2;
    
    if (m_gridCount == 4)
    {
        m_gridHeight = availableHeight /2;
        CCLOG("4 grids created");
    }
    else if (m_gridCount == 6){
        m_gridHeight = availableHeight /3;
        CCLOG("6 grids created");
        
    }
    else if (m_gridCount == 8){
        m_gridHeight = availableHeight /4;
        CCLOG("8 grids created");
    }
}

void HelloWorld::createGameGrids(){

    CCLOG("creating game grids and pushing them in a vector");
    for (int i=0; i < m_gridCount; i++)
    {
        LayerColor* layerE = LayerColor::create(Color4B(Color3B::GRAY), m_gridWidth, m_gridHeight);
        Color3B colorE = m_colorVector.at(i);
        layerE->setColor(colorE);
        layerE->setTag(GRID_BASE_TAG + i);

        Size ws = layerE->getContentSize();
        Size maskSize = ws* 0.925;
        
        float radius = 40;

        // create masked image and position to center it on screen
        Node* clipNode = createRoundedRectMaskNode(maskSize, radius, 1.0f, 10);
        clipNode->setAnchorPoint(Vec2(0,0));
        clipNode->addChild(layerE);
        
        if ( i % 2 == 0)
        {
            clipNode->setPosition(Vec2(10, (i * 0.5) * m_gridHeight + 10));
        }else{
            
            int factor = (int )i /2;
            clipNode->setPosition(Vec2(m_gridWidth, factor * m_gridHeight + 10));
        }
        
        this->addChild(clipNode);
        m_gridVector.push_back(clipNode);
    }

}


void HelloWorld::appendCubicBezier(int startPoint, std::vector<Vec2>& verts, const Vec2& from, const Vec2& control1, const Vec2& control2, const Vec2& to, uint32_t segments)
{
    float t = 0;
    for(int i = 0; i < segments; i++)
    {
        float x = powf(1 - t, 3) * from.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * to.x;
        float y = powf(1 - t, 3) * from.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * to.y;
        verts[startPoint + i] = Vec2(x,y);
        t += 1.0f / segments;
    }
}

Node* HelloWorld::createRoundedRectMaskNode(Size size, float radius, float borderWidth, int cornerSegments)
{
    const float kappa = 0.552228474;
    float oneMinusKappa = (1.0f-kappa);
    
    // define corner control points
    std::vector<Vec2> verts(16);
    
    verts[0] = Vec2(0, radius);
    verts[1] = Vec2(0, radius * oneMinusKappa);
    verts[2] = Vec2(radius * oneMinusKappa, 0);
    verts[3] = Vec2(radius, 0);
    
    verts[4] = Vec2(size.width - radius, 0);
    verts[5] = Vec2(size.width - radius * oneMinusKappa, 0);
    verts[6] = Vec2(size.width, radius * oneMinusKappa);
    verts[7] = Vec2(size.width, radius);
    
    verts[8] = Vec2(size.width, size.height - radius);
    verts[9] = Vec2(size.width, size.height - radius * oneMinusKappa);
    verts[10] = Vec2(size.width - radius * oneMinusKappa, size.height);
    verts[11] = Vec2(size.width - radius, size.height);
    
    verts[12] = Vec2(radius, size.height);
    verts[13] = Vec2(radius * oneMinusKappa, size.height);
    verts[14] = Vec2(0, size.height - radius * oneMinusKappa);
    verts[15] = Vec2(0, size.height - radius);
    
    // result
    std::vector<Vec2> polyVerts(4 * cornerSegments + 1);
    
    // add corner arc segments
    appendCubicBezier(0 * cornerSegments, polyVerts, verts[0], verts[1], verts[2], verts[3], cornerSegments);
    appendCubicBezier(1 * cornerSegments, polyVerts, verts[4], verts[5], verts[6], verts[7], cornerSegments);
    appendCubicBezier(2 * cornerSegments, polyVerts, verts[8], verts[9], verts[10], verts[11], cornerSegments);
    appendCubicBezier(3 * cornerSegments, polyVerts, verts[12], verts[13], verts[14], verts[15], cornerSegments);
    // close path
    polyVerts[4 * cornerSegments] = verts[0];
    
    // draw final poly into mask
    auto shapeMask = DrawNode::create();
    shapeMask->drawPolygon(&polyVerts[0], polyVerts.size(), Color4F::WHITE, 0.0f, Color4F::WHITE);
    
    // create clip node with draw node as stencil (mask)
    return ClippingNode::create(shapeMask);
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
