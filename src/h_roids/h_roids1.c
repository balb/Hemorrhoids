#include <graph.h>
#include "graph3.h"
#include "graph4.h"
#include "graph5.h"
#include "graph6.h"
#include "graph11.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> 
#include <malloc.h> 

//DEFINES////////////////////////////////////////////////////////////////////

#define SHIP_FRAMES     24
#define TOP_SPEED       8
#define PI              (float)3.14159 
#define MAX_BULLETS     6      

//TYPE DEFS//////////////////////////////////////////////////////////////////
typedef struct bullet_typ
    {
    int x,y;
    int direction; 
    int active;
    }bullet,*bullet_ptr; 
    
//GLOBALS////////////////////////////////////////////////////////////////////     

char numbers[10][36],letters[27][36];
int x_trig[24][TOP_SPEED],y_trig[24][TOP_SPEED];                                         
long long_index;

int i,n,frame=0;
int done=0,moved=0;

int speed_inc=0,velocity=0,direction=0,ship_x,ship_y,worm_inc=1;  

int num_bullets=0,next_bullet=0,temp_x,temp_y,flag=0,count=0,rand24=12;

int stars1[40][2],stars2[30][2],stars3[20][2]; 

int count_ufo=0,rand2=0;
sprite ship_sprite,ufo_sprite;
pcx_picture ship_pcx,background_pcx,ufo_pcx,alphabet_pcx;

RGB_color creg1,creg2,creg3; 

bullet missile[MAX_BULLETS];

//INITIALIZATION FUNCTIONS///////////////////////////////////////////////////
make_trig_table()
{
int index,index2;
float temp_x,temp_y;
for(index=0;index<24;index++)
    {
    for(index2=0;index2<(TOP_SPEED);index2++)
        {           
        temp_x=sin(PI*(index*15)/180);
        x_trig[index][index2]=index2*temp_x;
        temp_y=cos(PI*(index*15)/180);
        y_trig[index][index2]=-index2*temp_y;
        }
    }
return 0;
} 

init_stars()
{
for(i=0;i<40;i++)
    {
    stars1[i][0]=rand()%250;
    stars1[i][1]=rand()%200;
    }
for(i=0;i<30;i++)
    {
    stars2[i][0]=rand()%250;
    stars2[i][1]=rand()%200;
    }
for(i=0;i<20;i++)
    {
    stars3[i][0]=rand()%250;
    stars3[i][1]=rand()%200;
    }        
}    
    
init_bullets()
{
for(i=0;i<MAX_BULLETS;i++)
missile[i].active=0; 
return 0;
}  

init_colour_regs()
{
creg1.red=0;
creg1.green=0;
creg1.blue=0;
creg2.red=0;
creg2.green=0;
creg2.blue=0;
creg3.red=0;
creg3.green=0;
creg3.blue=0;
Set_Palette_Register(1,(RGB_color_ptr)&creg1); 
Set_Palette_Register(2,(RGB_color_ptr)&creg2);
Set_Palette_Register(3,(RGB_color_ptr)&creg3);  
return 0;
}
  
create_double_buffer()
{  
if(!Create_Double_Buffer(SCREEN_HEIGHT));
printf("You smell of poo\n and you do not have enough memory to \ncreate a double buffer!!!!");
Fill_Double_Buffer(0);
return 0;
}      

load_ship_sprite()
{ 
//load ship sprite
PCX_Init((pcx_picture_ptr)&ship_pcx);
PCX_Load("the_ship.pcx",(pcx_picture_ptr)&ship_pcx,0); 

sprite_width=15;
sprite_height=15; 

Sprite_Init((sprite_ptr)&ship_sprite,117,92,0,0,0,0);
ship_x=ship_sprite.x;
ship_y=ship_sprite.y;

//grab the ships
for(i=0;i<12;i++)//row one
    {
    PCX_Grab_Bitmap((pcx_picture_ptr)&ship_pcx,
            (sprite_ptr)&ship_sprite,i,i,0);
    }
for(i=0;i<12;i++)//row two
    {
    PCX_Grab_Bitmap((pcx_picture_ptr)&ship_pcx,
            (sprite_ptr)&ship_sprite,i+12,i,1);
    }

PCX_Delete((pcx_picture_ptr)&ship_pcx);

Behind_Sprite_DB((sprite_ptr)&ship_sprite);
ship_sprite.state=1;
return 0;
}

