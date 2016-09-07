//
//  GridImage.hpp
//  newGame
//
//  Created by Machintosh on 31/08/16.
//
//

#ifndef GridImage_hpp
#define GridImage_hpp

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

class GridImage : public Sprite
{

    private:
        GridImage();
        std::string m_imageName;
        int m_killCount;
        Label* m_killCountLabel;
    
    public:
        std::string getImageName();
        void setImageName(std::string imageName);
        ~GridImage();
        static GridImage* create(std::string fileName);
        void setKillStatus(bool isKilled);
        void setKillCount(int count);
        int getKillCount();
        void killCountDisplay();
        void setPermanentlyKilled(bool isKilled);

};


#endif /* GridImage_hpp */
