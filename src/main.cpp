#include "application.hpp"

int main(int argc, char **argv) {
  Application app;
  if (!app.Init()) {
    return 1;
  }

  app.Run();

  app.Cleanup();

  return 0;
}
