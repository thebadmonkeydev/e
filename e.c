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
  // Store original terminal config
  tcgetattr(STDIN_FILENO, &orig_termios);

  // Schedule resetting to original config at exit
  atexit(disable_raw_mode);

  /*
   * Disable terminal flags:
   *
   * ECHO   - By default terminals print the ascii character for key presses automatically
   * ICANON - Canonical mode, waits to return read() until the user presses ENTER
   * ISIG   - Respond to ctrl+c and ctrl+z signals
   * IXON   - Software flow control, ctrl+s and ctrl+q
   * IEXTEN - ctrl+v character literal input
   * ICRNL  - Translates carriage returns into line feeds when input by user (fixes ctrl+m)
   * OPOST  - Translates carriage returns to line feeds during output processing
   *
   * Less applicable to modern terminals but are technically part of "raw mode":
   *
   * BRKINT - Break conditions sent SIGINT
   * INPCK  - Parity checking...in a terminal...
   * ISTRIP - Strips 8th bit of input (probably already turned off)
   * CS8    - Sets character width to 8-bits (likely already on as well)
   */
  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  // Set our custom terminal config
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
      printf("%d\r\n", c);
    } else {
      printf("%d ('%c')\r\n", c, c);
    }
  }

  return 0;
}
