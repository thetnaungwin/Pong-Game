#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <sstream>
#define Screen_Width 129e1
#define Screen_Height 72e1
#define FPS 120
#define FRAME_TARGET_TIME (1000 / FPS)


class MyData{
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    Mix_Music* m_music;
    SDL_Surface* image;
    SDL_Texture* texture;
    SDL_Rect rect_ball={20,20,40,40};
    int last_fram_time=0;
    float Xvel=600.0f;
    float Yvel=500.0f;
  public:
    SDL_Rect left_paddle={30, 280, 10,200};
    SDL_Rect right_paddle={1250, 280, 10,200};
    enum Direction {NONE, UP, DOWN, UPUP, DOWNDOWN};
    Direction currentDirection= NONE;
    int paddle_speed=5;
    int left_score=0;
    int right_score=0;

    MyData(){
      window=SDL_CreateWindow("NULL", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, Screen_Width,Screen_Height,SDL_WINDOW_BORDERLESS);
      renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
      image=SDL_LoadBMP("image/Ball.bmp");
      SDL_SetColorKey(image,SDL_TRUE,SDL_MapRGB(image->format,255,255,255));
      texture=SDL_CreateTextureFromSurface(renderer, image);
    }
    ~MyData(){
      SDL_DestroyWindow(window);
      SDL_DestroyRenderer(renderer);
      Mix_FreeMusic(m_music);
    } 
    void Renderer(){
      SDL_SetRenderDrawColor(renderer,0,0,0,0);
      SDL_RenderClear(renderer);
      
      SDL_RenderCopy(renderer,texture,NULL, &rect_ball);  //ball

      TextRender(renderer,"Score", "font/ARIAL.TTF", 30, (int)Screen_Width/2-45, 3);
      TextRender(renderer, ToString(left_score), "font/ARIAL.TTF", 30, (int)Screen_Width/2-100, 50);
      TextRender(renderer, ToString(right_score), "font/ARIAL.TTF", 30, (int)Screen_Width/2+65, 50);

      SDL_SetRenderDrawColor(renderer, 39,59,245,0.58);
      SDL_RenderFillRect(renderer, &left_paddle);
      SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      SDL_Rect middle_line={(int)Screen_Width/2, 40, 2, (int)Screen_Height};
      SDL_RenderFillRect(renderer, &middle_line);
      SDL_SetRenderDrawColor(renderer, 39,59,245,0.58);
      SDL_RenderFillRect(renderer, &right_paddle);

      SDL_RenderPresent(renderer);
    }
    void UpdateBall(){
      int time_to_wait=FRAME_TARGET_TIME - (SDL_GetTicks() - last_fram_time );
       
      if (time_to_wait>0 && time_to_wait <= FRAME_TARGET_TIME )
      {
        SDL_Delay(time_to_wait);
      }

      float delta=(SDL_GetTicks() - last_fram_time) / 700.0f;
      last_fram_time = SDL_GetTicks();

      rect_ball.x += Xvel * delta;
      rect_ball.y += Yvel * delta;
  
      if(SDL_HasIntersection(&rect_ball, &right_paddle) || SDL_HasIntersection(&rect_ball, &left_paddle)){
        Xvel *= -1;
        MusicRender("music/Jump_Sound.mp3");
        playMusic(1);
      }else if (rect_ball.y >= (Screen_Height - rect_ball.h) || rect_ball.y <= 0){
        Yvel *=-1;
      }
      if (rect_ball.x <0) {
        rect_ball.x=Screen_Width/2;
        Xvel=500.0f;
        Yvel=400.0f;
        right_score +=1;
        MusicRender("music/game_over_sound.mp3");
        playMusic(1);
      }else if (rect_ball.x >Screen_Width){
        rect_ball.x=Screen_Width/2;
        Xvel=-500.0f;
        Yvel=-400.0f;
        left_score +=1;
        MusicRender("music/game_over_sound.mp3");
        playMusic(1);
      }
    }
    void TextRender(SDL_Renderer* renderer, std::string message, std::string fontFile, int fontSize, int x, int y){
      SDL_Color color = { 232, 14, 14, 0};
      TTF_Font* font=TTF_OpenFont(fontFile.c_str(), fontSize);
      SDL_Surface* surface=TTF_RenderText_Solid(font, message.c_str(), color);
      SDL_Texture* texture=SDL_CreateTextureFromSurface(renderer,surface);
      SDL_Rect text_box={x, y, surface->w, surface->h};

      SDL_RenderCopy(renderer,texture,NULL, &text_box);
      SDL_DestroyTexture(texture);
      SDL_FreeSurface(surface);
      TTF_CloseFont(font);
    }
    std::string ToString(int counter){
      std::stringstream ss;
      ss<<counter;
      std::string full=ss.str();
      return full;
    }
    void MusicRender(std::string music_filePath){
      if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)<0)
        std::cout<<"Audio is not ready to run."<<Mix_GetError()<<std::endl;
      m_music=Mix_LoadMUS(music_filePath.c_str());
    }
    void playMusic(int loops){
      Mix_PlayMusic(m_music, loops);
    }
    void pauseMusic(){
      Mix_PauseMusic();
    }
    void resumeMusic(){
      Mix_ResumeMusic();
    }
};