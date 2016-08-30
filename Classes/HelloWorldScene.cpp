#include "HelloWorldScene.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;

using namespace std;

#define GRID_BASE_TAG               100
#define NUMBER_OF_GRIDS             8


#define GAME_FONT                   "fonts/Deanna.ttf"
#define TIME_DURATION               0.75
#define GAME_HIGH_SCORE_KEY                         "highScoreKey"

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
    
    
    LayerColor* m_bgLayer = LayerColor::create(Color4B(Color3B(156,156,156)), visibleSize.width, visibleSize.height);
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
    
    Vec2 currLocation = touch->getLocation();
    
    if (isGameOver)
    {
        CCLOG("game is over");
        return true;
    }
    
    for (int i=0; i < m_gridCount; i++)
    {
        LayerColor* gridE = m_gridVector.at(i);
        Rect layerBox = gridE->getBoundingBox();
        if (layerBox.containsPoint(currLocation))
        {
            Color3B currColor = gridE->getColor();
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
    
    Label* gameOverText = Label::createWithTTF("GAME OVER", GAME_FONT, 100);
    gameOverText->setColor(Color3B::WHITE);
    gameOverText->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, popUpLayer->getContentSize().height - gameOverText->getContentSize().height *0.5));
    popUpLayer->addChild(gameOverText);

    std::string yourScoreStr = "Your Score :" + std::to_string(m_Score);
    Label* yourScore = Label::createWithTTF(yourScoreStr, GAME_FONT, 75);
    yourScore->setColor(Color3B::WHITE);
    yourScore->setAnchorPoint(Vec2(0.5,0.5));
    yourScore->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, popUpLayer->getContentSize().height * 0.4));
    popUpLayer->addChild(yourScore);

    
    std::string highScore = "High Score : " + std::to_string(m_highScore);
    Label* highScoreStr = Label::createWithTTF(highScore, GAME_FONT, 75);
    highScoreStr->setColor(Color3B::WHITE);
    highScoreStr->setAnchorPoint(Vec2(0.5, 0.5));
    highScoreStr->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, popUpLayer->getContentSize().height * 0.6));
    popUpLayer->addChild(highScoreStr);
    
    
    Label* gameStart = Label::createWithTTF("PLAY AGAIN", GAME_FONT, 80);
    MenuItemLabel* restartImage = MenuItemLabel::create(gameStart, CC_CALLBACK_1(HelloWorld::restartGameScenario, this));
    restartImage->setAnchorPoint(Vec2(0.5,0.5));
    auto restartMenu = Menu::create(restartImage, NULL);
    restartMenu->setPosition(Vec2(popUpLayer->getContentSize().width * 0.5, restartImage->getContentSize().height * 0.6));
    popUpLayer->addChild(restartMenu);
    
    isGameOver = true;
}


void HelloWorld::restartGameScenario(Ref* pSender){
    CCLOG("restart game clicked");
    
    m_previousNumber = -1;
    m_Score = 0;
    isTouched = true;
    isGameOver = false;
    playerScore->setString(std::to_string(m_Score));
    
    popUpLayer->removeFromParentAndCleanup(true);
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
    Color3B elem1 = Color3B::GREEN;
    m_colorVector.push_back(elem1);
    
    Color3B elem2 = Color3B::BLUE;
    m_colorVector.push_back(elem2);
    
    Color3B elem3 = Color3B::YELLOW;
    m_colorVector.push_back(elem3);
    
    Color3B elem4 = Color3B::RED;
    m_colorVector.push_back(elem4);
    
    Color3B elem5 = Color3B::MAGENTA;
    m_colorVector.push_back(elem5);
    
    Color3B elem6 = Color3B::ORANGE;
    m_colorVector.push_back(elem6);
    
    Color3B elem7 = Color3B::BLACK;
    m_colorVector.push_back(elem7);
    
    Color3B elem8 = Color3B(102,51,0);
    m_colorVector.push_back(elem8);
    
}

void HelloWorld::createTopHeader()
{
    m_topHeader = LayerColor::create(Color4B(Color3B(166,166,208)), visibleSize.width, visibleSize.height * 0.1);
    m_topHeader->setPosition(Vec2(0, visibleSize.height - visibleSize.height * 0.1));
    this->addChild(m_topHeader,1);
    
    Label* yourScroreText = Label::createWithTTF("SCORE : ", GAME_FONT, 80);
    yourScroreText->setPosition(Vec2(m_topHeader->getContentSize().width * 0.4, m_topHeader->getContentSize().height * 0.5));
    yourScroreText->setColor(Color3B::BLACK);
    m_topHeader->addChild(yourScroreText);
    
    playerScore = Label::createWithTTF(std::to_string(m_Score), GAME_FONT, 100);
    playerScore->setPosition(Vec2(m_topHeader->getContentSize().width * 0.75, m_topHeader->getContentSize().height * 0.5));
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
    
    LayerColor* currE = m_gridVector.at(randNo);
    currE->setColor(Color3B::WHITE);
    m_previousNumber = randNo;
    isTouched = false;
}

void HelloWorld::resetAllGridColors(){
    
    CCLOG("all grid colors reset");
    for (int i=0; i < m_gridCount; i++)
    {
        LayerColor* tempE = m_gridVector.at(i);
        Color3B originalColor = m_colorVector.at(i);
        tempE->setColor(originalColor);
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
        if ( i % 2 == 0)
        {
            layerE->setPosition(Vec2(0, (i * 0.5) * m_gridHeight));
        }else{
        
            int factor = (int )i /2;
            layerE->setPosition(Vec2(m_gridWidth, factor * m_gridHeight));
        }
        this->addChild(layerE);
        m_gridVector.push_back(layerE);
    }

}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
