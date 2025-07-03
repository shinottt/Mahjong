#ifndef _MAHJONG_SHINOHAYU_LANGUAGE_HEADER_
#define _MAHJONG_SHINOHAYU_LANGUAGE_HEADER_

#include<unordered_map>
#include<string>
#include<vector>
#include<memory>
#include<filesystem>
#include<stdexcept>
#include<fstream>

#include<nlohmann/json.hpp>

#include"basic.hpp"

enum class Language{
    Chinese, Japanese, English
};

class language_manager : public singleton<language_manager>{
    friend class singleton<language_manager>;
public:

    // Todo
    void init(const Language lang_);

    //Todo
    void switch_lang(const Language switch_lang);

    std::string get_str(const std::string lang_key) const;

private:
    language_manager()=default;

    std::string lang_folder_path_ = "./assets/lang";
    Language current_lang_;
    std::unordered_map<std::string, std::string> current_lang_map_;

    // 读取单个json文件并转化为map
    void json_to_map();

};







#endif //_MAHJONG_SHINOHAYU_LANGUAGE_HEADER_