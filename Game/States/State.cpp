#include "State.hpp"

State::Context::Context(sf::RenderWindow* window, TextureHolder* textures, FontHolder* fonts)
    : window(window),
      textures(textures),
      fonts(fonts)
{
}

State::State(State::Context context, StateStack* ptr)
    : mContext(context),
      mStateStack(ptr)
{
}