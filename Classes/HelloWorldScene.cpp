#include "HelloWorldScene.h"

USING_NS_CC;

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
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //add image texture to cache
    SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("WormHeroSaga.plist");
    
    //create background
    auto background = Sprite::createWithSpriteFrameName("Background.png");
    background->setPosition(visibleSize/2);
    this->addChild(background);
    
    moon = Sprite::createWithSpriteFrameName("MoonLaugh.png");
    moon->setPosition(Director::getInstance()->convertToGL(Vec2(490,234)));
    this->addChild(moon);
    
    
    //Start button
    Sprite* startButtonItem_normal = Sprite::createWithSpriteFrameName("StartButton_normal.png");
    Sprite* startButtonItem_onClick = Sprite::createWithSpriteFrameName("StartButton_onClick.png");
    
    MenuItemSprite* startButton = MenuItemSprite::create(startButtonItem_normal,
                                                         startButtonItem_onClick,
                                                         CC_CALLBACK_1(HelloWorld::startButtonCallBack, this));
    
    startButton->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width/2,730)));
    
    startMenu = Menu::create(startButton, NULL);
    
    startMenu->setPosition(Vec2::ZERO);
    startMenu->setTag(START_TAG);
    this->addChild(startMenu);
    
    
    
    //add stop button with setting menus
    Sprite* stopButton = Sprite::createWithSpriteFrameName("StopButton.png");
    MenuItemSprite* stopButtonItem = MenuItemSprite::create(stopButton, stopButton,
                                                            CC_CALLBACK_1(HelloWorld::stopButtonCallBack, this));
    stopButtonItem->setPosition(Director::getInstance()->convertToGL(Vec2(71,38)));
    stopMenu = Menu::create(stopButtonItem, NULL);
    stopMenu->setPosition(Vec2::ZERO);
    stopMenu->setTag(STOP_TAG);
    stopMenu->setVisible(false);
    
    this->addChild(stopMenu);
    
    
    //below list set menu
    //music setting in setting menu
    Sprite* musicOn = Sprite::createWithSpriteFrameName("MusicOn.png");
    Sprite* musicOff = Sprite::createWithSpriteFrameName("MusicOff.png");
    auto musicOnSprite  = MenuItemSprite::create(musicOn,musicOn);
    auto musicOffSprite  = MenuItemSprite::create(musicOff,musicOff);
    auto musicToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HelloWorld::MusicOnCallBack, this), musicOnSprite, musicOffSprite, NULL);
    
    musicToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width/2-10, 550)));
    
    
    
    
    //effect setting in setting menu
    Sprite* effectOn = Sprite::createWithSpriteFrameName("EffectOn.png");
    Sprite* effectOff = Sprite::createWithSpriteFrameName("EffectOff.png");
    auto effectOnSprite  = MenuItemSprite::create(effectOn,effectOn);
    auto effectOffSprite  = MenuItemSprite::create(effectOff,effectOff);
    auto effectToggleMenuItem = MenuItemToggle::createWithCallback(CC_CALLBACK_1(HelloWorld::MusicOnCallBack, this), effectOnSprite, effectOffSprite, NULL);
    effectToggleMenuItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width/2, 730)));
    
    
    //play setting in setting menu
    Sprite* PlayButton = Sprite::createWithSpriteFrameName("PlayButton.png");
    MenuItemSprite* PlayButtonItem = MenuItemSprite::create(PlayButton, PlayButton,
                                                            CC_CALLBACK_1(HelloWorld::playCallBack, this));
    PlayButtonItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width/2, 910)));
    
    
    //add all items into setMenu
    setMenu = Menu::create(musicToggleMenuItem, effectToggleMenuItem, PlayButtonItem, NULL);
    setMenu->setPosition(Vec2::ZERO);
    this->addChild(setMenu);
    
    setMenu->setVisible(false);
    
    
    
    return true;
}


void HelloWorld::startButtonCallBack(Ref* pSender){
    startMenu->setVisible(false);
    setMenu->setVisible(false);

    
    birdNumber = 0;
    
    stopMenu->setVisible(true);

    this->initWorld();
    this->scheduleUpdate();
    this->schedule(schedule_selector(HelloWorld::addBirds), 3.0f, kRepeatForever, 0);
    
    
    
    
}

