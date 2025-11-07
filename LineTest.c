#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int widht = 600;
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

  int waveType = 1;

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
    }
    const int minimum_audio = (8000 * sizeof (float)) / 2;
    static float SineSamples[512];
    static float SawSamples[512];
    if (SDL_GetAudioStreamQueued(stream) < minimum_audio){
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

      //SDL_PutAudioStreamData(stream, SineSamples, sizeof (SineSamples));
    }
    
    if(SDL_GetAudioStreamQueued(stream) < minimum_audio){
      for (int i=0;i<=512;i++){
        x = i*1;
        float phase = current_saw_sample*Freq/8000.0f;
        y = Amp*(phase*2-floor(phase*2));
        SawSamples[i] = y;
        current_saw_sample++;
        SDL_FPoint xy;
        xy.y = y+150;
        xy.x = x;
        Sawvalues[i] = xy;
      }
      current_saw_sample %= 8000;

      SDL_PutAudioStreamData(stream, SawSamples, sizeof (SawSamples));
    }
    

    SDL_SetRenderScale(renderer, 1.0f,1.0f);
    SDL_SetRenderDrawColor(renderer,0,0,0,1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,255,255,255,1);
    SDL_RenderLines(renderer, Sinevalues,  SDL_arraysize(Sinevalues));
    SDL_RenderLines(renderer, Sawvalues,  SDL_arraysize(Sawvalues));

    SDL_SetRenderDrawColor(renderer,255,255,255,1);
    SDL_RenderDebugTextFormat(renderer, 50, 140, "Freq: %f", Freq);
    SDL_RenderDebugTextFormat(renderer, 50, 160, "Amp: %f", Amp);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

}
