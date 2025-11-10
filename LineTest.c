#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
 
int widht = 1280;
int hight = 600;
SDL_Event windowEvent;

struct waveShittings{
    float Freq;
    float TAmp;
    float Amp;
    float PhaseShift;

    float Attack;
    float Decay;
    float Sustain;
    float Release;

    int WasMax;
};

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

  int CurrentWave = 0;

  int current_sine_sample = 0;
  int current_saw_sample = 0;

  int current_note = 0;

  struct waveShittings WaveShittings[2];

  WaveShittings[0].Freq = 0;
  WaveShittings[0].TAmp = 0;
  WaveShittings[0].Amp = 0;
  WaveShittings[0].Attack = 0.1f;
  WaveShittings[0].Decay = 0.1f;
  WaveShittings[0].Sustain = 0.5f;
  WaveShittings[0].Release = 0.1f;
  WaveShittings[0].WasMax = 0;
  WaveShittings[0].PhaseShift = 0;

  WaveShittings[1].Freq = 0;
  WaveShittings[1].TAmp = 0;
  WaveShittings[1].Amp = 0;
  WaveShittings[1].Attack = 0.1f;
  WaveShittings[1].Decay = 0.1f;
  WaveShittings[1].Sustain = 0.5f;
  WaveShittings[1].Release = 0.1f;
  WaveShittings[1].WasMax = 0;
  WaveShittings[1].PhaseShift = 0;

  float ScaleMultiplyer = 0.01f;
  
  int keyDown = 0;

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
            WaveShittings[CurrentWave].Freq += 2;
            continue;
          case SDL_SCANCODE_DOWN:
            WaveShittings[CurrentWave].Freq -= 2;
            continue;
          case SDL_SCANCODE_LEFT:
            WaveShittings[CurrentWave].Amp += 0.5f;
            continue;
          case SDL_SCANCODE_RIGHT:
            WaveShittings[CurrentWave].Amp -= 0.5f;
            continue;
          case SDL_SCANCODE_1:
            CurrentWave = 0;
            continue;
          case SDL_SCANCODE_2:
            CurrentWave = 1;
            continue;
          case SDL_SCANCODE_Q:
            WaveShittings[CurrentWave].Attack += 0.1f;
            continue;
          case SDL_SCANCODE_A:
            WaveShittings[CurrentWave].Attack -= 0.1f;
            continue;
          case SDL_SCANCODE_W:
            WaveShittings[CurrentWave].Decay += 0.1f;
            continue;
          case SDL_SCANCODE_S:
            WaveShittings[CurrentWave].Decay -= 0.1f;
            continue;
          case SDL_SCANCODE_E:
            WaveShittings[CurrentWave].Sustain += 0.1f;
            continue;
          case SDL_SCANCODE_D:
            WaveShittings[CurrentWave].Sustain -= 0.1f;
            continue;
          case SDL_SCANCODE_R:
            WaveShittings[CurrentWave].Release += 0.1f;
            continue;
          case SDL_SCANCODE_F:
            WaveShittings[CurrentWave].Release -= 0.1f;
            continue;
          case SDL_SCANCODE_Z:
            WaveShittings[CurrentWave].PhaseShift += 0.1f;
            continue;
          case SDL_SCANCODE_X:
            WaveShittings[CurrentWave].PhaseShift -= 0.1f;
            continue;
          case SDL_SCANCODE_G:
            current_note = 3;
            keyDown = 1;
            continue;
          case SDL_SCANCODE_H:
            current_note = 5;
            keyDown = 1;
            continue;
          case SDL_SCANCODE_J:
            current_note = 7;
            keyDown = 1;
            continue;
          case SDL_SCANCODE_K:
            current_note = 8;
            keyDown = 1;
            continue;
          case SDL_SCANCODE_L:
            current_note = 11;
            keyDown = 1;
            continue;
          case SDL_SCANCODE_SEMICOLON:
            current_note = 13;
            keyDown = 1;
            continue;
          case SDL_SCANCODE_APOSTROPHE:
            current_note = 14;
            keyDown = 1;
            continue;
        }
      }
      if(windowEvent.type == SDL_EVENT_KEY_UP){
        switch(sc){
        case SDL_SCANCODE_SPACE:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        case SDL_SCANCODE_G:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        case SDL_SCANCODE_H:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        case SDL_SCANCODE_J:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        case SDL_SCANCODE_K:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        case SDL_SCANCODE_L:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        case SDL_SCANCODE_SEMICOLON:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        case SDL_SCANCODE_APOSTROPHE:
          keyDown = 0;
          WaveShittings[0].WasMax = 0;
          WaveShittings[1].WasMax = 0;
          continue;
        }
      }

    }

    const int minimum_audio = (8000 * sizeof (float)) / 2;
    static float SineSamples[512];
    static float SawSamples[512];
    if (SDL_GetAudioStreamQueued(SineStream) < minimum_audio){
      for (int i=0;i<=512;i++) {
        x = i*1;
        float freq = 440*powf(2.0f,current_note/12.0f)+WaveShittings[0].Freq;
        float phase = WaveShittings[0].PhaseShift+current_sine_sample*(freq/8000.0f);
        if (keyDown){
          if(WaveShittings[0].TAmp < abs(WaveShittings[0].Amp) && !WaveShittings[0].WasMax){
            WaveShittings[0].TAmp += WaveShittings[0].Attack*ScaleMultiplyer;
          }else if (WaveShittings[0].TAmp >= abs(WaveShittings[0].Amp) && !WaveShittings[0].WasMax){
            WaveShittings[0].WasMax = 1;
          }else if (WaveShittings[0].TAmp > abs(WaveShittings[0].Amp*WaveShittings[0].Sustain) && WaveShittings[0].WasMax) {
            WaveShittings[0].TAmp -= WaveShittings[0].Decay*ScaleMultiplyer;
          }
        }else{
          if(WaveShittings[0].TAmp > 0){
            WaveShittings[0].TAmp -= WaveShittings[0].Release*ScaleMultiplyer;
          }else if (WaveShittings[0].TAmp <= 0) {
            WaveShittings[0].TAmp = 0;
            WaveShittings[0].WasMax = 0;
          }
        }
        y = WaveShittings[0].TAmp*sin(1*phase*3.141592635f);
        SineSamples[i] = y;
        current_sine_sample++;
        SDL_FPoint xy;
        xy.y = y+100;
        xy.x = x+192;
        Sinevalues[i] = xy;
      }
      current_sine_sample %= 8000;
    
      for (int i=0;i<=512;i++){
        x = i*1;
        float freq = 440*powf(2.0f,current_note/12.0f)+WaveShittings[1].Freq;
        float phase = WaveShittings[1].PhaseShift+current_saw_sample*(freq/8000.0f);
        if (keyDown){
          if(WaveShittings[1].TAmp < abs(WaveShittings[1].Amp) && !WaveShittings[1].WasMax){
            WaveShittings[1].TAmp += WaveShittings[1].Attack*ScaleMultiplyer;
          }else if (WaveShittings[1].TAmp >= abs(WaveShittings[1].Amp) && !WaveShittings[1].WasMax){
            WaveShittings[1].WasMax = 1;
          }else if (WaveShittings[1].TAmp > abs(WaveShittings[1].Amp*WaveShittings[1].Sustain) && WaveShittings[1].WasMax) {
            WaveShittings[1].TAmp -= WaveShittings[1].Decay*ScaleMultiplyer;
          }
        }else{
          if(WaveShittings[1].TAmp > 0){
            WaveShittings[1].TAmp -= WaveShittings[1].Release*ScaleMultiplyer;
          }else if (WaveShittings[1].TAmp <= 0) {
            WaveShittings[1].TAmp = 0;
            WaveShittings[1].WasMax = 0;
          }
        }
        y = WaveShittings[1].TAmp*(phase*1-floor(phase*1));
        SineSamples[i] += y;
        current_saw_sample++;
        Sinevalues[i].y += y;
      }
      current_saw_sample %= 8000;
    }


    if (SDL_GetAudioStreamQueued(SineStream) < minimum_audio){
      SDL_PutAudioStreamData(SineStream, SineSamples, sizeof (SineSamples));
    }
    /*
    if(SDL_GetAudioStreamQueued(SawStream) < minimum_audio){
      SDL_PutAudioStreamData(SawStream, SawSamples, sizeof (SawSamples));
    }*/

    SDL_SetRenderScale(renderer, 1.0f,1.0f);
    SDL_SetRenderDrawColor(renderer,0,0,0,1);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer,255,0,0,1);
    SDL_SetRenderScale(renderer, 1.25f,1.25f);
    SDL_RenderLines(renderer, Sinevalues,  SDL_arraysize(Sinevalues));
    SDL_SetRenderDrawColor(renderer,255,0,0,1);
    //SDL_RenderLines(renderer, Sawvalues,  SDL_arraysize(Sawvalues));

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    SDL_SetRenderScale(renderer, 1.5f,1.5f);
    SDL_RenderDebugTextFormat(renderer, 10, 200, "Current Wave (1/2): %i", CurrentWave+1);
    SDL_RenderDebugTextFormat(renderer, 300, 200, "Phase Shift (Z/X): %f", WaveShittings[CurrentWave].PhaseShift);
    SDL_RenderDebugTextFormat(renderer, 10, 240, "Freq (^): %f", WaveShittings[CurrentWave].Freq);
    SDL_RenderDebugTextFormat(renderer, 300, 240, "Amp (<>): %f", WaveShittings[CurrentWave].Amp);
    SDL_RenderDebugTextFormat(renderer, 10, 280, "keyDown (Spa): %i", keyDown);
    SDL_RenderDebugTextFormat(renderer, 300, 280, "TAmp: %f", WaveShittings[CurrentWave].TAmp);
    SDL_RenderDebugTextFormat(renderer, 10, 320, "Attack (Q/A): %f", WaveShittings[CurrentWave].Attack);
    SDL_RenderDebugTextFormat(renderer, 300, 320, "Decay (W/S): %f", WaveShittings[CurrentWave].Decay);
    SDL_RenderDebugTextFormat(renderer, 10, 360, "Sustain (E/D): %f", WaveShittings[CurrentWave].Sustain);
    SDL_RenderDebugTextFormat(renderer, 300, 360, "Release (R/F): %f", WaveShittings[CurrentWave].Release);
    
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyAudioStream(SineStream);
  SDL_DestroyAudioStream(SawStream);

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;

}
