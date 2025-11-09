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
  SDL_AudioStream *SineStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
  SDL_AudioStream *SawStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);

  SDL_ResumeAudioStreamDevice(SineStream);
  SDL_ResumeAudioStreamDevice(SawStream);

  float x = 0;
  float y = 0;
  SDL_FPoint Sinevalues[512];
  SDL_FPoint Sawvalues[512];

  int SineWave = 0;
  int SawWave = 0;

  int current_sine_sample = 0;
  int current_saw_sample = 0;

  float Freq = 200;
  float TAmp = 0;
  float Amp = -20;

  float Attack = 0.1f;
  float Decay = 0.1f;
  float Sustain = 0.5f;
  float Release = 0.1f;
  float ScaleMultiplyer = 0.01f;

  int keyDown = 0;
  int WasMax = 0;

  while (true) {
    if (SDL_PollEvent(&windowEvent)) {
      if (SDL_EVENT_QUIT == windowEvent.type) {
        break;
      }

      SDL_Scancode sc = windowEvent.key.scancode;
      
      if(windowEvent.type == SDL_EVENT_KEY_DOWN){
        switch (sc) {
          case SDL_SCANCODE_SPACE:
            keyDown = 1;
            continue;
          case SDL_SCANCODE_UP:
            Freq += 2;
            continue;
          case SDL_SCANCODE_DOWN:
            Freq -= 2;
            continue;
          case SDL_SCANCODE_LEFT:
            Amp += 0.5f;
            continue;
          case SDL_SCANCODE_RIGHT:
            Amp -= 0.5f;
            continue;
          case SDL_SCANCODE_1:
            SineWave = 1;
            continue;
          case SDL_SCANCODE_2:
            SawWave = 1;
            continue;
          case SDL_SCANCODE_3:
            SineWave = 0;
            SawWave = 0;
            continue;
          case SDL_SCANCODE_Q:
            Attack += 0.1f;
            continue;
          case SDL_SCANCODE_A:
            Attack -= 0.1f;
            continue;
          case SDL_SCANCODE_W:
            Decay += 0.1f;
            continue;
          case SDL_SCANCODE_S:
            Decay -= 0.1f;
            continue;
          case SDL_SCANCODE_E:
            Sustain += 0.1f;
            continue;
          case SDL_SCANCODE_D:
            Sustain -= 0.1f;
            continue;
          case SDL_SCANCODE_R:
            Release += 0.1f;
            continue;
          case SDL_SCANCODE_F:
            Release -= 0.1f;
            continue;
        }
      }
      if(windowEvent.type == SDL_EVENT_KEY_UP){
        if(sc == SDL_SCANCODE_SPACE){
          keyDown = 0;
          WasMax = 0;
        }
      }

    }

    const int minimum_audio = (8000 * sizeof (float)) / 2;
    static float SineSamples[512];
    static float SawSamples[512];
    if (SDL_GetAudioStreamQueued(SineStream) < minimum_audio){
      for (int i=0;i<=512;i++) {
        x = i*1;
        float phase = current_sine_sample*(Freq/8000.0f);
        if (keyDown){
          if(TAmp < abs(Amp) && !WasMax){
            TAmp += Attack*ScaleMultiplyer;
          }else if (TAmp >= abs(Amp) && !WasMax){
            WasMax = 1;
          }else if (TAmp > abs(Amp*Sustain) && WasMax) {
            TAmp -= Decay*ScaleMultiplyer;
          }
        }else{
          if(TAmp > 0){
            TAmp -= Release*ScaleMultiplyer;
          }else if (TAmp <= 0) {
            TAmp = 0;
            WasMax = 0;
          }
        }
        y = TAmp*sin(1*phase*3.141592635f);
        SineSamples[i] = y;
        current_sine_sample++;
        SDL_FPoint xy;
        xy.y = y+50;
        xy.x = x;
        Sinevalues[i] = xy;
      }
      current_sine_sample %= 8000;
    }
    
    if (SDL_GetAudioStreamQueued(SawStream) < minimum_audio && keyDown){
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


    if (SDL_GetAudioStreamQueued(SineStream) < minimum_audio){
      if (SineWave){
      SDL_PutAudioStreamData(SineStream, SineSamples, sizeof (SineSamples));
      }
    }
    if(SDL_GetAudioStreamQueued(SawStream) < minimum_audio && keyDown){
      if(SawWave){
      SDL_PutAudioStreamData(SawStream, SawSamples, sizeof (SawSamples));
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
    SDL_RenderDebugTextFormat(renderer, 10, 200, "Freq (^): %f", Freq);
    SDL_RenderDebugTextFormat(renderer, 300, 200, "Amp (<>): %f", Amp);
    SDL_RenderDebugTextFormat(renderer, 10, 240, "keyDown (Spa): %i", keyDown);
    SDL_RenderDebugTextFormat(renderer, 300, 240, "TAmp: %f", TAmp);
    SDL_RenderDebugTextFormat(renderer, 10, 280, "Attack (Q/A): %f", Attack);
    SDL_RenderDebugTextFormat(renderer, 300, 280, "Decay (W/S): %f", Decay);
    SDL_RenderDebugTextFormat(renderer, 10, 320, "Sustain (E/D): %f", Sustain);
    SDL_RenderDebugTextFormat(renderer, 300, 320, "Release (R/F): %f", Release);

    SDL_RenderPresent(renderer);
  }

  SDL_DestroyAudioStream(SineStream);
  SDL_DestroyAudioStream(SawStream);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

}
