//
//  SelectCharacter.hpp
//  newGame
//
//  Created by Machintosh on 01/09/16.
//
//

#ifndef SelectCharacter_hpp
#define SelectCharacter_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "ui/UIWidget.h"
#include "GridImage.hpp"

using namespace cocos2d::ui;
USING_NS_CC;

class SelectCharacter : public cocos2d::Layer
{
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual void onEnter();
    
  
    Size visibleSize;
    LayerColor* m_topHeader;
    std::vector<GridImage*> m_colorVector;
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(SelectCharacter);
};


#endif /* SelectCharacter_hpp */