load_ufo_sprite()
{ 
//load ufo sprite
PCX_Init((pcx_picture_ptr)&ufo_pcx);
PCX_Load("ufo.pcx",(pcx_picture_ptr)&ufo_pcx,0); 

sprite_width=31;
sprite_height=31; 

Sprite_Init((sprite_ptr)&ufo_sprite,50,50,0,0,0,0);

//grab the ufo's
for(i=0;i<9;i++)//row one
    {
    PCX_Grab_Bitmap((pcx_picture_ptr)&ufo_pcx,
            (sprite_ptr)&ufo_sprite,i,i,0);
    }
//grab the ufo's
for(i=0;i<8;i++)//row one
    {
    PCX_Grab_Bitmap((pcx_picture_ptr)&ufo_pcx,
            (sprite_ptr)&ufo_sprite,i+9,i,1);
    }    

PCX_Delete((pcx_picture_ptr)&ufo_pcx);

Behind_Sprite_DB((sprite_ptr)&ufo_sprite);
ufo_sprite.state=1;
return 0;
}

load_background()
{ 
char far *data;    
PCX_Init((pcx_picture_ptr)&background_pcx);
PCX_Load("mscreen.pcx",(pcx_picture_ptr)&background_pcx,0); 
data=background_pcx.buffer;
_asm
    {
    push ds;
    les di, double_buffer;
    lds si, data;
    mov cx,32000;
    cld;
    rep movsw;
    pop ds;
    }    
PCX_Delete((pcx_picture_ptr)&background_pcx);
return 0;
}    
  
grab_numbers()
{    
int counter=0,col,row,offset=2561;
for(i=0;i<10;i++)
    {
    for(row=0;row<6;row++)
        {   
        for(col=0;col<6;col++)
            {
            numbers[i][counter]=double_buffer[offset+col+(row*320)];
            counter++;
            }
        }
    offset+=7;
    counter=0;
    }    
return 0;
}  

grab_letters()
{    
int counter=0,col,row,offset=321;
for(i=0;i<27;i++)
    {
    for(row=0;row<6;row++)
        {   
        for(col=0;col<6;col++)
            {
            letters[i][counter]=double_buffer[offset+col+(row*320)];
            counter++;
            }
        }
    offset+=7;
    counter=0;
    }    
return 0;
}              

load_alphabet()
{ 
char far *data;    
PCX_Init((pcx_picture_ptr)&alphabet_pcx);
PCX_Load("alphabet.pcx",(pcx_picture_ptr)&alphabet_pcx,0); 
data=alphabet_pcx.buffer;
_asm
    {
    push ds;
    les di, double_buffer;
    lds si, data;
    mov cx,32000;
    cld;
    rep movsw;
    pop ds;
    }
grab_numbers();
grab_letters();    
PCX_Delete((pcx_picture_ptr)&alphabet_pcx);
return 0;
}
       

//ERASE FUNCTIONS////////////////////////////////////////////////////////////
erase_ship()
{                                         
sprite_width=15;
sprite_height=15;
Erase_Sprite_DB((sprite_ptr)&ship_sprite);
} 

erase_ufo()
{                                         
sprite_width=31;
sprite_height=31;
Erase_Sprite_DB((sprite_ptr)&ufo_sprite);
}

erase_bullets()
{
for(i=0;i<MAX_BULLETS;i++)
if(missile[i].active)
    double_buffer[((missile[i].y<<8)+(missile[i].y<<6))+missile[i].x]=0;
} 

