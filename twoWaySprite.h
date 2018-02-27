#include <string>
#include "drawable.h"

class TwoWaySprite : public Drawable {
public:
  TwoWaySprite(const std::string&);
  TwoWaySprite(const std::string&, const Vector2f& pos, const Vector2f& vel, 
         const Image*);
  TwoWaySprite(const TwoWaySprite&);
  virtual ~TwoWaySprite() { } 
  TwoWaySprite& operator=(const TwoWaySprite&);

  virtual void draw() const;
  virtual void update(Uint32 ticks);

  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const { 
    return image->getSurface();
  }
  int getScaledWidth()  const { return getScale()*image->getWidth();  } 
  int getScaledHeight() const { return getScale()*image->getHeight(); } 

private:
  const Image * image;

protected:
  int worldWidth;
  int worldHeight;

  int getDistance(const TwoWaySprite*) const;
  Vector2f makeVelocity(int, int) const;
};
