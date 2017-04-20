
// posn is the block in board with coordinate(x,y) and can only cooresponding to one of:
//   UNREVEALED, REVEALED[0..8], FLAG or MINE
struct posn {
  int x;
  int y;
};

// board is the game board for mine sweeper start from(1,1) and has height weight > 0
struct ms_board;

// the followings are functions:
// flag(b, x, y) makes an UNREVEALED tile (x,y) flagged or vice versa, 
//   returns true if successful and false othersie(was already revealed).
// requires: *b is a valid
//           1 <= x <= b->width
//           1 <= y <= b->height
// effects: tile (x,y) of board b may change

bool flag(struct ms_board *b, int x, int y);

// reveal(b, x, y) reveals (x,y) and replaces it with either a MINE
//   or the number of mines adjacent to (x,y), returns true if successful and false if the tile (x,y) 
//   was already REVEALED, a MINE or a FLAG, if no minew is adjacented, it also reveals all unrevealed adjacent neighbours,
//   and (recursively) continues to do so until no more 0's are revealed.
// requires: *b is a valid
//           1 <= x <= b->width
//           1 <= y <= b->height
// effects: some tiles may be revealed

bool reveal(struct ms_board *b, int x, int y);

// game_won(b) returns true if game won(all safe tiles have been revealed), and false otherwise.
// requires: *b is a valid ms_board

bool game_won(const struct ms_board *b);

// game_lost(b) returns true if there are any MINEs on board b, and false otherwise.
// requires: *b is a valid ms_board

bool game_lost(const struct ms_board *b);
