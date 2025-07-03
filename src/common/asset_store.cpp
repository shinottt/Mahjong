#include"asset_store.h"


void asset_store::load_image(const std::string& file_path){
    SDL_Texture* texture = IMG_LoadTexture(renderer_, file_path.c_str());
    if(texture == NULL){
        SDL_Log("Failed to load image: %s\n Error: %s", file_path.c_str(), SDL_GetError());
        return;
    }
    textures_.emplace(file_path, texture);      //如果已存在，则不重复加载
}

void asset_store::load_font(const std::string& file_path, int font_size){
    TTF_Font *font = TTF_OpenFont(file_path.c_str(), font_size);
    if(font == NULL){
        SDL_Log("Failed to load font: %s\n Error: %s", file_path.c_str(), SDL_GetError());
        return;
    }
    fonts_.emplace(file_path + std::to_string(font_size), font);      //如果已存在，则不重复加载
}


SDL_Texture* asset_store::get_image(const std::string& file_path){
    auto iter = textures_.find(file_path);
    // 如果找不到，则加载并再次查找
    if(iter == textures_.end()){
        load_image(file_path);
        iter = textures_.find(file_path);
    }
    // 找不到则返回NULL
    if(iter == textures_.end()){
        SDL_Log("Failed to load and get image: %s\n Error: %s", file_path.c_str(), SDL_GetError());
        return NULL;
    }
    return iter->second;
}


TTF_Font* asset_store::get_font(const std::string& file_path, int font_size){
    std::string key = file_path + std::to_string(font_size);
    auto iter = fonts_.find(key);
    if(iter == fonts_.end()){
        load_font(file_path, font_size);
        iter = fonts_.find(key);
    }
    if(iter == fonts_.end()){
        SDL_Log("Failed to load and get font: %s\n Error: %s", file_path.c_str(), SDL_GetError());
        return NULL;
    }
    return iter->second;
}


void asset_store::clean(){
    for(auto& texture : textures_){
        SDL_DestroyTexture(texture.second);
    }
    textures_.clear();
    for(auto& font : fonts_){
        TTF_CloseFont(font.second);
    }
    fonts_.clear();
}


