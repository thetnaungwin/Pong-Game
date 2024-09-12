#include "func.h"

int WinMain(int argc, char* argv[]){
  

  if (SDL_Init(SDL_INIT_EVERYTHING)<0)
  {
    std::cout<<"Window is not ready to run."<< SDL_GetError()<<std::endl;
  }
  if (TTF_Init()<0)
  {
    std::cout<<"Text is not ready to run."<<TTF_GetError()<<std::endl;
  }

  bool running=true;
  SDL_Event event;
  //OOP constructor
  MyData myObj;
 
  myObj.MusicRender("music/stranger-things.MP3");
  myObj.playMusic(-1); 
 
  while (running)
  {
    myObj.Renderer();
    myObj.UpdateBall();
    while (SDL_PollEvent(&event))
    {
      if (event.type)
      {
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          running=false;
          break;
        case SDLK_w:
          myObj.currentDirection=myObj.UP;
          break;
        case SDLK_s:
          myObj.currentDirection=myObj.DOWN;
          break;
        case SDLK_UP:
          myObj.currentDirection=myObj.UPUP;
          break;
        case SDLK_DOWN:
          myObj.currentDirection=myObj.DOWNDOWN;
          break;
        }
      }
    }

    switch (myObj.currentDirection)
    {
    case myObj.UP:
     if (myObj.left_paddle.y>0)
      myObj.left_paddle.y -=myObj.paddle_speed;
     break;
    case myObj.DOWN:
     if (myObj.left_paddle.y<520)
      myObj.left_paddle.y +=myObj.paddle_speed;
     break;
    case myObj.UPUP:
     if (myObj.right_paddle.y>0)
      myObj.right_paddle.y -=myObj.paddle_speed;
      break;
    case myObj.DOWNDOWN:
     if (myObj.right_paddle.y<520)
      myObj.right_paddle.y +=myObj.paddle_speed;
      break;
    }
    
  }
  
  SDL_Quit();
  return 0;
}