void HelloWorld::stopButtonCallBack(Ref* pSender){
    log("stop play");
    
    
    stopMenu->setVisible(false);
    setMenu->setVisible(true);
    
    Director::getInstance()->pause();
    
    
    
    
}


void HelloWorld::MusicOnCallBack(Ref* pSender){
    
    if (musicOn) {
        log("Music Off!");
        musicOn =  false;
    }
    else {
        log("Music On!");
        musicOn = true;
    }
}

void HelloWorld::EffectOnCallBack(Ref* pSender){
    
    if (effectOn) {
        log("Effect Off!");
        musicOn =  false;
    }
    else {
        log("Effect On!");
        musicOn = true;
    }
        
}

void HelloWorld::playCallBack(Ref* pSender){
    
    log("Play!");
    Director::getInstance()->resume();
    
    setMenu->setVisible(false);
    stopMenu->setVisible(true);
}



//initialize new physics world
void HelloWorld::initWorld(){
    //gravity parameter
    
    Size s = Director::getInstance()->getVisibleSize();
    b2Vec2 gravity;
    gravity.Set(0.0f, -15.0f);
    
    world = new b2World(gravity);
    isContact = false;
    
    //allow body sleep
    world->SetAllowSleeping(true);
    
    //Enable continuous Physics test
    world->SetContinuousPhysics(true);
    
    //set ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0,0);
    
    b2Body* groundBody = world->CreateBody(&groundBodyDef);
    b2EdgeShape groundBox;
    
    //set bottom
    groundBox.Set(b2Vec2(0, 0), b2Vec2(s.width/PTM_RATIO, 0));
    
    
    //use to fix shape on body
    groundBody->CreateFixture(&groundBox, 0);
    
    
    
    //set top
    groundBox.Set(b2Vec2(0, s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO, s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox, 0);
    
    
    //set left
    groundBox.Set(b2Vec2(0, s.height/PTM_RATIO), b2Vec2(0, 0));
    groundBody->CreateFixture(&groundBox, 0);
    
    //set right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO, s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO, 0));
    groundBody->CreateFixture(&groundBox, 0);
    
    
    

    
    //add a worm
    this->addWorm();
    
    setTouchEnabled(true);
    setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    
    world->SetContactListener(this);
    
}

void HelloWorld::addWorm(){
    Point initialPosition = Vec2(320,300);
    
    
    
    worm = Sprite::createWithSpriteFrameName("Worm.png");
    worm->setTag(WORM_TAG);
    worm->setPosition(initialPosition);
    this->addChild(worm);
    
    
    
    b2BodyDef wormBodyDef;
    wormBodyDef.type = b2_dynamicBody;
    wormBodyDef.position.Set(initialPosition.x/ PTM_RATIO, initialPosition.y/PTM_RATIO);
    
    wormBody = world->CreateBody(&wormBodyDef);
    wormBody->SetUserData(worm);
    

    
    //build shape as ball
    b2PolygonShape wormShape;
    wormShape.SetAsBox(31/2/PTM_RATIO, 98/2/PTM_RATIO);
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &wormShape;
    fixtureDef.density = 0.5f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;
    
    wormBody->CreateFixture(&fixtureDef);
    
#if line == NULL
    //add line
    
    line = Sprite::createWithSpriteFrameName("Line.png");
    line->setPosition(Vec2(320, worm->getPositionY()+590));
    line->setTag(102);
    this->addChild(line);
#endif
    
}


void HelloWorld::update(float dt){
    float timeStep = 0.03f;
    int32 velocityIterations = 8;
    int32 positionIterations = 1;

    world->Step(timeStep, velocityIterations, positionIterations);
    
    for(b2Body*b = world->GetBodyList(); b; b= b->GetNext()) {
        
        //let line move with worm
        line->setPosition(Vec2(320, worm->getPositionY()+590));
        
        
        //destroy birds passed by
        if (b->GetUserData()!= nullptr) {
            
            
            Sprite* sprite = (Sprite*) b->GetUserData();
            sprite->setPosition(Vec2(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
            
            sprite->setRotation(-1* CC_RADIANS_TO_DEGREES(b->GetAngle()));
            
        }
        
        
        Sprite* s;
        
        if (b->GetPosition().x + 200 < 0 || b->GetPosition().x > 700) {
            s = (Sprite*)b->GetUserData();
            
            if (s!=NULL) {
                s->removeFromParent();
            }
            world->DestroyBody(b);
            
            log("%d", birdNumber);
        }
        
        
        
        
        
    }
    
    
    //reset moon status, when worm moves up, moon screams, when move down, moon laugh
    b2Vec2 vel = wormBody->GetLinearVelocity();
    
    if (vel.y<=0 && isContact == false) {
        moon->setSpriteFrame("MoonLaugh.png");
        moon_status = LAUGH;
    }
    else if (vel.y > 0 && isContact == false) {
        moon->setSpriteFrame("MoonScream.png");
        moon_status = SCREAM;
    }
    
    
    
}


bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
   
    wormBody->SetLinearVelocity(b2Vec2(0, 15.0f));
    moon->setSpriteFrame("MoonScream.png");
    
    
    
    
    return true;
}





void HelloWorld::addLeftBirdAtWormPosition(){
    
    Sprite* leftBird = Sprite::createWithSpriteFrameName("Bird_left.png");
    leftBird->setPosition(Vec2(0, worm->getPositionY()));
    this->addChild(leftBird);

    b2BodyDef leftBirdBodyDef;
    leftBirdBodyDef.type = b2_kinematicBody;
    leftBirdBodyDef.position.Set(0/ PTM_RATIO, worm->getPositionY()/PTM_RATIO);
    
    b2Body* leftBirdBody = world->CreateBody(&leftBirdBodyDef);
    leftBirdBody->SetUserData(leftBird);
    
    b2CircleShape *birdShape = new b2CircleShape();
    birdShape->m_radius = BIRD_RADIUS/PTM_RATIO;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = birdShape;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;
    
    leftBirdBody->CreateFixture(&fixtureDef);
    leftBirdBody->SetLinearVelocity(b2Vec2(10.0f, 0.0f));
    
    birdNumber++;
    
    
    
}

void HelloWorld::addRightBirdAtWormPosition(){
    
    Sprite* rightBird = Sprite::createWithSpriteFrameName("Bird_right.png");
    rightBird->setPosition(Vec2(640, worm->getPositionY()));
    this->addChild(rightBird);
    
    
    b2BodyDef rightBirdBodyDef;
    rightBirdBodyDef.type = b2_kinematicBody;
    rightBirdBodyDef.position.Set(640/ PTM_RATIO, worm->getPositionY()/PTM_RATIO);
    
    b2Body* rightBirdBody = world->CreateBody(&rightBirdBodyDef);
    rightBirdBody->SetUserData(rightBird);
    
    //build shape as ball
    b2CircleShape *birdShape = new b2CircleShape();
    birdShape->m_radius = BIRD_RADIUS/PTM_RATIO;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = birdShape;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;
    
    rightBirdBody->CreateFixture(&fixtureDef);
    rightBirdBody->SetLinearVelocity(b2Vec2(-12.0f, 0.0f));
    
    birdNumber++;
    
}

void HelloWorld::addLeftBirdAtRandomPosition(){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point p = visibleSize/(rand()%5+1);
    
    
    Sprite* leftBird = Sprite::createWithSpriteFrameName("Bird_left.png");
    leftBird->setPosition(Vec2(0, p.y));
    this->addChild(leftBird);
    
    
    
    b2BodyDef leftBirdBodyDef;
    leftBirdBodyDef.type = b2_kinematicBody;
    leftBirdBodyDef.position.Set(0/ PTM_RATIO, p.y/PTM_RATIO);
    
    b2Body* leftBirdBody = world->CreateBody(&leftBirdBodyDef);
    leftBirdBody->SetUserData(leftBird);
    
    
    
    //build shape as ball
    b2CircleShape *birdShape = new b2CircleShape();
    birdShape->m_radius = BIRD_RADIUS/PTM_RATIO/2;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = birdShape;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;
    
    leftBirdBody->CreateFixture(&fixtureDef);
    leftBirdBody->SetLinearVelocity(b2Vec2(13.0f, 0.0f));
    
    birdNumber++;
    
}


void HelloWorld::addRightBirdAtRandomPosition(){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point p = visibleSize/(rand()%5+1);
    
    Sprite* rightBird = Sprite::createWithSpriteFrameName("Bird_right.png");
    rightBird->setPosition(Vec2(visibleSize.width, p.y));
    this->addChild(rightBird);
    
    
    b2BodyDef rightBirdBodyDef;
    rightBirdBodyDef.type = b2_kinematicBody;
    rightBirdBodyDef.position.Set(visibleSize.width/ PTM_RATIO, p.y/PTM_RATIO);
    
    b2Body* rightBirdBody = world->CreateBody(&rightBirdBodyDef);
    rightBirdBody->SetUserData(rightBird);
    
    
    
    //build shape as ball
    b2CircleShape *birdShape = new b2CircleShape();
    birdShape->m_radius = BIRD_RADIUS/PTM_RATIO;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = birdShape;
    fixtureDef.density = 0.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.restitution = 0.8f;
    
    rightBirdBody->CreateFixture(&fixtureDef);
    rightBirdBody->SetLinearVelocity(b2Vec2(-14.0f, 0.0f));
    
    birdNumber++;
    
}


void HelloWorld::addBirds(float dt){
    
    
    
    if (birdNumber<=3) {
        addLeftBirdAtWormPosition();
    }
    
    else if (birdNumber<=6) {
        addRightBirdAtWormPosition();
    }
    
    else if (birdNumber<=10){
        addRightBirdAtWormPosition();
        addLeftBirdAtWormPosition();
    }
    
    else if (birdNumber<=20){
        addLeftBirdAtWormPosition();
        addRightBirdAtRandomPosition();
    }
    
    else if (birdNumber<=30){
        
        addLeftBirdAtWormPosition();
        addRightBirdAtWormPosition();
        addRightBirdAtRandomPosition();
    }
    
    else {
        addLeftBirdAtWormPosition();
        addRightBirdAtWormPosition();
        addLeftBirdAtRandomPosition();
        addRightBirdAtRandomPosition();
    }
    
    
}


void HelloWorld::BeginContact(b2Contact* contact){
    
    
    
    if (contact->GetFixtureA()->GetBody()->GetUserData() == worm ||
        contact->GetFixtureB()->GetBody()->GetUserData() == worm ) {
        stopGame();
        isContact = true;
        
        //add message menu
        
        
        
        
    }
}

void HelloWorld::stopGame(){
    unscheduleUpdate();
    unschedule(schedule_selector(HelloWorld::addBirds));
    
    stopMenu->setVisible(false);

    
    this->showResult();
    
}

void HelloWorld::showResult(){
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //add black cover
    blackLayer = LayerColor::create(Color4B(0, 0, 0, 120));
    blackLayer->setContentSize(visibleSize);
    blackLayer->setPosition(0,0);
    addChild(blackLayer);
    
    
    cool = Label::createWithSystemFont("COOL!", "Hobo std", 90);
    cool->setPosition(visibleSize.width/2, visibleSize.height*2/3);
    this->addChild(cool);
    moon->setSpriteFrame("MoonCry.png");
    
    //Play again button
    Sprite* PlayAgainButton = Sprite::createWithSpriteFrameName("ReplayButton.png");
    MenuItemSprite* PlayButtonItem = MenuItemSprite::create(PlayAgainButton, PlayAgainButton,
                                                            CC_CALLBACK_1(HelloWorld::playAgainCallBack, this));
    PlayButtonItem->setPosition(Director::getInstance()->convertToGL(Vec2(visibleSize.width/2-100, visibleSize.height*3/4)));
    
    
    //add all items into setMenu
    playAgainMenu = Menu::create(PlayButtonItem, NULL);
    playAgainMenu->setPosition(Vec2::ZERO);
    this->addChild(playAgainMenu);
    
    
}

void HelloWorld::playAgainCallBack(Ref* pSender){
    
    
    for(b2Body*b = world->GetBodyList(); b; b= b->GetNext()) {
        
        Sprite* s = (Sprite*)b->GetUserData();
            
            if (s!=NULL) {
                s->removeFromParent();
            }
        
            world->DestroyBody(b);
        
        
    }
    
    this->removeChild(line);
    blackLayer->removeFromParent();
    playAgainMenu->removeFromParent();
    cool->removeFromParent();    this->startButtonCallBack(pSender);
    
}

HelloWorld::~HelloWorld(){
    CC_SAFE_DELETE(world);
}