erase_stars()
{
for(i=0;i<40;i+=5)
    {
    double_buffer[(stars1[i][1]<<8)+(stars1[i][1]<<6)+stars1[i][0]]=0;
    double_buffer[(stars1[i+1][1]<<8)+(stars1[i+1][1]<<6)+stars1[i+1][0]]=0;
    double_buffer[(stars1[i+2][1]<<8)+(stars1[i+2][1]<<6)+stars1[i+2][0]]=0;
    double_buffer[(stars1[i+3][1]<<8)+(stars1[i+3][1]<<6)+stars1[i+3][0]]=0;
    double_buffer[(stars1[i+4][1]<<8)+(stars1[i+4][1]<<6)+stars1[i+4][0]]=0;
    }
for(i=0;i<30;i+=5)
    {
    double_buffer[(stars2[i][1]<<8)+(stars2[i][1]<<6)+stars2[i][0]]=0;
    double_buffer[(stars2[i+1][1]<<8)+(stars2[i+1][1]<<6)+stars2[i+1][0]]=0;
    double_buffer[(stars2[i+2][1]<<8)+(stars2[i+2][1]<<6)+stars2[i+2][0]]=0;
    double_buffer[(stars2[i+3][1]<<8)+(stars2[i+3][1]<<6)+stars2[i+3][0]]=0;
    double_buffer[(stars2[i+4][1]<<8)+(stars2[i+4][1]<<6)+stars2[i+4][0]]=0;
    } 
for(i=0;i<20;i+=5)
    {
    double_buffer[(stars3[i][1]<<8)+(stars3[i][1]<<6)+stars3[i][0]]=0;
    double_buffer[(stars3[i+1][1]<<8)+(stars3[i+1][1]<<6)+stars3[i+1][0]]=0;
    double_buffer[(stars3[i+2][1]<<8)+(stars3[i+2][1]<<6)+stars3[i+2][0]]=0;
    double_buffer[(stars3[i+3][1]<<8)+(stars3[i+3][1]<<6)+stars3[i+3][0]]=0;
    double_buffer[(stars3[i+4][1]<<8)+(stars3[i+4][1]<<6)+stars3[i+4][0]]=0;
    }
}                  

//MOVE FUNCTIONS/////////////////////////////////////////////////////////////
thrust()
{
moved=1;
if(velocity<5)velocity++;
direction=frame; 
creg1.red=rand()%64;
creg1.green=40;
creg1.blue=20;
creg2.red=rand()%64; 
Set_Palette_Register(1,(RGB_color_ptr)&creg1); 
Set_Palette_Register(2,(RGB_color_ptr)&creg2);
}    

new_bullet()
{   
missile[next_bullet].x=ship_x+7;
missile[next_bullet].y=ship_y+8;
missile[next_bullet].direction=frame;
missile[next_bullet].active=1;
next_bullet++;
if(next_bullet==MAX_BULLETS)next_bullet=0;
num_bullets++;
count=4; 
}               

