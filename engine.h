#include <vector>
#include <SDL2/SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"

class Engine {
public:
  Engine ();
  ~Engine ();
  // Disallow copy and assignment constructors explicitly.
  Engine (const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;  
  void play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;
  // Background images.
  World frontTreeWorld;
  World middleTreeWorld;
  World backTreeWorld;
  World mountainWorld;
  World cloudWorld;
  World mistWorld;
  
  Viewport& viewport;

  std::vector<Drawable*> spriteContainer;
  int currentSprite;

  bool makeVideo;

  void draw() const;
  void update(Uint32);
  // Removed copy and assignment constructors to delete within the public
  // section.
  void printScales() const;
  void checkForCollisions();
};
