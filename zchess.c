#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zchess.h"

#define  JIANG   (0xCF & BJ)
#define  CHE     (0xCF & BC)
#define  MA      (0xCF & BM)
#define  XIANG   (0xCF & BX)
#define  SHI     (0xCF & BS)
#define  PAO     (0xCF & BP)
#define  BING    (0xCF & BB)

#define  SUB_ABS(n1,n2)    ((n1)>(n2)? (n1)-(n2) : (n2)-(n1))

const char BOARD[BOARD_SIZE] = \
{
	WC,  WM,  WX,  WS,  WJ,  WS,  WX,  WM,  WC,
	EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,
	EM,  WP,  EM,  EM,  EM,  EM,  EM,  WP,  EM,
	WB,  EM,  WB,  EM,  WB,  EM,  WB , EM,  WB,
	EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,
	EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,
	BB,  EM,  BB,  EM,  BB,  EM,  BB,  EM,  BB,
	EM,  BP,  EM,  EM,  EM,  EM,  EM,  BP,  EM,
	EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,  EM,
	BC,  BM,  BX,  BS,  BJ,  BS,  BX,  BM,  BC
};

struct zchess
{
	char  board[BOARD_SIZE];
	enum  zchess_player  curr_player;
	enum  zchess_player  init_player;
	enum  zchess_status  curr_status;
};

static enum zchess_player _player( _c )
	int _c;
{
	if( _c >= BJ && _c <= BB )
	{
		return ZCHESS_PLAYER_BLACK;
	}

	if( _c >= WJ && _c <= WB )
	{
		return ZCHESS_PLAYER_WHITE;
	}

	return ZCHESS_PLAYER_INVALID;
}

static int _count_path_chess( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	int count = 0;
	int xd = _sx>_dx? -1 : _sx==_dx? 0 : 1;
	int yd = _sy>_dy? -1 : _sy==_dy? 0 : 1;
	for( _sx+=xd, _sy+=yd; _sx!=_dx || _sy!=_dy; _sx+=xd, _sy+=yd )
	{
		if( _chess->board[ _sy*9+_sx ] != EM )
		{
			++count;
		}
	}
	return count;
}

static enum zchess_status  _play_jiang( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	int xd = SUB_ABS(_sx, _dx);
	int yd = SUB_ABS(_sy, _dy);
	if( (xd==1 && yd==0) || (xd==0&&yd==1) )
	{
		int offset = 0;
		offset = _player( _chess->board[_sy*9+_sx] )==_chess->init_player? 7 : 0;
		if( _dx>=3 && _dx<=5 && _dy>=(offset) && _dy<=(offset+2) )
		{
			return ZCHESS_STATUS_NONE;
		}
	}
	else if( (0xCF & _chess->board[_dy*9+_dx]) == JIANG && _sx==_dx )
	{
                if( _count_path_chess( _chess, _sx, _sy, _dx, _dy ) == 0 )
		{
                        return ZCHESS_STATUS_NONE;
		}
	}
	return ZCHESS_STATUS_INVALID;
}

static enum zchess_status  _play_che( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	if( _sx==_dx || _sy==_dy )
	{
		if( _count_path_chess( _chess, _sx, _sy, _dx, _dy )==0 )
		{
			return ZCHESS_STATUS_NONE;
		}
	}
	return ZCHESS_STATUS_INVALID;
}

static enum zchess_status  _play_ma( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	int xd = SUB_ABS( _sx, _dx );
	int yd = SUB_ABS( _sy, _dy );
	if( xd==1? yd==2? 1 : 0 : xd==2? yd==1? 1 : 0 : 0  )
	{
		xd = xd==2? _sx>_dx? -1 : 1 : 0;
		yd = yd==2? _sy>_dy? -1 : 1 : 0;
		if( _chess->board[ (_sy+yd)*9+(_sx+xd) ]==EM )
		{
			return ZCHESS_STATUS_NONE;
		}
	}
	return ZCHESS_STATUS_INVALID;
}

static enum zchess_status  _play_xiang( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	int xd = SUB_ABS( _sx, _dx );
	int yd = SUB_ABS( _sy, _dy );
	if( xd==yd && xd==2 )
	{
		xd = _sx>_dx? -1 : 1;
		yd = _sy>_dy? -1 : 1;
		if( _chess->board[ (_sy+yd)*9+(_sx+xd) ]==EM )
		{
			yd = _player( _chess->board[_sy*9+_sx] ) == _chess->init_player? -1 : 1;
			if( yd==-1? _dy>=5 : _dy<5 )
			{
				return ZCHESS_STATUS_NONE;
			}
		}
	}
	return ZCHESS_STATUS_INVALID;
}

