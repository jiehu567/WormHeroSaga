#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "math.h"

const float PTM_RATIO = 36.0f;        //each 36 pix equals to 1 meter in real world
const int WORM_TAG = 101,
          BIRD_RADIUS = 50,
          START_TAG = 100,
          STOP_TAG = 101;

enum MOON_STATUS {LAUGH = 0, SCREAM, CRY};



class HelloWorld : public cocos2d::Layer, public b2ContactListener
{
private:
    cocos2d::Menu* startMenu;
    cocos2d::Menu* stopMenu;
    cocos2d::Menu* setMenu;
    cocos2d::Menu* playAgainMenu;
    cocos2d::Label* cool;
    
    
    bool musicOn = true;
    bool effectOn = true;

    b2World* world;
    cocos2d::Sprite* worm;
    cocos2d::Sprite* line;
    b2Body* wormBody;
    
    cocos2d::Sprite* moon;
    //cocos2d::Sprite* bird;
    int moon_status;
    
    int birdNumber;
    
    bool isContact;
    cocos2d::LayerColor* blackLayer;
    
public:
    
    ~HelloWorld();
    
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void startButtonCallBack(cocos2d::Ref* pSender);
    void stopButtonCallBack(cocos2d::Ref* pSender);
    
    //items in setting menu
    void MusicOnCallBack(Ref* pSender);
    void EffectOnCallBack(Ref* pSender);
    void playCallBack(Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    //Physics world of playing part
    void initWorld();
    
    void addWorm();
    void addLeftBirdAtWormPosition();
    void addRightBirdAtWormPosition();
    void addLeftBirdAtRandomPosition();
    void addRightBirdAtRandomPosition();
    
    //override update
    virtual void update(float dt);
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void addBirds(float dt);
    
    
    //detect contact with worm and stop game
    virtual void BeginContact(b2Contact* contact);
    void stopGame();
    void showResult();
    void playAgainCallBack(Ref* pSender);
};

#endif // __HELLOWORLD_SCENE_H__
