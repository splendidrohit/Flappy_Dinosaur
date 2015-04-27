#include<bits/stdc++.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <time.h> 
#include<windows.h>
#include<mmsystem.h>
#define win_rate 2
#define FRONT 1
#define  BACK  0

#define dc 0.00014
#define translate(x,y) {x+=y;}
#define reflect(x){x=-1*x;}
#define ob_ht 6
#define level 0.000003

long long highest=0;
float xx=0;
bool end=false;
float inc=0;
float vmx=0;
float mx=0.05;
float dec=dc;
bool window_motion=false;
float win_start=-10;
float win_end=50;
int direction=FRONT;
float speedX= 0.02;
float posX=0;
bool jump=false;
float last;
std::vector< std::pair<float,float> > obstacles;
std::vector< std::pair<float,float> > range;
long score=0;
char hscore[100];
int hdig;
 // DRAGON COORDINATES
 float body[][2] = { {0, 3}, {1, 1}, {5, 1}, {8, 4}, {10, 4}, {11, 5},
  {11, 11.5}, {13, 12}, {13, 13}, {10, 13.5}, {13, 14}, {13, 15}, {11, 16},
  {8, 16}, {7, 15}, {7, 13}, {8, 12}, {7, 11}, {6, 6}, {4, 3}, {3, 2},
  {1, 2} };
  
 float arm[][2] = { {8, 10}, {9, 9}, {10, 9}, {13, 8}, {14, 9}, {16, 9},
  {15, 9.5}, {16, 10}, {15, 10}, {15.5, 11}, {14.5, 10}, {14, 11}, {14, 10},
  {13, 9}, {11, 11}, {9, 11} };
  
 float leg[][2] = { {8, 6}, {8, 4}, {9, 3}, {9, 2}, {8, 1}, {8, 0.5}, {9, 0},
  {12, 0}, {10, 1}, {10, 2}, {12, 4}, {11, 6}, {10, 7}, {9, 7} };
  
 float eye[][2] = { {8.75, 15}, {9, 14.7}, {9.6, 14.7}, {10.1, 15},
  {9.6, 15.25}, {9, 15.25} };
  
 int extreme[4]={5,7,0,13};
  
inline bool factor1(float x) {	if(x>win_end-10) return true;else return false;}
inline bool factor2(float x) {	if(x<win_end-30) return  true; else false;}

inline float generate_rand(){
	int	no = rand() % 30;
	int no2=rand() % 30;
	no=std::min(no,no2);
	return ((float)no*5)/5;
}

inline float generate_range(){
	int	no = rand() % 10+3;
	return ((float)no*5)/5;
}

void draw_obstacles(){
	glLineWidth(3);
	for(int i=std::max(0,(int)obstacles.size()-6);i<obstacles.size();i++){
	
		glColor3f(0.72,0.37,0.26);
		glBegin(GL_POLYGON);
			glVertex2f(obstacles[i].first,obstacles[i].second);
			glVertex2f(obstacles[i].first,obstacles[i].second+range[i].second);
			glColor3f(0.81,0.76,0.55);
			glVertex2f(obstacles[i].first+range[i].first,obstacles[i].second+range[i].second);
			glVertex2f(obstacles[i].first+range[i].first,obstacles[i].second);
		glEnd();
	}
	
	for(int i=std::max(0,(int)obstacles.size()-6);i<obstacles.size();i++){
		glColor3f(0.6,0.6,0.6);
		glBegin(GL_LINE_LOOP);
			glVertex2f(obstacles[i].first,obstacles[i].second);
			glVertex2f(obstacles[i].first,obstacles[i].second+range[i].second);
			glVertex2f(obstacles[i].first+range[i].first,obstacles[i].second+range[i].second);
			glVertex2f(obstacles[i].first+range[i].first,obstacles[i].second);
		glEnd();
	}
}

void translate_dragon(float x,float y){
  	int bl=22;
	int al=16;
	int ll=14;
	int el=6;
	for(int i=0;i<bl;i++)
	{
		translate(body[i][0],x);
		translate(body[i][1],y);
	}
	for(int i=0;i<al;i++)
	{
		translate(arm[i][0],x);
		translate(arm[i][1],y);
	}
	for(int i=0;i<ll;i++)
	{
		translate(leg[i][0],x);
		translate(leg[i][1],y);
	}
	for(int i=0;i<el;i++)
	{
		translate(eye[i][0],x);
		translate(eye[i][1],y);
	}
}

void reflect_dragon(){
	int bl=22;
		int al=16;
		int ll=14;
		int el=6;
	translate_dragon(-posX-3,0);
	for(int i=0;i<bl;i++)
		reflect(body[i][0]);
	for(int i=0;i<al;i++)
		reflect(arm[i][0]);
	for(int i=0;i<ll;i++)
		reflect(leg[i][0]);
	for(int i=0;i<el;i++)
		reflect(eye[i][0]);
	translate_dragon(posX+3,0);
}

