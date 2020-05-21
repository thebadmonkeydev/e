/*
 *  E
 */
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

struct termios orig_termios;

void
disable_raw_mode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH,  &orig_termios);
}

void
enable_raw_mode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int
main(int argc, char *argv[])
{
  // Enable raw mode and other convinient terminal settings
  // runs disable_raw_mode() using atexit()
  enable_raw_mode();

  char c;
  while (read(STDIN_FILENO, &c, 1) && c != 'q') {
    if (iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
    }
  }

  return 0;
}