move_stars()
{   
int velocity2=0,velocity3=0;
if(velocity>1)velocity2=velocity-1;
if(velocity>2)velocity3=velocity-2; 


for(i=0;i<40;i+=4)
    {
    stars1[i][0]-=x_trig[direction][velocity];
    stars1[i][1]-=y_trig[direction][velocity];
    if(stars1[i][0]<0)stars1[i][0]+=249;
    else if(stars1[i][0]>249)stars1[i][0]-=249;
    if(stars1[i][1]<0)stars1[i][1]+=199;
    else if(stars1[i][1]>199)stars1[i][1]-=199;
    
    stars1[i+1][0]-=x_trig[direction][velocity];
    stars1[i+1][1]-=y_trig[direction][velocity];
    if(stars1[i+1][0]<0)stars1[i+1][0]+=249;
    else if(stars1[i+1][0]>249)stars1[i+1][0]-=249;
    if(stars1[i+1][1]<0)stars1[i+1][1]+=199;
    else if(stars1[i+1][1]>199)stars1[i+1][1]-=199;
    
    stars1[i+2][0]-=x_trig[direction][velocity];
    stars1[i+2][1]-=y_trig[direction][velocity];
    if(stars1[i+2][0]<0)stars1[i+2][0]+=249;
    else if(stars1[i+2][0]>249)stars1[i+2][0]-=249;
    if(stars1[i+2][1]<0)stars1[i+2][1]+=199;
    else if(stars1[i+2][1]>199)stars1[i+2][1]-=199;
    
    stars1[i+3][0]-=x_trig[direction][velocity];
    stars1[i+3][1]-=y_trig[direction][velocity];
    if(stars1[i+3][0]<0)stars1[i+3][0]+=249;
    else if(stars1[i+3][0]>249)stars1[i+3][0]-=249;
    if(stars1[i+3][1]<0)stars1[i+3][1]+=199;
    else if(stars1[i+3][1]>199)stars1[i+3][1]-=199;
    }
for(i=0;i<30;i+=3)
    {
    stars2[i][0]-=x_trig[direction][velocity2];
    stars2[i][1]-=y_trig[direction][velocity2];
    if(stars2[i][0]<0)stars2[i][0]+=249;
    else if(stars2[i][0]>249)stars2[i][0]-=249;
    if(stars2[i][1]<0)stars2[i][1]+=199;
    else if(stars2[i][1]>199)stars2[i][1]-=199;
    
    stars2[i+1][0]-=x_trig[direction][velocity2];
    stars2[i+1][1]-=y_trig[direction][velocity2];
    if(stars2[i+1][0]<0)stars2[i+1][0]+=249;
    else if(stars2[i+1][0]>249)stars2[i+1][0]-=249;
    if(stars2[i+1][1]<0)stars2[i+1][1]+=199;
    else if(stars2[i+1][1]>199)stars2[i+1][1]-=199;
    
    stars2[i+2][0]-=x_trig[direction][velocity2];
    stars2[i+2][1]-=y_trig[direction][velocity2];
    if(stars2[i+2][0]<0)stars2[i+2][0]+=249;
    else if(stars2[i+2][0]>249)stars2[i+2][0]-=249;
    if(stars2[i+2][1]<0)stars2[i+2][1]+=199;
    else if(stars2[i+2][1]>199)stars2[i+2][1]-=199;
    }
    
for(i=0;i<20;i+=2)
    {
    stars3[i][0]-=x_trig[direction][velocity3];
    stars3[i][1]-=y_trig[direction][velocity3];
    if(stars3[i][0]<0)stars3[i][0]+=249;
    else if(stars3[i][0]>249)stars3[i][0]-=249;
    if(stars3[i][1]<0)stars3[i][1]+=199;
    else if(stars3[i][1]>199)stars3[i][1]-=199;
    
    stars3[i+1][0]-=x_trig[direction][velocity3];
    stars3[i+1][1]-=y_trig[direction][velocity3];
    if(stars3[i+1][0]<0)stars3[i+1][0]+=249;
    else if(stars3[i+1][0]>249)stars3[i+1][0]-=249;
    if(stars3[i+1][1]<0)stars3[i+1][1]+=199;
    else if(stars3[i+1][1]>199)stars3[i+1][1]-=199;
    }
}    

move_ship()
{                            
ship_x+=x_trig[direction][velocity]; 
ship_y+=y_trig[direction][velocity];
if(ship_x<0)ship_x+=303;
else if(ship_x>303)ship_x-=303;
if(ship_y<0)ship_y+=183;
else if(ship_y>183)ship_y-=183;       

ship_sprite.x=ship_x;
ship_sprite.y=ship_y;  
}  

thrusters_off()
{
creg1.red=0;
creg1.green=0;
creg1.blue=0;
creg2.red=0; 
Set_Palette_Register(1,(RGB_color_ptr)&creg1); 
Set_Palette_Register(2,(RGB_color_ptr)&creg2);
}

move_ufo()
{
if(!count_ufo)  
        {
        rand2=rand()%2;
        if(rand2)rand24++;
        else rand24--;
        if(rand24==-1)rand24=23;
        else if(rand24==24)rand24=0;
        }
    count_ufo++;
    if(count_ufo==15)count_ufo=0;   
ufo_sprite.curr_frame++;

if(ufo_sprite.curr_frame==3 && ufo_sprite.state!=2)ufo_sprite.curr_frame=0;
else if(ufo_sprite.curr_frame==17)ufo_sprite.state=0;

ufo_sprite.x+=x_trig[rand24][TOP_SPEED-1];
ufo_sprite.y+=y_trig[rand24][TOP_SPEED-1]; 
if(ufo_sprite.x<0)ufo_sprite.x+=227;
else if(ufo_sprite.x>227)ufo_sprite.x-=227;
if(ufo_sprite.y<0)ufo_sprite.y+=167;
else if(ufo_sprite.y>167)ufo_sprite.y-=167;        
}       

