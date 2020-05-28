/*
 *  E
 */
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#define CTRL_KEY(k) ((k) & 0x1f)

/* Global editor state */
struct estate {
  int rows;
  int cols;
  struct termios orig_termios;
};

struct estate E;

struct abuf {
  char *b;
  int len;
};

#define ABUF_INIT {NULL, 0}

void abappend(struct abuf *ab, const char *s, int len) {
  char *new = realloc(ab->b, ab->len + len);

  if (new == NULL) return;
  memcpy(&new[ab->len], s, len);
  ab->b = new;
  ab->len += len;
}

void abfree(struct abuf *ab) {
  free(ab->b);
}

void refresh();

void die(const char *s) {
  perror(s);
  exit(1);
}

int get_window_size(int *rows, int *cols) {
  struct winsize ws;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *cols = ws.ws_col;
    *rows = ws.ws_row;
    return 0;
  }
}

void teardown() {
  if(tcsetattr(STDIN_FILENO, TCSAFLUSH,  &E.orig_termios) == -1)
    die("tcsetattr");
}

// Enable raw mode and other convinient terminal settings
// runs teardown() using atexit()
void setup() {
  // Store original terminal config
  if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) die("tcgetattr");
  write(STDOUT_FILENO, "\x1b[2J", 4);

  // Schedule resetting to original config at exit
  atexit(teardown);

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
  struct termios raw = E.orig_termios;
  raw.c_cflag |= (CS8);
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_oflag &= ~(OPOST);

  // Set our custom terminal config
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcgetattr");

  if (get_window_size(&E.rows, &E.cols) == -1) die("get_window_size");
}


char read_key() {
  char c = '\0';
  if (read(STDIN_FILENO, &c, 1) == -1) die("read");
  return c;
}

void process_keys() {
  char c = read_key();

  switch (c) {
    case CTRL_KEY('q'):
      exit(0);
      break;
  }
}

void draw_rows(struct abuf *ab) {
  for (int y = 0; y < E.rows; y++) {
    abappend(ab, "~", 1);

    abappend(ab, "\x1b[k", 3);
    if(y < (E.rows - 1)) {
      abappend(ab, "\r\n", 2);
    }
  }
}

void refresh() {
  struct abuf ab = ABUF_INIT;

  abappend(&ab, "\x1b[?25l", 6);
  abappend(&ab, "\x1b[H", 3);

  draw_rows(&ab);

  abappend(&ab, "\x1b[H", 3);
  abappend(&ab, "\x1b[?25h", 6);

  write(STDOUT_FILENO, ab.b, ab.len);
  abfree(&ab);
}

int main(int argc, char *argv[])
{
  setup();

  while (1) {
    refresh();
    process_keys();
  }

  return 0;
}
