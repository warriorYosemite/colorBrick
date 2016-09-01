//
//  GridImage.cpp
//  newGame
//
//  Created by Machintosh on 31/08/16.
//
//

#include "GridImage.hpp"

GridImage::GridImage() : Sprite()
{
    
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

std::string GridImage::getImageName(){

    return m_imageName;
}