move_and_plot_bullets()  
{
for(i=0;i<MAX_BULLETS;i++)
    {
    missile[i].x+=x_trig[(missile[i].direction)][6];    
    missile[i].y+=y_trig[(missile[i].direction)][6];
    if(missile[i].x<0 || missile[i].x>249
        || missile[i].y<0 ||missile[i].y>199)
        {
        num_bullets--;
        missile[i].active=0;
        }
    if(missile[i].active)
        double_buffer[(missile[i].y<<8)+(missile[i].y<<6)+missile[i].x]=44;    
    }
}

//COLLISION FUNCTIONS////////////////////////////////////////////////////////
ship_collision()
{ 
int ufo_x,ufo_y;
ufo_x=ufo_sprite.x+16;
ufo_y=ufo_sprite.y+16;
if(ufo_x>104 && ufo_x<144 
    && ufo_y>79 && ufo_y<119)ufo_sprite.state=2;
} 

bullet_collision()
{
for(i=0;i<MAX_BULLETS;i++)
    {
    if(missile[i].active)
        {
        if(missile[i].x>ufo_sprite.x && missile[i].x<(ufo_sprite.x+31)
            && missile[i].y>ufo_sprite.y &&
            missile[i].y<(ufo_sprite.y+31))
            {
            missile[i].active=0;
            double_buffer
            [((missile[i].y<<8)+(missile[i].y<<6))+missile[i].x]=0;
            
            ufo_sprite.state=2;            
            }
        }             
    }

}           

//DRAW FUNCTIONS/////////////////////////////////////////////////////////////
print_number(int x,int y,int num)
{
int row,col,o_set,counter=0;
o_set=((y<<8)+(y<<6)+x);
for(row=0;row<6;row++)
    {
    for(col=0;col<6;col++)
        {
        if(numbers[num][counter])
            double_buffer[o_set+(row<<8)+(row<<6)+col]=numbers[num][counter];
        counter++;
        }
    }
}

print_string(int x, int y,char string[])
{
int row,col,o_set,counter=0,co_set;
o_set=((y<<8)+(y<<6)+x);
for(i=0;string[i]!=NULL;i++)
    {
    co_set=i+(i<<2)+(i<<1);
    for(row=0;row<6;row++)
        {
        for(col=0;col<6;col++)
            {
            if(letters[string[i]-97][counter])
                double_buffer[o_set+(row<<8)+(row<<6)+col+co_set]=
                    letters[string[i]-97][counter];
            counter++;
            }
        }
    counter=0;    
    }
}    

draw_stars()
{
for(i=0;i<40;i+=5)
    {
    double_buffer[(stars1[i][1]<<8)+(stars1[i][1]<<6)+stars1[i][0]]=26;
    double_buffer[(stars1[i+1][1]<<8)+(stars1[i+1][1]<<6)+stars1[i+1][0]]=26;
    double_buffer[(stars1[i+2][1]<<8)+(stars1[i+2][1]<<6)+stars1[i+2][0]]=26;
    double_buffer[(stars1[i+3][1]<<8)+(stars1[i+3][1]<<6)+stars1[i+3][0]]=26;
    double_buffer[(stars1[i+4][1]<<8)+(stars1[i+4][1]<<6)+stars1[i+4][0]]=26;
    }
for(i=0;i<30;i+=5)
    {    
    double_buffer[(stars2[i][1]<<8)+(stars2[i][1]<<6)+stars2[i][0]]=24;
    double_buffer[(stars2[i+1][1]<<8)+(stars2[i+1][1]<<6)+stars2[i+1][0]]=24;
    double_buffer[(stars2[i+2][1]<<8)+(stars2[i+2][1]<<6)+stars2[i+2][0]]=24;
    double_buffer[(stars2[i+3][1]<<8)+(stars2[i+3][1]<<6)+stars2[i+3][0]]=24;
    double_buffer[(stars2[i+4][1]<<8)+(stars2[i+4][1]<<6)+stars2[i+4][0]]=24;
    }
for(i=0;i<20;i+=5)
    {    
    double_buffer[(stars3[i][1]<<8)+(stars3[i][1]<<6)+stars3[i][0]]=24;
    double_buffer[(stars3[i+1][1]<<8)+(stars3[i+1][1]<<6)+stars3[i+1][0]]=22;
    double_buffer[(stars3[i+2][1]<<8)+(stars3[i+2][1]<<6)+stars3[i+2][0]]=22;
    double_buffer[(stars3[i+3][1]<<8)+(stars3[i+3][1]<<6)+stars3[i+3][0]]=22;
    double_buffer[(stars3[i+4][1]<<8)+(stars3[i+4][1]<<6)+stars3[i+4][0]]=22;
    }    
}  

