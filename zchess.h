#ifndef  _ZCHESS_H_
#define  _ZCHESS_H_

#ifdef  __cplusplus
extern "C" {
#endif

#define EM   0x00    // 空白
#define BJ   0x11    // 黑帅
#define BC   0x12    // 黑车
#define BM   0x13    // 黑马
#define BX   0x14    // 黑象
#define BS   0x15    // 黑士
#define BP   0x16    // 黑炮
#define BB   0x17    // 黑兵
#define WJ   0x21    // 白将
#define WC   0x22    // 白车
#define WM   0x23    // 白马
#define WX   0x24    // 白相
#define WS   0x25    // 白仕
#define WP   0x26    // 白砲
#define WB   0x27    // 白卒

#define BOARD_SIZE   90

struct zchess;

enum zchess_player
{
    ZCHESS_PLAYER_INVALID   =  0,
    ZCHESS_PLAYER_WHITE     =  1,
    ZCHESS_PLAYER_BLACK     =  2
};

enum zchess_status
{
    ZCHESS_STATUS_INVALID   = -1,
    ZCHESS_STATUS_NONE      =  0,
    ZCHESS_STATUS_WHITE_WIN =  1,
    ZCHESS_STATUS_BLACK_WIN =  2,
    ZCHESS_STATUS_DRAW      =  3
};

struct zchess* zchess_init( enum zchess_player _player );
enum zchess_status zchess_play( struct zchess* _chess, int _sx, int _sy, int _dx, int _dy );
enum zchess_status zchess_status( const struct zchess *_chess );
enum zchess_player zchess_curr_player( const struct zchess *_chess );
void zchess_replay( struct zchess* _chess );
void zchess_set_player( struct zchess* _chess, enum zchess_player _player );
void zchess_board( const struct zchess *_chess, char _board[BOARD_SIZE] );
void zchess_free( struct zchess *_chess );

#ifdef  __cplusplus
}
#endif

#endif

