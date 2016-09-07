//
//  GridImage.cpp
//  newGame
//
//  Created by Machintosh on 31/08/16.
//
//

#include "GridImage.hpp"
#include "constants.h"

#define BLOOD_STAIN_TAG                 123
#define DEAD_STAMP_TAG                  125

GridImage::GridImage() : Sprite()
{
    m_killCount = 0;
}

GridImage::~GridImage() {
    // TODO
}

GridImage* GridImage::create(std::string fileName)
{
    GridImage* temp = new GridImage();
    
    Sprite* pressed = Sprite::create(fileName);

    if (temp->initWithTexture(pressed->getTexture()))
    {
//        temp->autorelease();
        return temp;
    }
    CC_SAFE_DELETE(temp);
    CC_SAFE_DELETE(pressed);
    
    return NULL;
}

void GridImage::setImageName(std::string imageName){

    m_imageName = imageName;
}

void GridImage::setKillStatus(bool isKilled){

    if (isKilled){
    
        Sprite* bloodStain = Sprite::create("gunshot.png");
        bloodStain->setPosition(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5);
        bloodStain->setTag(BLOOD_STAIN_TAG);
        this->addChild(bloodStain);
        
    }else{
    
        Sprite* bloodImage =  (Sprite*)this->getChildByTag(BLOOD_STAIN_TAG);
        if (bloodImage != NULL)
        {
            bloodImage->removeFromParent();
        }
        
    }
}

void GridImage::setPermanentlyKilled(bool isKilled){

    if (isKilled){
    
        Sprite* deadImage = Sprite::create("dead.png");
        deadImage->setPosition(this->getContentSize().width * 0.5, this->getContentSize().height * 0.5);
        deadImage->setTag(DEAD_STAMP_TAG);
        this->addChild(deadImage);
        
    }
    else{
        
        Sprite* deadStamp =  (Sprite*)this->getChildByTag(DEAD_STAMP_TAG);
        if (deadStamp != NULL)
        {
            deadStamp->removeFromParent();
        }
    }
}

void GridImage::killCountDisplay(){

    m_killCountLabel = Label::createWithTTF(std::to_string(m_killCount), GAME_FONT_HEADER, 100);
    m_killCountLabel->setAnchorPoint(Vec2(1,1));
    m_killCountLabel->setPosition(Vec2(this->getContentSize().width * 0.2, this->getContentSize().height - m_killCountLabel->getContentSize().height * 0.5));
    this->addChild(m_killCountLabel);
}

void GridImage::setKillCount(int count){

    m_killCount = count;
    m_killCountLabel->setString(std::to_string(m_killCount));
}

int GridImage::getKillCount(){
    
    return m_killCount;
}

std::string GridImage::getImageName(){

    return m_imageName;
}