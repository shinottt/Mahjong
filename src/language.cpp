#include"language.h"


void language_manager::init(const Language lang_){
    current_lang_ = lang_;
    json_to_map();
    //Todo，可能在这儿更新界面文字渲染
}

void language_manager::switch_lang(const Language switch_lang){
    if(switch_lang == current_lang_){
        return;
    }
    current_lang_map_.clear();
    init(switch_lang);
}

std::string language_manager::get_str(const std::string lang_key) const {
    auto map_it = current_lang_map_.find(lang_key);
    if(map_it == current_lang_map_.end()){
        throw std::runtime_error("Language key not found: " + lang_key);
    }
    return map_it->second;
}



void language_manager::json_to_map(){
    using json = nlohmann::json;

    std::string filename;
    switch(current_lang_){
        case Language::Chinese: filename = "zh-CN.json";break;
        case Language::Japanese: filename = "ja-JP.json";break;
        case Language::English: filename = "en-US.json";break;
        default: throw std::runtime_error("Unknown language");break;
    }
    std::string file_path = lang_folder_path_ + '/' + filename;
    std::ifstream file(file_path.c_str());
    if(!file.is_open()){
        throw std::runtime_error("Failed to open language file: " + file_path);
    }

    json data = json::parse(file);
    current_lang_map_.clear();
    for(auto& [key, val] : data.items()){
        current_lang_map_[key] = val.get<std::string>();
    }
    if(current_lang_map_.empty()){
        throw std::runtime_error("Failed to parse language file: " + file_path);
    }


    file.close();

}

