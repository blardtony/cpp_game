// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1400; // Width of window in pixel
constexpr unsigned frame_height = 900; // Height of window in pixel
// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;

// Helper function to initialize SDL
void init();

//animal
class animal {
  private:
    SDL_Surface* window_surface_ptr_;
    SDL_Surface* image_ptr_;
  public:
    animal(const std::string& file_path, SDL_Surface* window_surface_ptr);
    ~animal(); 

    void draw();
    virtual void move() {}; 

  protected:
    int width;
    int height;
    int x;
    int y;
    int speed;
};

//sheep
class sheep : public animal {
  public:
    sheep(SDL_Surface *window_surface_ptr);
    virtual ~sheep();
    virtual void move();
};

//wolf
class wolf : public animal {
  public:
    wolf(SDL_Surface *window_surface_ptr);
    virtual ~wolf();
    virtual void move();
};


//ground
class ground {
  private:
    SDL_Surface* window_surface_ptr_;
    std::vector<animal *> zoo;

  public:
    ground(SDL_Surface* window_surface_ptr, unsigned n_sheep, unsigned n_wolf);
    ~ground(){};
    void add_animal(animal *animal);
    void update(); 
    void draw();
};


//application
class application {
private:
  SDL_Window* window_ptr_;
  SDL_Surface* window_surface_ptr_;
  SDL_Event window_event_;

  ground *ptr_ground;
  SDL_Renderer *ptr_window_renderer;
public:
  application(unsigned n_sheep, unsigned n_wolf);
  ~application();                                 

  int loop(unsigned period); // main loop of the application.
                             // this ensures that the screen is updated
                             // at the correct rate.
                             // See SDL_GetTicks() and SDL_Delay() to enforce a
                             // duration the application should terminate after
                             // 'period' seconds
};