static enum zchess_status  _play_shi( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	int xd = SUB_ABS( _sx, _dx );
	int yd = SUB_ABS( _sy, _dy );
	if( xd==yd && xd==1 )
	{
		int offset = _player( _chess->board[_sy*9+_sx] )==_chess->init_player? 7 : 0;
		if( _dx>=3 && _dx<=5 && _dy>=offset && _dy<=(offset+2) )
		{
			return ZCHESS_STATUS_NONE;
		}
	}
	return ZCHESS_STATUS_INVALID;
}

static enum zchess_status  _play_pao( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	if( _sx==_dx || _sy==_dy )
	{
		int count = _count_path_chess( _chess, _sx, _sy, _dx, _dy );
		if( _chess->board[ _dy*9+_dx ]!=EM && count==1 )
		{
			return ZCHESS_STATUS_NONE;
		}
		
		if( _chess->board[ _dy*9+_dx ]==EM && count==0 )
		{
                        return ZCHESS_STATUS_NONE;
		}
	}
	return ZCHESS_STATUS_INVALID;
}

static enum zchess_status  _play_bing( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	int xd = SUB_ABS( _sx, _dx );
	int yd = SUB_ABS( _sy, _dy );
	if( ( xd==1 && yd==0 ) || (xd==0&&yd==1) )
	{
		if( _sx != _dx )
		{
                	if( _player( _chess->board[_sy*9+_sx] ) == _chess->init_player && _sy<5 )
			{
				return ZCHESS_STATUS_NONE;
			}

			if( _player( _chess->board[_sy*9+_sx] ) != _chess->init_player && _sy>=5 )
			{
				return ZCHESS_STATUS_NONE;
			}
		}
		else
		{
			if( _player(_chess->board[_sy*9+_sx]) == _chess->init_player && _sy>_dy )
			{
				return ZCHESS_STATUS_NONE;
			}
			
			if( _player(_chess->board[_sy*9+_sx]) != _chess->init_player && _sy<_dy )
			{
				return ZCHESS_STATUS_NONE;
			}
		}
	}
	       	
	return ZCHESS_STATUS_INVALID;
}

static int _check_jiang( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	enum zchess_player  player;
	int  jx = -1, jy = -1, ret = 0;
	int  tmp = _chess->board[ _dy*9+_dx ];

	_chess->board[ _dy*9+_dx ] = _chess->board[ _sy*9+_sx ];
	_chess->board[ _sy*9+_sx ] = EM;
	player = _player( _chess->board[ _dy*9+_dx ] );

	for( int i=0; i<10; i++ )
	{
		for( int j=0; j<9; j++ )
		{
			if( (0xCF & _chess->board[ i*9+j ]) == JIANG )
			{
				if( _player( _chess->board[ i*9+j ] ) == player )
				{
					jx = j;  jy = i;
					break;
				}
			}
		}
	}

	if( jx!=-1 && jy!=-1 )
	{
		enum zchess_status  status;
		player = player==ZCHESS_PLAYER_WHITE? ZCHESS_PLAYER_BLACK : ZCHESS_PLAYER_WHITE;
		for( int i=0; i<10; i++ )
		{
			for( int j=0; j<9; j++ )
			{
				if( player == _player(_chess->board[ i*9+j ]) )
				{
					switch( 0xCF & _chess->board[i*9+j] )
					{
						case JIANG: status = _play_jiang( _chess, j, i, jx, jy );  break;
						case CHE:   status = _play_che( _chess, j, i, jx, jy );    break;
						case MA:    status = _play_ma( _chess, j, i, jx, jy );     break;
						case XIANG: status = _play_xiang( _chess, j, i, jx, jy );  break;
						case SHI:   status = _play_shi( _chess, j, i, jx, jy );    break;
						case PAO:   status = _play_pao( _chess, j, i, jx, jy );    break;
						case BING:  status = _play_bing( _chess, j, i, jx, jy );   break;
					}

					if( status==ZCHESS_STATUS_NONE )
					{
						ret = -1;
						break;
					}
				}
			}
		}
	}
	else
	{
		ret = -1;
	}

	_chess->board[ _sy*9+_sx ] = _chess->board[ _dy*9+_dx ];
	_chess->board[ _dy*9+_dx ] = tmp;
	return ret;
}

