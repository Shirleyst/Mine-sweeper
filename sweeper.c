#include "minesweeper.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

struct ms_board {
  int width;
  int height;
  char *board;
  int num_mines;
  struct posn *mines;
};

const char UNREVEALED = ' ' ;
const char REVEALED[9] = "012345678";
const char FLAG = 'F';
const char MINE = '*';


bool flag(struct ms_board *b, int x, int y) {
  assert(b);
  assert((1 <= x) && (x <= b->width));
  assert((1 <= y) && (y <= b->height));
  char * board = b->board;
  int position = (y - 1) * b->width + x - 1;
  if (board[position] == FLAG) {
    board[position] = UNREVEALED;
  } else if(board[position] == UNREVEALED) {
    board[position] = FLAG;
  } else {
    return false;
  }
  return true;
}

static bool is_mine(const struct ms_board *b, int x, int y) {
  assert(b);
  struct posn * under = b->mines;
  int len = b->num_mines;
  for(int i = 0; i < len; i++) {
    if((under[i].x == x) && (under[i].y == y)) {
      return true;
    }
  }
  return false;
}

static int adj_mine(const struct ms_board *b, int x, int y) {
  assert(b);
  int count = 0;
  for(int x_off = -1; x_off < 2; x_off++) {
    for(int y_off = -1; y_off < 2; y_off++) {
      if ((is_mine(b, (x + x_off), (y + y_off)))) {
        count++;
      }
    }
  }
  return count;
}

// reveal adjacent tiles of (x,y)
static void rev_adj(struct ms_board *b, int x, int y) {
  assert(b);
  char * board = b->board;
  int w = b->width;
  int h = b->height;
  int c = 0;
  for(int x_off = -1; x_off < 2; x_off++) {
    for(int y_off = -1; y_off < 2; y_off++) {//adj
      if((x_off == 0) && (y_off == 0)) {
        continue;
      }
      int current = (y + y_off - 1) * w + x + x_off - 1;
      if((1 <= x + x_off) && (x + x_off <= w) 
         && (1 <= y + y_off) && (y + y_off <= h)
         && (board[current] == UNREVEALED)) {
        c = adj_mine(b, x + x_off, y + y_off);// reveal
        board[current] = REVEALED[c];
        if(c == 0) {
          rev_adj(b, x + x_off, y + y_off);
        }
      }
    }
  }
}

bool reveal(struct ms_board *b, int x, int y) {
  assert(b);
  assert((1 <= x) && (x <= b->width));
  assert((1 <= y) && (y <= b->height));
  char * board = b->board;
  int position = (y - 1) * b->width + x - 1;
  if (board[position] == UNREVEALED) {
    if (is_mine(b, x, y)) {// is MINE
      board[position] = MINE;
      return true;
    }
    // else consider adjacent
    int count = adj_mine(b, x, y);
    board[position] = REVEALED[count]; //(x,y) revealed
    if(count == 0) {
      rev_adj(b, x, y);
    }
  } else {
    return false;
  }
  return true;
}

bool game_won(const struct ms_board *b) {
  assert(b);
  int w = b->width;
  int h = b->height;
  int len = b->num_mines;
  int num_tiles = w * h;
  char *board = b->board;
  char tiles[num_tiles + 1];
  for(int k = 0; k < num_tiles; k++) {
    tiles[k] = board[k];
  }
  struct posn * under = b->mines;
  for(int j = 0; j < len; j++) {// for every mines
    int x = under[j].x;
    int y = under[j].y;
    char position = tiles[(y - 1) * w + x - 1];
    if(position == MINE) {
      return false;
    } else {
      tiles[(y - 1) * w + x - 1] = REVEALED[0];
    }
  }
  for(int i = 0; i < w * h; i++) {// for every tiles
    char p = tiles[i];
    if((p == MINE) || (p == UNREVEALED) || (p == FLAG)) {
      // no mine to be revealed
      return false;
    }
  }
  return true;
}

bool game_lost(const struct ms_board *b) {
  assert(b);
  int len = b->num_mines;
  struct posn * under = b->mines;
  for(int i = 0; i < len; i++) {
    int x = under[i].x;
    int y = under[i].y;
    char position = b->board[(y - 1) * b->width + x - 1];
    if(position == MINE) {
      return true;
    }
  }
  return false;
}
