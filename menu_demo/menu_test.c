// author: Tom Van den Bon
// date created: 2013/01/08

#include <gb/gb.h>
#include <stdio.h>
#include <gb/console.h>

#define RIGHT_ARROW_CHAR 3
#define LEFT_ARROW_CHAR 4
#define SPACE_CHAR ' '

struct TextParams {
    char *name;
    UBYTE configuration;
    UBYTE configValueIndex;
    
    // bit 1 - value option
};

const struct TextParams screen_0[] = {
    { "Basic Setup  ", 0x00, 0x00 },
    { "Complex Setup",  0x00, 0x00 },    
    { "" , 0x02, 0x00 }, 
    { "Take Action  ",  0x00, 0x00 },
    { "Option 1     ",  0x01, 0x00 },
    { "Option 2     ",  0x00, 0x00 },
    { "Option 3     ",  0x00, 0x00 },
    { NULL, 0x00, 0x00 }
};

const struct TextParams screen_1[] = {
    { "Go Here     ",  0x00, 0x00 },
    { "Go There    ",  0x00, 0x00 },
    { NULL, 0x00, 0x00 }
};

WORD configValues[10];

const struct TextParams *screens_array[] = 
{
    screen_0,
    screen_1
};

struct TextParams *currentScreenPointer;

void cls()
{
    // dirty cls
    int c = 0;
    for ( c = 0; c < 18; c++ )
    {
        gotoxy( 1, c );
        printf( "                    ");
    }
    gotoxy(1, 1);
}

UBYTE drawScreen()
{
    UBYTE counter = 0;
    WORD dt = 0;
    
    cls();
    
    for ( counter = 0; currentScreenPointer[counter].name; counter++ )
    {
        gotoxy(2, counter+1);
        if ( currentScreenPointer[counter].configuration & 0x01 )
            printf( "%s %d", currentScreenPointer[counter].name, configValues[currentScreenPointer[counter].configValueIndex] );
        else
            printf( "%s", currentScreenPointer[counter].name );
        
    }
    
    gotoxy( 1, 1 );
    setchar( RIGHT_ARROW_CHAR );    
    
    return counter;
}

void main() 
{
    UBYTE currentScreen = 0;
    UBYTE cursorPosition = 1;
    UBYTE counter = 0;
    
    UBYTE chartemp = 1;
    UBYTE temp = 0;
    
    UBYTE input;

    configValues[0] = 40;
    
    // setup and draw the inital screen
    currentScreenPointer = screens_array[currentScreen];
    counter = drawScreen();
    
    while (1)
    {
        input = joypad();        
        
        // temporary test to flip between screens
        if ( input & J_START )
        {
            waitpadup();
            
            if ( currentScreen == 1 )
                currentScreen = 0;
            else
                currentScreen = 1;
            
            currentScreenPointer = screens_array[currentScreen];
            counter = drawScreen();
            cursorPosition = 1;
        }
        
        if ( input & J_LEFT )
        {
            waitpadup();
            // is this option a value option ?
            if ( currentScreenPointer[cursorPosition-1].configuration & 0x01 )
            {
                // todo: check limits
                configValues[currentScreenPointer[cursorPosition-1].configValueIndex]--;
                gotoxy(2, cursorPosition);
                printf( "%s %d", currentScreenPointer[cursorPosition-1].name, configValues[currentScreenPointer[cursorPosition-1].configValueIndex] );
            }
        }
        
        if ( input & J_RIGHT )
        {
            waitpadup();
            // is this option a value option ?
            if ( currentScreenPointer[cursorPosition-1].configuration & 0x01 )
            {
                // todo: check limits
                configValues[currentScreenPointer[cursorPosition-1].configValueIndex]++;
                gotoxy(2, cursorPosition);
                printf( "%s %d", currentScreenPointer[cursorPosition-1].name, configValues[currentScreenPointer[cursorPosition-1].configValueIndex] );
            }
        }
        
        
        if ( input & J_DOWN )
        {
            waitpadup();
            gotoxy( 1, cursorPosition); setchar( SPACE_CHAR );
            gotoxy( 0, cursorPosition); setchar( SPACE_CHAR );
            cursorPosition = cursorPosition + 1;
            
            if ( cursorPosition > counter )
                cursorPosition = counter;
            
            if ( currentScreenPointer[cursorPosition-1].configuration & 0x02 )
                cursorPosition++;           
            
            if ( currentScreenPointer[cursorPosition-1].configuration & 0x01 )
            {
                gotoxy( 0, cursorPosition); setchar( LEFT_ARROW_CHAR );            
                gotoxy( 1, cursorPosition); setchar( RIGHT_ARROW_CHAR );            
            }
            else
            {
                gotoxy( 1, cursorPosition); setchar( RIGHT_ARROW_CHAR );            
            }
        }
        
        if ( input & J_UP )
        {
            waitpadup();
            gotoxy( 1, cursorPosition); setchar( SPACE_CHAR );
            gotoxy( 0, cursorPosition); setchar( SPACE_CHAR );
            cursorPosition = cursorPosition - 1;
            
            if ( cursorPosition < 1 )
                cursorPosition = 1;
            
            // not selectable
            if ( currentScreenPointer[cursorPosition-1].configuration & 0x02 )
                cursorPosition--;
            
            if ( currentScreenPointer[cursorPosition-1].configuration & 0x01 )
            {
                gotoxy( 0, cursorPosition); setchar( LEFT_ARROW_CHAR );            
                gotoxy( 1, cursorPosition); setchar( RIGHT_ARROW_CHAR );            
            }
            else
            {
                gotoxy( 1, cursorPosition); setchar( RIGHT_ARROW_CHAR );            
            }
        }
        
    }        

}