void Draw_Dragon(){
	  	int bl=22;
		int al=16;
		int ll=14;
		int el=6;
	  	
		glColor3f(1.0,0.0,0.2);
		glLineWidth(2);
		glBegin(GL_LINE_LOOP);
			for(int i=0;i<bl;i++)
				glVertex2f(body[i][0],body[i][1]);
		glEnd();
		glColor3f(0.0,0.6,0.2);
		glBegin(GL_LINE_LOOP);
			for(int i=0;i<al;i++)
				glVertex2f(arm[i][0],arm[i][1]);
		glEnd();
		glColor3f(0.0,1.0,0.9);
		glBegin(GL_LINE_LOOP);
			for(int i=0;i<ll;i++)
				glVertex2f(leg[i][0],leg[i][1]);
		glEnd();
		glBegin(GL_LINE_LOOP);
			for(int i=0;i<el;i++)
				glVertex2f(eye[i][0],eye[i][1]);
		glEnd();
}
  
void initial(){
		
  	glClearColor (0.0, 0.0, 0, 1.0);
	glShadeModel (GL_SMOOTH);
    glLineStipple(1, 0x3F07);
    FILE *fp;
    fp=fopen("highest score.dat","r");
   if(fp==NULL){
   	FILE *fp1;
		fp1=fopen("highest score.dat","w");
		fprintf(fp1,"%d",highest);
		fclose(fp1);
   }
   else{
   	fscanf(fp,"%lld",&highest);
   }
	
    	hdig=0;
		long no=highest;
		while(no){					
			hscore[hdig++]=(no%10)+'0';
			no/=10;
		}
 	    int bl=22;
		int al=16;
		int ll=14;
		int el=6;
		float tx=4.5,ty=3.5;
	for(int i=0;i<bl;i++)
		{	body[i][0]=body[i][0]/tx;body[i][1]=body[i][1]/ty;}
	for(int i=0;i<al;i++)
		{	arm[i][0]=arm[i][0]/tx;arm[i][1]=arm[i][1]/ty;}
	for(int i=0;i<ll;i++)
		{	leg[i][0]=leg[i][0]/tx;leg[i][1]=leg[i][1]/ty;}
	for(int i=0;i<el;i++)
		{	eye[i][0]=eye[i][0]/tx;eye[i][1]=eye[i][1]/ty;}
		
	obstacles.push_back(std::make_pair(50,generate_rand()));
	obstacles.push_back(std::make_pair(80,generate_rand()));
	obstacles.push_back(std::make_pair(110,generate_rand()));
	 
	range.push_back(std::make_pair(generate_range(),generate_range()));
	range.push_back(std::make_pair(generate_range(),generate_range()));
	range.push_back(std::make_pair(generate_range(),generate_range()));
	
	last=110;
}

int generate_dis(){
  	int no=rand()%25+20;
  	return no;
}

void win_move(int fac){
  		win_start+=(fac*speedX);
		win_end+=(fac*speedX);
		if(win_end>last-40){
			int no=generate_dis();
			obstacles.push_back(std::make_pair(last+no,generate_rand()));
			range.push_back(std::make_pair(generate_range(),generate_range()));
			last+=no;
		}
}
  
void init(void)
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(win_start,win_end,-20,50);
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glClear (GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT);
}

bool collision_detection(){						//Detection of collision when dino's 'x/y' meets any blocks's 'x/y' respectively.
	for(int i=std::max(0,(int)obstacles.size()-6);i<obstacles.size();i++){
		if(arm[extreme[0]][0]>=obstacles[i].first && arm[extreme[0]][0]<=obstacles[i].first+range[i].first && arm[extreme[0]][1]+inc<=obstacles[i].second+range[i].second && arm[extreme[0]][1]+inc>=obstacles[i].second)
			return true;
		if(leg[extreme[1]][0]>=obstacles[i].first && leg[extreme[1]][0]<=obstacles[i].first+range[i].first && leg[extreme[1]][1]+inc<=obstacles[i].second+range[i].second && leg[extreme[1]][1]+inc>=obstacles[i].second)
			return true;
		if(body[extreme[2]][0]>=obstacles[i].first && body[extreme[2]][0]<=obstacles[i].first+range[i].first && body[extreme[2]][1]+inc<=obstacles[i].second+range[i].second && body[extreme[2]][1]+inc>=obstacles[i].second)
			return true;
		if(body[extreme[3]][0]>=obstacles[i].first && body[extreme[3]][0]<=obstacles[i].first+range[i].first && body[extreme[3]][1]+inc<=obstacles[i].second+range[i].second && body[extreme[3]][1]+inc>=obstacles[i].second)
			return true;
		if(inc>40)
			return true;
	}
	return false;
}

void init_again(){		//RESTART

	FILE *fp;
		fp=fopen("highest score.dat","w");
		fprintf(fp,"%d",highest);
		fclose(fp);
	hdig=0;
		long no=highest;
		while(no){					
			hscore[hdig++]=(no%10)+'0';
			no/=10;
		}
	translate_dragon(-posX,0);
 	end=false;
 	inc=0;
 	vmx=0;
 	mx=0.05;
 	dec=dc;
 	window_motion=false;
 	win_start=-10;
 	win_end=50;
 	direction=FRONT;
    speedX= 0.02;
    posX=0;
    score=0;
    xx=0;
 	jump=false;
 	obstacles.clear();
 	range.clear();
 	obstacles.push_back(std::make_pair(50,generate_rand()));
	obstacles.push_back(std::make_pair(80,generate_rand()));
	obstacles.push_back(std::make_pair(110,generate_rand()));
	 
	range.push_back(std::make_pair(generate_range(),generate_range()));
	range.push_back(std::make_pair(generate_range(),generate_range()));
	range.push_back(std::make_pair(generate_range(),generate_range()));
	
	last=110;
}

