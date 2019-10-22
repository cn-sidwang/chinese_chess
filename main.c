#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zchess.h"

//  注意此颜色标识仅在Linux系统上有效, 其他系统请自行修改并进行颜色区分
const char*  red_fmt  = "\033[1;31m%s\033[0m ";
const char*  blue_fmt = "\033[1;37m%s\033[0m ";

void display_board( board )
    const char board[BOARD_SIZE];
{
    int  color;
    char word[16] = {0};
    for( int i=0; i<9; i++ )
    {
        printf("  %d",i );
    }
    printf("\n");

    for( int i=0; i<10; i++ )
    {
	printf("%d ", i);
        for( int j=0; j<9; j++ )
	{
            if( board[i*9+j]!=EM )
	    {
                switch( board[i*9+j] )
		{
                    case BJ: color=1; sprintf( word, "将" ); break;
	            case BC: color=1; sprintf( word, "车" ); break;
		    case BM: color=1; sprintf( word, "马" ); break;
		    case BX: color=1; sprintf( word, "象" ); break;
		    case BS: color=1; sprintf( word, "士" ); break;
		    case BP: color=1; sprintf( word, "炮" ); break;
		    case BB: color=1; sprintf( word, "兵" ); break;
                    case WJ: color=0; sprintf( word, "帅" ); break;
	            case WC: color=0; sprintf( word, "車" ); break;
		    case WM: color=0; sprintf( word, "馬" ); break;
		    case WX: color=0; sprintf( word, "相" ); break;
		    case WS: color=0; sprintf( word, "仕" ); break;
		    case WP: color=0; sprintf( word, "砲" ); break;
		    case WB: color=0; sprintf( word, "卒" ); break;
		}
		if( color )
		{
                    printf( red_fmt, word );
		}
		else
		{
		    printf( blue_fmt, word );
		}
	    }
	    else
	    {
                printf("   " );
	    }
	}
	printf("\n");
    }
}

int main( argc, argv )
    int argc;
    char **argv;
{
    int  sx, sy, dx, dy;
    FILE  *fp = NULL;
    char msg[128] = {0};
    char board[BOARD_SIZE];
    struct zchess *chess;
    chess = zchess_init( ZCHESS_PLAYER_WHITE );
    fp = fopen( "order.list", "r" );
    if( !fp )
    {
        printf("ERROR: open 'order.list' failed!\n");
	return -1;
    }

    while( !feof(fp) )
    {
        zchess_board( chess, board );
	display_board( board );
	do
	{
            memset( msg, 0, 128 );
	    fgets( msg, 128, fp );
	    if( strcmp( msg, "\n" )!=0 )
	    {
                break;
	    }
	}while( !feof(fp) );
	
	if( strcmp( msg, "END\n")==0 )
	{
            break;
	}

	fscanf( fp, "%d,%d", &sx, &sy );
	fscanf( fp, "%d,%d", &dx, &dy );
        
	printf("MSG: %s, sx->%d, sy->%d, dx->%d, dy->%d      ", msg, sx, sy, dx, dy );
	if( zchess_play( chess, sx, sy, dx, dy )!=ZCHESS_STATUS_NONE )
	{
            printf("[failed]\n");
	    break;
	}
	else
	{
            printf("[successed]\n");
	}
    }

    fclose( fp );
    zchess_free( chess );
    return 0;
}