static void _init_chess_board( _board, _player )
	char _board[BOARD_SIZE];
	enum zchess_player _player;
{
	if( _player == ZCHESS_PLAYER_BLACK )
	{
		memcpy( _board, BOARD, sizeof(char)*BOARD_SIZE );
	}
	else
	{
		for( int i=0; i<BOARD_SIZE; i++ )
		{
			_board[i] = BOARD[BOARD_SIZE-i-1];
		}
	}
}

struct zchess* zchess_init( _player )
	enum zchess_player _player;
{
	struct zchess *_chess = NULL;
	if( _player == ZCHESS_PLAYER_INVALID )
	{
		return NULL;
	}
	_chess = (struct zchess*)calloc(sizeof(struct zchess),1);
	_chess->curr_player = ZCHESS_PLAYER_BLACK;
	_chess->init_player = _player;
	_init_chess_board( _chess->board, _player );
	return _chess;
}

enum zchess_status zchess_status( _chess )
	const struct zchess *_chess;
{
	return _chess!=NULL? _chess->curr_status : ZCHESS_STATUS_INVALID;
}

enum zchess_player zchess_curr_player( _chess )
	const struct zchess *_chess;
{
	return _chess!=NULL? _chess->curr_player : ZCHESS_PLAYER_INVALID;
}

enum zchess_status zchess_play( _chess, _sx, _sy, _dx, _dy )
	struct zchess* _chess;
	int _sx;
	int _sy;
	int _dx;
	int _dy;
{
	enum zchess_status  status = ZCHESS_STATUS_INVALID;
	if( _chess->curr_status != ZCHESS_STATUS_NONE )
	{
		return _chess->curr_status;
	}

	if( _chess!=NULL && _sx>=0 && _sx<=9 && _dx>=0 && _dy<=10 )
	{
		if( _player( _chess->board[_sy*9+_sx ] ) != _chess->curr_player )
		{
			return ZCHESS_STATUS_INVALID;
		}

		if( _player( _chess->board[_dy*9+_dx] ) == _chess->curr_player )
		{
			return ZCHESS_STATUS_INVALID;
		}

		switch( 0xCF & _chess->board[_sy*9+_sx] )
		{
			case JIANG:  status = _play_jiang( _chess, _sx, _sy, _dx, _dy );  break;
			case CHE:    status = _play_che( _chess, _sx, _sy, _dx, _dy );    break;
			case MA:     status = _play_ma( _chess, _sx, _sy, _dx, _dy );     break;
			case XIANG:  status = _play_xiang( _chess, _sx, _sy, _dx, _dy );  break;
			case SHI:    status = _play_shi( _chess, _sx, _sy, _dx, _dy );    break;
			case PAO:    status = _play_pao( _chess, _sx, _sy, _dx, _dy );    break;
			case BING:   status = _play_bing( _chess, _sx, _sy, _dx, _dy );   break;
		}

		if( status == ZCHESS_STATUS_NONE )
		{
			if( _check_jiang( _chess, _sx, _sy, _dx, _dy )==0 )
			{
				_chess->board[_dy*9+_dx] = _chess->board[_sy*9+_sx];
				_chess->board[_sy*9+_sx] = EM;
				_chess->curr_player = _chess->curr_player == ZCHESS_PLAYER_WHITE? 
					                    ZCHESS_PLAYER_BLACK : ZCHESS_PLAYER_WHITE;
			}
			else
			{
				status = ZCHESS_STATUS_INVALID;
			}
		}    
	}
	return status;
}

void zchess_replay( _chess )
	struct zchess *_chess;
{
	if( _chess )
	{
		_chess->curr_player = ZCHESS_PLAYER_BLACK;
		_chess->curr_status = ZCHESS_STATUS_NONE;
		_init_chess_board( _chess->board, _chess->init_player );
	}
}

void zchess_set_player( _chess, _player )
	struct zchess *_chess;
	enum zchess_player _player;
{
	if( _chess )
	{
		_chess->init_player = _player;
		zchess_replay( _chess );
	}
}

void zchess_board( _chess, _board )
	const struct zchess *_chess;
	char _board[BOARD_SIZE];
{
	if( _chess!=NULL )
	{
		for( int i=0; i<BOARD_SIZE; i++ )
		{
			_board[i] = _chess->board[i];
		}
	}
}

void zchess_free( _chess )
	struct zchess *_chess;
{
	if( _chess!=NULL )
	{
		free( _chess );
	}
}

