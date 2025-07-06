#ifndef _MAHJONG_SHINOHAYU_CLIENT_GAME_H_111_
#define _MAHJONG_SHINOHAYU_CLIENT_GAME_H_111_

#include<string>
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include<glm/glm.hpp>

#include"basic.hpp"
#include"asset_store.h"
#include"language.h"

class game_client : public singleton<game_client>{
    friend class singleton<game_client>;
public:

    void init(const std::string, int width, int height);
    void run();
    void handle_events();
    void update(float dt);
    void clean();
    void render();

private:
    
    bool is_running_ = true;

    glm::vec2 window_size_ = glm::vec2(0,0);
    SDL_Window* window_ = NULL;
    SDL_Renderer* renderer_ = NULL;
    TTF_TextEngine* ttf_engine_ = NULL;


    Uint64 FPS_ = 60;
    Uint64 frame_delay_ = 0;        //纳秒，在init()中赋值
    float dt_ = 0.0f;

    std::shared_ptr<asset_store> asset_store_ = asset_store::get_instance();
    std::shared_ptr<language_manager> language_manager_ = language_manager::get_instance();


};






#endif      //_MAHJONG_SHINOHAYU_CLIENT_GAME_H_111_