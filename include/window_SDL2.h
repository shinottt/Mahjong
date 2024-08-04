#ifndef _WINDOW_SDL2_HEADER_
#define _WINDOW_SDL2_HEADER_


#include"SDL.h"

struct WindowInfo{
    const char* title;
    int width;
    int height;

};



class Window_SDL2{
public:
    SDL_Window* sn_window = nullptr;
    
    WindowInfo windowinfo_1600_900 = {"shino", 1600, 900};

    Window_SDL2()=default;
    Window_SDL2(const Window_SDL2& aim)=delete;
    
    ~Window_SDL2(){
        Release();
    }



    void init();
    void Release();
    void eventLoop();
};

inline void Window_SDL2::init(){
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0){
        SDL_Log("SDL2 video or events initialization failed: %s", SDL_GetError());
        exit(1);
    }

    sn_window = SDL_CreateWindow(
        windowinfo_1600_900.title,
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        windowinfo_1600_900.width, windowinfo_1600_900.height,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if(sn_window == nullptr){
        SDL_Log("SDL2 window creation failed: %s", SDL_GetError());
        exit(1);
    }
    
}


inline void Window_SDL2::Release(){
    SDL_DestroyWindow(sn_window);
    SDL_Quit();
}





#endif   // _WINDOW_SDL2_HEADER_