#include <cmath>
#include <random>
#include <functional>
#include "twoWaySprite.h"
#include "gamedata.h"
#include "renderContext.h"

void TwoWaySprite::advanceFrame() {
    currentFrame = (currentFrame+1) % numberOfFrames;
}

Vector2f TwoWaySprite::makeVelocity(int vx, int vy) const {
  float newvx = Gamedata::getInstance().getRandFloat(vx-50,vx+50);
  float newvy = Gamedata::getInstance().getRandFloat(vy-50,vy+50);
  newvx *= [](){ if(rand()%2) return -1; else return 1; }();
  newvy *= [](){ if(rand()%2) return -1; else return 1; }();

  return Vector2f(newvx, newvy);
}

TwoWaySprite::TwoWaySprite(const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"), 
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")), 
           makeVelocity(
                    Gamedata::getInstance().getXmlInt(name+"/speedX"), 
                    Gamedata::getInstance().getXmlInt(name+"/speedY")) 
           ),

  images( RenderContext::getInstance()->getImages(name) ),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwoWaySprite::TwoWaySprite(const TwoWaySprite& s) :
  Drawable(s), 
  images(s.images),
  currentFrame(s.currentFrame),
  numberOfFrames( s.numberOfFrames ),
  worldWidth(s.worldWidth),
  worldHeight(s.worldHeight)
{ }

TwoWaySprite& TwoWaySprite::operator=(const TwoWaySprite& rhs) {
  Drawable::operator=( rhs );
  images = rhs.images;
  currentFrame = (rhs.currentFrame);
  numberOfFrames = (rhs.numberOfFrames);
  worldWidth = rhs.worldWidth;
  worldHeight = rhs.worldHeight;
  return *this;
}

inline namespace{
  constexpr float SCALE_EPSILON = 2e-7;
}

void TwoWaySprite::draw() const { 
  images[currentFrame]->draw(getX(), getY(), getScale()); 
}

void TwoWaySprite::update(Uint32 ticks) { 
  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( std::abs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -std::abs( getVelocityY() ) );
  }

  // When the changes velocity, "flip" the image.
  if(currentFrame != 0 && getVelocityX() >= 0)
    currentFrame = 0;
  else if(currentFrame != 1 && getVelocityX() < 0)
    currentFrame = 1;

  if ( getX() < 0) {
    setVelocityX( std::abs( getVelocityX() ) );
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -std::abs( getVelocityX() ) );
  }  
}