draw_ship()
{
sprite_width=15;
sprite_height=15;
    
Behind_Sprite_DB((sprite_ptr)&ship_sprite);
    
Draw_Sprite_DB((sprite_ptr)&ship_sprite);  
} 

draw_ufo()
{
sprite_width=31;
sprite_height=31;

Behind_Sprite_DB((sprite_ptr)&ufo_sprite);
Draw_Sprite_DB((sprite_ptr)&ufo_sprite);  
}              

draw_map()
{
double_buffer[8284]=15;
}     
/////////////////////////////////////////////////////////////////////////////

main()
{
//INITIALIZE/////////////////////////////////////////////////////////////////
init_bullets(); 

make_trig_table();

Install_Keyboard(); 

_setvideomode(19); 

create_double_buffer();

init_colour_regs();  

load_ufo_sprite();

load_ship_sprite();          

init_stars();

load_alphabet();

load_background();

    //MAIN GAME LOOP/////////////////////////////////////////////////////////
    do
    {  
    //ERASE//////////////////////////////////////////////////////////////////                           
    moved=0;
    if(ufo_sprite.state)erase_ufo();
    erase_ship();
    erase_bullets(); 
    erase_stars();  
    
    
    //MOVE///////////////////////////////////////////////////////////////////
    
    if(key_table[INDEX_ESC])done=1;
    if(key_table[INDEX_UP])thrust();
       
    if(key_table[INDEX_LEFT])frame--;
        
    if(key_table[INDEX_RIGHT])frame++;  
            
    if(key_table[INDEX_CTRL] && num_bullets<MAX_BULLETS && !count)
        new_bullet();
             
    if(frame==SHIP_FRAMES)frame=0;
    if(frame==-1)frame=SHIP_FRAMES-1;
    ship_sprite.curr_frame=frame;   
                                                     
    move_stars(); 
        
    if(!moved)thrusters_off(); 
    
    if(ufo_sprite.state)move_ufo();
        
    move_and_plot_bullets(); 
    
    //COLLISIONS///////////////////////////////////////////////////////////// 
    if(ufo_sprite.state)
        {
        ship_collision();
        bullet_collision();
        }
    //DRAW///////////////////////////////////////////////////////////////////    
        
    draw_stars();
    
    if(ship_sprite.state)draw_ship();
    
    if(ufo_sprite.state)draw_ufo();
    
    draw_map();
                                                 
    if(count)count--;  
    
    print_number(260,160,1);
    print_number(267,160,3);
    print_number(274,160,5);
    print_number(281,160,7);    
    print_string(10,10,"i{am{the{lord{of{the{dance"); 
    
    Wait_For_Vsync();
    Show_Double_Buffer(double_buffer);
    Delay(1);
    }
    while(!done);  

Sprite_Delete((sprite_ptr)&ship_sprite);    
Sprite_Delete((sprite_ptr)&ufo_sprite); 
                    
Delete_Keyboard();                     
Delete_Double_Buffer();
_setvideomode((-1));
return 0;
} 