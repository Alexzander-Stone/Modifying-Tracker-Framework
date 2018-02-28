#include <string>
#include "drawable.h"

class TwoWaySprite : public Drawable {
public:
  TwoWaySprite(const std::string&);
  TwoWaySprite(const TwoWaySprite&);
  virtual ~TwoWaySprite() { } 

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return images[currentFrame]; }
  virtual const SDL_Surface* getSurface() const { 
    return images[currentFrame]->getSurface();
  }
  int getScaledWidth()  const { return getScale()*images[currentFrame]->getWidth();  } 
  int getScaledHeight() const { return getScale()*images[currentFrame]->getHeight(); } 

protected:
  std::vector<Image *> images;
  unsigned currentFrame;
  unsigned numberOfFrames;
  int worldWidth;
  int worldHeight;

  void advanceFrame();
  TwoWaySprite& operator=(const TwoWaySprite&);
  int getDistance(const TwoWaySprite*) const;
  Vector2f makeVelocity(int, int) const;
};
