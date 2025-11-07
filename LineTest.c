#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int widht = 512;
int hight = 400;
SDL_Event windowEvent;


int main(int argc, char *argv[]) {
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  SDL_Window *window = SDL_CreateWindow("Line Test", widht, hight, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  SDL_AudioSpec spec;
  spec.channels = 1;
  spec.format = SDL_AUDIO_F32;
  spec.freq = 8000;
  SDL_AudioStream *stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

  SDL_ResumeAudioStreamDevice(stream);

  

  float x = 0;
  float y = 0;
  SDL_FPoint Sinevalues[512];
  SDL_FPoint Sawvalues[512];

  int SineWave = 0;
  int SawWave = 0;

  int current_sine_sample = 0;
  int current_saw_sample = 0;

  float Freq = 200;
  float Amp = -20;

  while (true) {
    if (SDL_PollEvent(&windowEvent)) {
      if (SDL_EVENT_QUIT == windowEvent.type) {
        break;
      }
      SDL_Scancode sc = windowEvent.key.scancode;

      if(sc == SDL_SCANCODE_UP){
        Freq += 2;
      }
      if(sc == SDL_SCANCODE_DOWN){
        Freq -= 2;
      }
      if(sc == SDL_SCANCODE_LEFT){
        Amp += 0.5f;
      }
      if(sc == SDL_SCANCODE_RIGHT){
        Amp -= 0.5f;
      }
      if(sc == SDL_SCANCODE_1){
          SineWave = 1;
      }
      if(sc == SDL_SCANCODE_2){
          SawWave = 1;
      }
      if(sc == SDL_SCANCODE_3){
        SineWave = 0;
        SawWave = 0;
      }
    }
    const int minimum_audio = (8000 * sizeof (float)) / 2;
    static float SineSamples[512];
    static float SawSamples[512];
    if (SDL_GetAudioStreamQueued(stream) < minimum_audio && SineWave){
      for (int i=0;i<=512;i++) {
        x = i*1;
        float phase = current_sine_sample*(Freq/8000.0f);
        y = Amp*sin(1*phase*3.141592635f);
        SineSamples[i] = y;
        current_sine_sample++;
        SDL_FPoint xy;
        xy.y = y+50;
        xy.x = x;
        Sinevalues[i] = xy;
      }

      current_sine_sample %= 8000;
    }
    
    if (SDL_GetAudioStreamQueued(stream) < minimum_audio && SawWave){
      for (int i=0;i<=512;i++){
        x = i*1;
        float phase = current_saw_sample*(Freq/8000.0f);
        y = Amp*(phase*1-floor(phase*1));
        SawSamples[i] = y;
        current_saw_sample++;
        SDL_FPoint xy;
        xy.y = y+150;
        xy.x = x;
        Sawvalues[i] = xy;
      }
      current_saw_sample %= 8000;
 
    }


    if (SDL_GetAudioStreamQueued(stream) < minimum_audio){
      if (SineWave){
      SDL_PutAudioStreamData(stream, SineSamples, sizeof (SineSamples));
      }
      if(SawWave){
      SDL_PutAudioStreamData(stream, SawSamples, sizeof (SawSamples));
      }
    }

    SDL_SetRenderScale(renderer, 1.0f,1.0f);
    SDL_SetRenderDrawColor(renderer,0,0,0,1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,1);
    SDL_RenderLines(renderer, Sinevalues,  SDL_arraysize(Sinevalues));
    SDL_SetRenderDrawColor(renderer,255,0,0,1);
    SDL_RenderLines(renderer, Sawvalues,  SDL_arraysize(Sawvalues));

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_RenderDebugTextFormat(renderer, 10, 200, "Freq: %f", Freq);
      SDL_RenderDebugTextFormat(renderer, 10, 240, "Amp: %f", Amp);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

}
