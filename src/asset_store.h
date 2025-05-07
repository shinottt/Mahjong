#ifndef _MAHJONG_SHINOHAYU_ASSET_STORE_HEADER_
#define _MAHJONG_SHINOHAYU_ASSET_STORE_HEADER_

#include<unordered_map>    
#include<string>
#include<SDL3/SDL.h>
#include<SDL3_ttf/SDL_ttf.h>
#include<SDL3_image/SDL_image.h>

class asset_store{
public:
    asset_store(SDL_Renderer* renderer):renderer_(renderer){}
    asset_store(const asset_store& other) = delete;
    asset_store& operator=(const asset_store& other) = delete;


    void load_image(const std::string& file_path);
    // Todo
    void load_audio(const std::string& file_path);
    void load_font(const std::string& file_path, int font_size);


    SDL_Texture* get_image(const std::string& file_path);
    
    TTF_Font* get_font(const std::string& file_path, int font_size);

    void clean();

private:

    SDL_Renderer* renderer_;

    std::unordered_map<std::string, SDL_Texture*> textures_;

    std::unordered_map<std::string, TTF_Font*> fonts_;
};



#endif  //_MAHJONG_SHINOHAYU_ASSET_STORE_HEADER_