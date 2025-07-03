#ifndef _MAHJONG_SHINOHAYU_ASSET_STORE_HEADER_
#define _MAHJONG_SHINOHAYU_ASSET_STORE_HEADER_

#include<unordered_map>    
#include<string>
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include<SDL3_image/SDL_image.h>

#include"basic.hpp"

class asset_store : public singleton<asset_store>{
    friend class singleton<asset_store>;
public:
    asset_store() = default;
    ~asset_store() = default;

    void init(SDL_Renderer* renderer_ptr){
        renderer_ = renderer_ptr;
    }

    // 载入函数
    void load_image(const std::string& file_path);
    void load_sound(const std::string& file_path);  //Todo
    void load_audio(const std::string& file_path);  //Todo
    void load_font(const std::string& file_path, int font_size);


    // 载入函数
    SDL_Texture* get_image(const std::string& file_path);
    TTF_Font* get_font(const std::string& file_path, int font_size);

    void clean();

private:

    SDL_Renderer* renderer_ = NULL;

    std::unordered_map<std::string, SDL_Texture*> textures_;
    std::unordered_map<std::string, TTF_Font*> fonts_;
};



#endif  //_MAHJONG_SHINOHAYU_ASSET_STORE_HEADER_