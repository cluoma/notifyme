#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

int main(int argc, const char * argv[])
{
    int playchan;
    struct stat s;
    Mix_Chunk *n;

    if( argc != 2 )
    {
        fprintf(stderr, "Missing argument.\n");
        return 1;
    }

    if (stat(argv[1], &s) == -1 || !S_ISREG(s.st_mode) || S_ISDIR(s.st_mode))
    {
        fprintf(stderr, "Couldn't open file.\n");
        return 1;
    }

    if( SDL_Init( SDL_INIT_AUDIO ) == -1 )
    {
        fprintf(stderr, "Couldn't initialize SDL.\n");
        return 1;
    }

    if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY,
                       MIX_DEFAULT_FORMAT,
                       2,
                       1024 ) == -1 )
    {
        fprintf(stderr, "Couldn't initialize SDL_mixer.\n");
        SDL_Quit();
        return 1;
    }

    n = Mix_LoadWAV( argv[1] );
    if( !n )
    {
        fprintf(stderr, "Incompatible file type (need .wav).\n");
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    playchan = Mix_PlayChannel( -1, n, 0 );
    if( playchan == -1 )
    {
        fprintf(stderr, "Couldn't open channel.\n");
        Mix_FreeChunk( n );
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }

    while( Mix_Playing( -1 ) > 0 )
    {
        SDL_Delay(100);
    }

    Mix_FreeChunk( n );
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}