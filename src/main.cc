#include <glib.h>
#include <libnotify/notify.h>
#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <stdexcept>
#include <string>

#include "handler.hh"
#include "inspector.hh"

int main(int argc, char** argv) {
  std::string const HOME = getenv("HOME");
  std::string const APPLICATION_DIR = HOME + "/Applications";

  notify_init("inspector");

  try {
    rlxos::Inspector inspector;
    inspector.add(APPLICATION_DIR.c_str());
    inspector.start();

  } catch (std::exception const& exc) {
    std::cerr << "Error! " << exc.what() << std::endl;
  }

  notify_uninit();
  return 0;
}