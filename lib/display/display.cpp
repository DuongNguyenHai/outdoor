#include "display.h"

// SystemDisplay::SystemDisplay() {}
SystemDisplay::~SystemDisplay() {}

bool SystemDisplay::numberIndividual(unsigned char led, unsigned char n) {
   return display(led, n);
}

bool SystemDisplay::number(int n) {
   return displayDual(n);
}

bool SystemDisplay::character(unsigned char led, char c) {
   return displayCharacter(led, c);
}

bool SystemDisplay::string(const char *s) {
   return displayDualCharacter(s);
}

SystemDisplay Display(LATCH, CLK, DATA);
