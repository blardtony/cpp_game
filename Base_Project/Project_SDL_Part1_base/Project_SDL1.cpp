// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

void init() {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));
}

namespace {

SDL_Surface* load_surface_for(const std::string& path, SDL_Surface* window_surface_ptr) {
  
  auto surface = IMG_Load(path.c_str());
  if (!surface)
  {
    throw std::runtime_error("Loading image failed!!!");
  }
  auto surface_ptr = SDL_ConvertSurface(surface, window_surface_ptr->format, 0);
  
  if (surface_ptr == NULL)
  {
    std::cout << ("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError()) << std::endl;
  }
  SDL_FreeSurface(surface);
  
  return surface_ptr;
}
}

// Application
application::application(unsigned n_sheep, unsigned n_wolf)
{
  std::cout << "Constructor application" << std::endl;

  window_ptr_ = SDL_CreateWindow("Windows game", SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED, 1200,800, SDL_WINDOW_SHOWN);
  if (!window_ptr_)
    throw std::runtime_error("Failed window");

  window_surface_ptr_ = SDL_GetWindowSurface(window_ptr_);

  if (!window_surface_ptr_)
    throw std::runtime_error("Failed surface window");
  
  this->ptr_ground = new ground(this->window_surface_ptr_, n_sheep, n_wolf);
}

int application::loop(unsigned period)
{
  std::cout << "Method loop" << std::endl;
  auto time = SDL_GetTicks();
  bool quit = false;
  
  SDL_Event e;
  while (!quit && (SDL_GetTicks() - time < period * 1000))
  {
    while (SDL_PollEvent(&e))
    {
      if (e.type == SDL_QUIT)
      {
        quit = true;
      }
    }
    this->ptr_ground->update();
    SDL_UpdateWindowSurface(window_ptr_);
  }
   
  return 0;
}
application::~application()
{
  std::cout << "Destructor application" << std::endl;
  SDL_DestroyRenderer(this->ptr_window_renderer);
  SDL_DestroyWindow(this->window_ptr_);
}

// Ground

ground::ground(SDL_Surface* window_surface_ptr, unsigned n_sheep, unsigned n_wolf)
{
  this->window_surface_ptr_ = window_surface_ptr;
  std::cout << "Constructor ground" << std::endl;
  this->zoo = std::vector<animal *>();
  for (unsigned i = 0; i < n_sheep; ++i)
  {
    this->add_animal(new sheep(window_surface_ptr));
  }
  for (unsigned i = 0; i < n_wolf; ++i)
  {
    this->add_animal(new wolf(window_surface_ptr));
  }
}

void ground::add_animal(animal *animal)
{
  
  std::cout << "Add animal" << std::endl;
  this->zoo.push_back(animal);
}

void ground::update()
{
  std::cout << "Update ground" << std::endl;
  for (animal * animal : this->zoo)
  {
    animal->move();
  }
  SDL_FillRect(window_surface_ptr_, nullptr,
                 SDL_MapRGB(window_surface_ptr_->format, 0xFF, 0xFF, 0xFF));
  
  std::cout<< "Here" << std::endl;
  for (animal * animal : this->zoo)
  {
    animal->draw();
  }
}


// animal
animal::animal(const std::string& file_path, SDL_Surface* window_surface_ptr)
{
  std::cout << "Constructor animal" << std::endl;
  window_surface_ptr_ = window_surface_ptr;
  image_ptr_ = load_surface_for(file_path, window_surface_ptr_);
  this->width = image_ptr_->w;
  this->height = image_ptr_->h;
}

animal::~animal()
{
    SDL_FreeSurface(image_ptr_);
    image_ptr_ = NULL;
}

void animal::draw()
{
  auto rect = SDL_Rect{x, y, image_ptr_->h, image_ptr_->w};
  if (SDL_BlitSurface(image_ptr_, NULL, window_surface_ptr_, &rect))
    throw std::runtime_error("Bug texture");

}

//sheep
sheep::sheep(SDL_Surface *window_surface_ptr):animal("../../media/sheep.png", window_surface_ptr)
{
  std::cout << "Constructor sheep" << std::endl;
  x = 10;
  y = 10;
}

sheep::~sheep()
{
    std::cout << "Destructor sheep" << std::endl;
}

void sheep::move()
{
  std::cout << "Move sheep" << std::endl;

}

//wolf
wolf::wolf(SDL_Surface *window_surface_ptr):animal("../../media/wolf.png", window_surface_ptr)
{
  std::cout << "Constructor wolf" << std::endl;
  x = 500;
  y = 500;
}
wolf::~wolf()
{
    std::cout << "Destructor sheep" << std::endl;
}

void wolf::move()
{
  std::cout << "Move wolf" << std::endl;
  //todo Refactor : Choices : up or down or left or rigth
  int width = 1200;
  int heigth = 800;
  x+=1;
  y+=1;
  if (x < 0) {
    x = 0;
  }
  std::cout << this->width << std::endl;
  if (x > (width - this->width)) {
    x = width - this->width;
  }
  if (y < 0) {
    y = 0;
  }
  if (y > height - this->height) {
    y = height - this->height;
  }
}