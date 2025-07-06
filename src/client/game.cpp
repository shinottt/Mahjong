#include"game.h"

void game_client::init(const std::string title, int width, int height){
    window_size_ = glm::vec2(width, height);
    //SDL3初始化
    if(!SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO)){
        SDL_Log("SDL3 failed to init, error: %s", SDL_GetError());
    }
    //SDL3_ttf初始化
    if(!TTF_Init()){
        SDL_Log("SDL3_ttf failed to init, error: %s", SDL_GetError());
    }
    //SDL3_mixer初始化，还没有引入SDL3_mixer库

    SDL_CreateWindowAndRenderer(title.c_str(), width, height, SDL_WINDOW_RESIZABLE, &window_, &renderer_);
    if(!window_ || !renderer_){
        SDL_Log("SDL3 failed to create window and renderer, error: %s", SDL_GetError());
    }
    //设置窗口逻辑分辨率
    SDL_SetRenderLogicalPresentation(renderer_, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    ttf_engine_ = TTF_CreateRendererTextEngine(renderer_);

    frame_delay_ = 1e9 / FPS_;

    asset_store_->init(renderer_);
    language_manager_->init(Language::Chinese);

}

void game_client::run(){
    //游戏主循环
    /**测试内容**/




    /**测试内容**/

    while(is_running_){
        auto start = SDL_GetTicksNS();

        /**测试内容**/



        /**测试内容**/

        // 只是测试
        handle_events();
        render();
        

        // 控制帧数为 FPS_ 设定的值
        auto end = SDL_GetTicksNS();
        auto elapsed = end - start;
        if(elapsed < frame_delay_){
            SDL_DelayNS( frame_delay_ - elapsed);
            dt_ = (frame_delay_ / (float)1e9);
        } else{
            dt_ = (elapsed / (float)1e9);
        }

       //SDL_Log("FPS: %f", 1.0 / dt_);

    }

}

void game_client::clean(){

    asset_store_.reset();
    language_manager_.reset();

    TTF_DestroyRendererTextEngine(ttf_engine_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    TTF_Quit();
    SDL_Quit();
}

