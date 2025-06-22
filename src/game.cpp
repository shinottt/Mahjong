#include"game.h"

void game::init(const std::string title, int width, int height){
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


void game::run(){
    //游戏主循环
    /**测试内容**/

    SDL3_audio_stream test_music_3("./assets/music/Secret utopIA (八宮めぐる Ver.).wav");

    test_music_3.open_audio_stream();

    test_music_3.play_audio_stream();


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


void game::handle_events(){
    SDL_Event event;
    while(SDL_PollEvent(&event)){
        switch (event.type) {
            case SDL_EVENT_QUIT:
                is_running_ = false;
                break;
            default:
                break;
        }
    }
}



void game::render(){
    //Todo, 现在只是测试
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);
    SDL_RenderPresent(renderer_);
}



void game::clean(){

    asset_store_.reset();
    language_manager_.reset();

    TTF_DestroyRendererTextEngine(ttf_engine_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    TTF_Quit();
    SDL_Quit();
}

