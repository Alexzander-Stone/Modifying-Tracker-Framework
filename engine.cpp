#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"

Engine::~Engine() { 
  delete star;
  delete spinningStar;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  // Background image worlds.
  frontTreeWorld("frontTree", Gamedata::getInstance().getXmlInt("frontTree/factor") ),
  middleTreeWorld("middleTree", Gamedata::getInstance().getXmlInt("middleTree/factor") ),
  backTreeWorld("backTree", Gamedata::getInstance().getXmlInt("backTree/factor") ),
  mountainWorld("mountain", Gamedata::getInstance().getXmlInt("mountain/factor") ),
  cloudWorld("cloud", Gamedata::getInstance().getXmlInt("cloud/factor") ),
  mistWorld("mist", Gamedata::getInstance().getXmlInt("mist/factor") ),
  viewport( Viewport::getInstance() ),
  star(new Sprite("YellowStar")),
  spinningStar(new MultiSprite("SpinningStar")),
  currentSprite(0),
  makeVideo( false )
{
  
  Viewport::getInstance().setObjectToTrack(star);
  std::cout << "Loading complete" << std::endl;
}

void Engine::draw() const {
  // Background images.
  mistWorld.draw();
  cloudWorld.draw();
  mountainWorld.draw();
  backTreeWorld.draw();
  middleTreeWorld.draw();
  frontTreeWorld.draw();

  star->draw();
  spinningStar->draw();

  viewport.draw();
  
  // Screen height and width for drawing.
  int height = Gamedata::getInstance().getXmlInt("view/height");
  int width = Gamedata::getInstance().getXmlInt("view/width");
  // Draw FPS, use different color from overloaded writeText.
  std::stringstream fpsStream;
  fpsStream << clock.getFps();
  string fpsCounter = "FPS: " + fpsStream.str();
  IoMod::getInstance().
      writeText(fpsCounter, width - 100, 0, SDL_Color{220, 60, 242, 255});
  // Draw name.
  IoMod::getInstance().
      writeText("Alexzander Stone", 
                0, 
                height - Gamedata::getInstance().getXmlInt("font/size"), 
                SDL_Color{220, 60, 242, 255});

  SDL_RenderPresent(renderer);

}

void Engine::update(Uint32 ticks) {
  star->update(ticks);
  spinningStar->update(ticks);
  frontTreeWorld.update();
  middleTreeWorld.update();
  backTreeWorld.update();
  mountainWorld.update();
  cloudWorld.update();
  mistWorld.update();
  viewport.update(); // always update viewport last
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(spinningStar);
  }
  else {
    Viewport::getInstance().setObjectToTrack(star);
  }
}

void Engine::play() {
  SDL_Event event;
  const Uint8* keystate;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:
    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_T] ) {
          switchSprite();
        }
        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }

    // In this section of the event loop we allow key bounce:

    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {
      clock.incrFrame();
      draw();      
      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
}
