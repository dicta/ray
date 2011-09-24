#include <SDL.h>
#include "GUI/SDLApp.h"
#include <limits>

int main(int argc, char *argv[])
{
   SDLApp app(argc, argv);
   app.run();
   return 0;
}
