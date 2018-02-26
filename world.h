#include <string>
#include "image.h"
#include "viewport.h"

class World {
public:
  World(const std::string&, int fact);
  // ~World() { } // The image will be deleted by the FrameFactory
  void update();
  void draw() const;
private:
  // Background image.
  Image* const image; 
  int factor;
  unsigned worldWidth;
  // Background image widths.
  unsigned backgroundWidth; 
  float viewX;
  float viewY;
  const Viewport & view;
  World(const World&);
  World& operator=(const World&);
};
