#ifndef _MAHJONG_SHINOHAYU_SDL3_AUDIO_654f_HEADER_
#define _MAHJONG_SHINOHAYU_SDL3_AUDIO_654f_HEADER_

#include<chrono>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<atomic>
#include<SDL3/SDL.h>

class SDL3_audio_stream{
public:
    SDL3_audio_stream(char* wav_path):stream_(NULL), wav_data_(NULL), wav_length_(0), wav_path_(wav_path){}

    ~SDL3_audio_stream(){
        exit_ = true;
        if(condition_thread_.joinable()){
            condition_thread_.join();
        }
        cond_var_.notify_all();
        if(put_thread_.joinable()){
            put_thread_.join();
        }
        SDL_free(stream_);
        SDL_free(wav_data_);
        SDL_free(wav_path_);
    }

    void open_audio_stream(){
        // 加载wav文件
        if(!SDL_LoadWAV(wav_path_, &wav_spec_, &wav_data_, &wav_length_)){
            SDL_Log("SDL_LoadWAV{%s} Error: %s", wav_path_, SDL_GetError());
            exit(SDL_APP_FAILURE);
        }
        // 通常情况下的audio init函数
        stream_ = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &wav_spec_, NULL, NULL);
        if(!stream_){
            SDL_Log("SDL_OpenAudioDeviceStream Error: %s", SDL_GetError());
            exit(SDL_APP_FAILURE);
        }
    }
    void play_audio_stream(){
        SDL_ResumeAudioStreamDevice(stream_);
        condition_thread_ = std::thread([this](){
            while(!exit_){
                if(SDL_GetAudioStreamAvailable(stream_)<(int)wav_length_){
                    cond_var_.notify_one();
                }
                // 唤醒频率，1秒60次
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        });
        put_thread_ = std::thread([this](){put_audio_stream_data();});
    }
    void pause_audio_stream(){
        SDL_PauseAudioStreamDevice(stream_);
    }

    void resume_audio_stream(){
        SDL_ResumeAudioStreamDevice(stream_);
    }

private:

    SDL_AudioStream* stream_;
    Uint8* wav_data_;
    Uint32 wav_length_;

    char* wav_path_;
    SDL_AudioSpec wav_spec_;

    std::mutex mutex_;
    std::condition_variable cond_var_;
    std::thread put_thread_;
    std::thread condition_thread_;
    std::atomic<bool> exit_ = false;



    void put_audio_stream_data(){
        while(!exit_){
            std::unique_lock<std::mutex> lock(mutex_);
            cond_var_.wait(lock, [this](){return SDL_GetAudioStreamAvailable(stream_)<(int)wav_length_ || exit_;});
            SDL_PutAudioStreamData(stream_, wav_data_, wav_length_);
        }
    }

};

#endif      //_MAHJONG_SHINOHAYU_SDL3_AUDIO_654f_HEADER_