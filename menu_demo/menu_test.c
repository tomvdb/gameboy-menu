// author: Tom Van den Bon
// date created: 2013/01/08

#include <gb/gb.h>
#include <stdio.h>
#include <gb/console.h>

#define RIGHT_ARROW_CHAR    3
#define LEFT_ARROW_CHAR     4
#define UNCHECKED_CHAR      5
#define SCREEN_CHAR         6
#define CHECK_CHAR          8
#define NOTE_CHAR           11
#define SPACE_CHAR          ' '

struct TextParams {
    char *name;
    UBYTE configuration;
    
    // bit 1 - value option - configValueIndex contains index to configValues array
    // bit 2 - not a menu item
    // bit 3 - open new screen - configValueIndex contains screen number
    // bit 4 - checkbox
    
    UBYTE configValueIndex;
};

const struct TextParams screen_0[] = {
    { "Basic Setup  ", 0x04, 0x01 },
    { "Complex Setup",  0x04, 0x02 },    
    { "" , 0x02, 0x00 }, 
    { "Option 1     ",  0x01, 0x00 },
    { "Option 2     ",  0x08, 0x00 },
    { "Option 3     ",  0x08, 0x00 },
    { "" , 0x02, 0x00 }, 
    { "" , 0x02, 0x00 }, 
    { "Take Action  ",  0x00, 0x00 },
    { NULL, 0x00, 0x00 }
};

const struct TextParams screen_1[] = {
    { "BOption 1    ",  0x00, 0x00 },
    { "BOption 2    ",  0x00, 0x00 },
    { "BOption 3    ",  0x00, 0x00 },
    { "BOption 4    ",  0x00, 0x00 },
    { NULL, 0x00, 0x00 }
};

const struct TextParams screen_2[] = {
    { "COption 1    ",  0x00, 0x00 },
    { "COption 2    ",  0x00, 0x00 },
    { "COption 3    ",  0x00, 0x00 },
    { "COption 4    ",  0x00, 0x00 },
    { NULL, 0x00, 0x00 }
};


WORD configValues[10];

const struct TextParams *screens_array[] = 
{
    screen_0,
    screen_1,
    screen_2
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
        
        
        // show screen option
        if ( currentScreenPointer[counter].configuration & 0x04 )
        {
            gotoxy( 16, counter+1 );
            setchar( SCREEN_CHAR );
        }
        
        // show checkbox
        if ( currentScreenPointer[counter].configuration & 0x08 )
        {
            gotoxy( 16, counter+1 );
            setchar( UNCHECKED_CHAR );
        }
        
    }
       
    gotoxy( 1, 1 );
    setchar( RIGHT_ARROW_CHAR );  

        
    
    return counter;
}

void main() 
{
    UBYTE currentScreen = 0;
    UBYTE previousScreen = 0;
    UBYTE cursorPosition = 1;
    UBYTE counter = 0;
    
    UBYTE chartemp = 1;
    UBYTE temp = 0;
    
    UBYTE input;
    
    previousScreen = 0;
    configValues[0] = 40;
    
    // setup and draw the inital screen
    currentScreenPointer = screens_array[currentScreen];
    counter = drawScreen();
    
    while (1)
    {
        input = joypad();        
        
        // current option selected "Enter Pressed"
        if ( input & J_A )
        {
            waitpadup();
                       
            if ( currentScreenPointer[cursorPosition-1].configuration & 0x04 )  // open new screen
            {
                // todo: check if the screen actually exists
                
                previousScreen = currentScreen;
                currentScreen = currentScreenPointer[cursorPosition-1].configValueIndex;
                currentScreenPointer = screens_array[currentScreen];
                counter = drawScreen();
                cursorPosition = 1;                
            }
        }
        
        // current option selected "Back Pressed"
        if ( input & J_B )
        {
            waitpadup();

            if ( currentScreen != previousScreen )
            {
                currentScreen = previousScreen;
                currentScreenPointer = screens_array[currentScreen];
                counter = drawScreen();
                cursorPosition = 1;                
            }
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