void display(){
	if(score>highest){
		highest=score;
	}
  	if(posX>xx+80)
  			xx+=80;
  		init();
  		glLineWidth(2);
  			glColor3f(1.0,1.0,1.0);
  		glEnable(GL_LINE_STIPPLE);
			glBegin(GL_LINES);
			glVertex2f(xx-10.0f,0.0f);
			glVertex2f(xx+200,0.0f);
			glEnd();
			
			
			glBegin(GL_LINES);
			glVertex2f(xx-10.0f,45.0f);
			glVertex2f(xx+200,45.0f);
			glEnd();
		glDisable(GL_LINE_STIPPLE);
		
	if(jump){
		inc+=vmx;
		vmx-=dec;
	}
	if(collision_detection()){
		//PlaySound("dead.wav",NULL,SND_ASYNC);
		end=true;
		vmx=-mx;
		dec=dc;
		inc+=vmx;
		vmx-=dec;
		jump=true;
		glTranslatef(0,inc,0);
		Draw_Dragon();
	glTranslatef(0,-inc,0);
	}
	else
	{
		jump=true;
	}
	if(inc<=0 && end==false)
	{
		vmx=0;
		inc=0;
		dec=0;
	}
	if(end==false){		//If the game continues , increase the speed
		speedX+=level;
		posX+=speedX;
		translate_dragon(speedX,0);		
	}
	if(collision_detection()){
			//PlaySound("dead.wav",NULL,SND_ASYNC);
				end=true;
		vmx=-mx;
		dec=dc;
		inc+=2*vmx;
		vmx-=dec;
		jump=true;
		glTranslatef(0,inc,0);
		Draw_Dragon();
	glTranslatef(0,-inc,0);
			}
	else if(end==false)
		win_move(1);
		score++;
	glTranslatef(0,inc,0);
		Draw_Dragon();
	glTranslatef(0,-inc,0);
		draw_obstacles();
		
		glColor3f(1.0,1.0,1.0);
		glRasterPos2f(posX-9,47);
		char aa[100]="Distance : ";
		for(int i=0;aa[i]!='\0';i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,aa[i]);
			
			
		int dig=0;
		long no=score;
		while(no){					//INCREMENT THE SCORE AS HE COVERS THE DISTANCE
			aa[dig++]=(no%10)+'0';
			no/=10;
		}
		for(int i=dig-1;i>=0;i--)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,aa[i]);
		
		
		glRasterPos2f(posX-9,-5);
		char aa2[100]="Highest Score : ";
		for(int i=0;aa2[i]!='\0';i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,aa2[i]);
		
		glRasterPos2f(posX+1,-5);
		 if(score<highest)
		for(int i=hdig-1;i>=0;i--)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,hscore[i]);
		else
		for(int i=dig-1;i>=0;i--)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,aa[i]);
		
		
		glColor3ub( (char) rand()%256, (char) rand()%256, (char) rand()%256);
		glRasterPos2f(posX+15,47);
		//glColor3f(1.0,1.0,1.0);
		
		char aad[100]="** FLAPPY DINO **";
		for(int i=0;aad[i]!='\0';i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,aad[i]);
		
		
	
		if(end==true){
			glColor3f(1.0,1.0,1.0);
			glRasterPos2f(posX+12,17);
			char aaa[100]="GAME OVER! Press 'r' to continue.";
			for(int i=0;aaa[i]!='\0';i++)
				glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,aaa[i]);
				FILE *fp;
			fp=fopen("highest score.dat","w");
			fprintf(fp,"%d",highest);
			fclose(fp);
		}
		
		glFlush ();
		
		if(inc>-30)
	glutPostRedisplay();
}
  
void keyPressed(unsigned char Key, int x, int y)
{
	if(Key==' ' && end==false){  //SPAACE bar to jump over the blocks
		vmx=mx;
		dec=dc;
		jump=true;
		//PlaySound("jump.wav",NULL,SND_ASYNC);
		glutPostRedisplay();
	}
	else if(Key=='r' || Key=='R')
	{
		init_again();
		glutPostRedisplay();}
	else if(Key==27)
	{
		FILE *fp;
		fp=fopen("highest score.dat","w");
		fprintf(fp,"%d",highest);
		fclose(fp);
		exit(0);
glutPostRedisplay();}
	
}

void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);	
}
  
int main(int argc, char** argv)
{
	srand (time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1000, 1000);
	glutInitWindowPosition (100, 100);
	glutCreateWindow (argv[0]);
	initial ();
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutDisplayFunc(display);


	glutMainLoop();
	return 0;
}
