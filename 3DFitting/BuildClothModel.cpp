#include "BuildClothModel.h"

using namespace std;

//from square cloth
int number = 0;
const GLfloat edge = 20;
const int line = 40;
M3DVector3f ClothCenter = {0.0f, 0.0f, 0.0f};

void outFileClothSquare() {
	ofstream out_file("ParticleList.txt",ios::out);
	if(!out_file)
		exit(-1);
	for(int tempz=0;tempz<line;tempz++)
	{
		for(int tempx=0;tempx<line;tempx++)
		{
			out_file<<number<<" ";
			out_file<<(float)tempx*edge<<" "<<0.0<<" "<<(float)tempz*edge<<" ";
			if(tempz<line-1)
				out_file<<number+line<<" ";
			else if(tempz=line-1)
				out_file<<-1<<" ";
			if(tempz>0)
				out_file<<number-line<<" ";
			else if(tempz==0)
				out_file<<-1<<" ";
			if(tempx>0)
				out_file<<number-1<<" ";
			else if(tempx==0)
				out_file<<-1<<" ";
			if(tempx<line-1)
				out_file<<number+1<<" ";
			else if(tempx==line-1)
				out_file<<-1<<" ";
			
			
			if(tempx==0||tempz==line-1)
				out_file<<-1<<" ";
			else if(tempx>0&&tempz<line-1)
				out_file<<number+line-1<<" ";
			if(tempx==line-1||tempz==line-1)
				out_file<<-1<<" ";
			else if(tempx<line-1&&tempz<line-1)
				out_file<<number+line+1<<" ";
			if(tempx==0||tempz==0)
				out_file<<-1<<" ";
			else if(tempx>0 && tempz>0)
				out_file<<number-line-1<<" ";
			if(tempx==line-1 || tempz==0)
				out_file<<-1<<" ";
			else if(tempx<line-1 && tempz>0)
				out_file<<number-line+1<<" ";

			if(tempx-2<0 || tempz+2 > line-1)
				out_file<<-1<<" ";
			else
				out_file<<number+line+line-2<<" ";
			if(tempz+2>line-1)
				out_file<<-1<<" ";
			else
				out_file<<number+2*line<<" ";
			if(tempx+2>line-1||tempz+2>line-1)
				out_file<<-1<<" ";
			else 
				out_file<<number+line*2+2<<" ";
			if(tempx-2<0)
				out_file<<-1<<" ";
			else
				out_file<<number-2<<" ";
			if(tempx+2>line-1)
				out_file<<-1<<" ";
			else
				out_file<<number+2<<" ";
			if(tempx-2<0 || tempz-2<0)
				out_file<<-1<<" ";
			else
				out_file<<number-line*2-2<<" ";
			if(tempz-2<0)
				out_file<<-1<<" ";
			else
				out_file<<number-line*2<<" ";
			if(tempx+2>line-1 || tempz-2<0)
				out_file<<-1<<" ";
			else
				out_file<<number-2*line+2<<" ";
			 number++;
			 out_file<<endl;
		}
	}
}

//cloth t shirt
bool inCircle1(float x1 ,float y1 , float z1)
{
	float x = (float)(MODELX + (float)(WIDNUMBER - 1) * EDGELENGTH/2.0);
	float y = (float)MODELY;
	float z = (float)MODELZ;
	float a = (x1 - x) * (x1 - x) + (y1 - y) * (y1 - y) + (z1 - z) * (z1 - z);
	if( a > COLLARRADIUS * COLLARRADIUS )
		return false;
	else
		return true;
}
bool inCircle2(float x1 ,float y1 , float z1)
{
	float x = (float)(MODELX + (float)(WIDNUMBER - 1) * EDGELENGTH/2.0);
	float y = (float)MODELY;
	float z = (float)(MODELZ + GAPLENGTH);
	float a = (x1 - x) * (x1 - x) + (y1 - y) * (y1 - y) + (z1 - z) * (z1 - z);
	if( a > COLLARRADIUS * COLLARRADIUS )
		return false;
	else
		return true;
}
void inFileClothModel(void)
{
	ifstream in_file("ClothPoints.txt",ios::in);
	if (!in_file) exit(-1);
	for(int TEMP=0;!in_file.eof();TEMP++)
	{
		int num;
 //		int xyz[3];
		M3DVector3f xyz;
		int structadj[4]; //structual constraint
		int shearadj[4]; //Shear constraint
		int bendadj[8]; //Bending constraint
		in_file>>num;
//		printf("%d  ",num);
		for(int temp=0;temp<3;temp++) 
		{
			in_file>>xyz[temp];
		}
//		printf("%f %f %f\n",xyz[0],xyz[1],xyz[2]);
		for(int temp=0;temp<4;temp++)
		{
			in_file>>structadj[temp];
		}
//		printf("%d %d %d %d",structadj[0],structadj[1],structadj[2],structadj[3]);
		for(int temp=0;temp<4;temp++)
		{
			in_file>>shearadj[temp];
		}
		for(int temp=0;temp<8;temp++)
		{
			in_file>>bendadj[temp];
		}
		int a;
		bool exist;
		in_file>>a;
		if(a == 1)
			exist = true;
		else
			exist = false;
		initPtc(&ClothLnk[TEMP],num,MASS,xyz,structadj,shearadj,bendadj,exist);
	}
}

int outFileClothModel() {
	ofstream out_file("ClothPoints.txt",ios::out);
	if(!out_file)
		exit(-1);
	for(int tempy = 0;tempy < HEIGHTNUMBER ; tempy ++)
	{
		for(int tempx=0;tempx < WIDNUMBER;tempx ++)
		{
			if(inCircle1((float)tempx*EDGELENGTH + MODELX , MODELY - (float)tempy * EDGELENGTH , (float)MODELZ))
			{
				out_file<<number<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";

				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";

				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<endl;
				number ++;
			}
			else
			{
				float a = (float)tempx*EDGELENGTH + MODELX;
				float b = MODELY - (float)tempy * EDGELENGTH;
				float c = (float)MODELZ;
				out_file<<number<<" ";
				out_file<<(float)tempx*EDGELENGTH + MODELX <<" "<<MODELY - (float)tempy * EDGELENGTH <<" "<<(float)MODELZ<<" ";
				if(tempy < HEIGHTNUMBER-1)
				{
					if(inCircle1(a,b - EDGELENGTH,c))
						out_file<<-1<<" ";
					else
						out_file<<number+WIDNUMBER<<" ";
				}
				else if(tempy = HEIGHTNUMBER - 1)
					out_file<<-1<<" ";
				if(tempy > 0)
				{
					if(inCircle1(a,b + EDGELENGTH,c))
						out_file<<-1<<" ";
					else
						out_file<<number - WIDNUMBER<<" ";
				}
				else if(tempy == 0)
					out_file<<number + HEIGHTNUMBER * WIDNUMBER<<" ";
				if(tempx > 0)
				{
					if(inCircle1(a - EDGELENGTH,b,c))
						out_file<<-1<<" ";
					else
						out_file<< number - 1 <<" ";
				}
				else if(tempx == 0)
				{
					if(tempy > CUFFLENGTH)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				if(tempx < WIDNUMBER - 1)
				{
					if(inCircle1(a + EDGELENGTH,b,c))
						out_file<<-1<<" ";
					else
						out_file<<number + 1<<" ";
				}
				else if(tempx==WIDNUMBER - 1)
				{
					if(tempy > CUFFLENGTH)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}



				if(tempx == 0 || tempy == HEIGHTNUMBER - 1)
				{
					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else if(tempx > 0 && tempy < HEIGHTNUMBER - 1)
				{
					if(inCircle1(a - EDGELENGTH ,b - EDGELENGTH,c))
						out_file<<-1<<" ";
					else
						out_file<<number + WIDNUMBER - 1<<" ";
				}
				if(tempx == WIDNUMBER - 1 || tempy == HEIGHTNUMBER - 1)
				{
					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else if(tempx < WIDNUMBER - 1 && tempy < HEIGHTNUMBER - 1)
				{
					if(inCircle1(a + EDGELENGTH,b - EDGELENGTH,c))
						out_file<<-1<<" ";
					else
						out_file<<number + WIDNUMBER + 1<<" ";
				}
				if(tempx == 0 || tempy == 0)
				{
					if(tempy == 0 && tempx != 0)
					{
						if(inCircle2(a - EDGELENGTH ,b ,c + GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number + HEIGHTNUMBER * WIDNUMBER - 1<<" ";
					}
					else if(tempx == 0 && tempy > CUFFLENGTH + 1)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else if(tempx > 0 && tempy > 0)
				{
					if(inCircle1(a - EDGELENGTH,b + EDGELENGTH,c))
						out_file<<-1<<" ";
					else
						out_file<<number-WIDNUMBER-1<<" ";
				}
				if(tempx == WIDNUMBER - 1 || tempy == 0)
				{
					if(tempy == 0 && tempx != WIDNUMBER - 1)
					{
						if(inCircle2(a + EDGELENGTH,b,c + GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1<<" ";
					}
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH + 1)
					{
						if(inCircle1(a + EDGELENGTH,b + EDGELENGTH,c))
							out_file<<-1<<" ";
						else
							out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
					}
					else
						out_file<<-1<<" ";
				}
				else if(tempx < WIDNUMBER - 1 && tempy>0)
				{
					if(inCircle1(a + EDGELENGTH,b+EDGELENGTH ,c))
						out_file<<-1<<" ";
					else
						out_file<<number - WIDNUMBER + 1<<" ";
				}


				if(tempx - 2 < 0 || tempy + 2 > HEIGHTNUMBER - 1)
				{
					if(tempx == 0 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
					else if(tempx == 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle1(a - EDGELENGTH * 2,b - EDGELENGTH * 2,c))
						out_file<<-1<<" ";
					else
						out_file<<number + WIDNUMBER + WIDNUMBER - 2<<" ";
				}
				if(tempy + 2 > HEIGHTNUMBER - 1)
				{
					out_file<<-1<<" ";
				}
				else
					out_file<<number+2 * WIDNUMBER<<" ";
				if(tempx + 2 > WIDNUMBER - 1 || tempy + 2 > HEIGHTNUMBER - 1)
				{
					if(tempx ==  WIDNUMBER - 2 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else 
				{
					if(inCircle1(a + EDGELENGTH * 2,b - EDGELENGTH * 2,c))
						out_file<<-1<<" ";
					else
						out_file<<number+WIDNUMBER * 2 + 2<<" ";
				}
				if(tempx - 2 < 0)
				{
					if(tempx == 0 && tempy > CUFFLENGTH)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1<<" ";
					else if(tempx == 1 && tempy > CUFFLENGTH)
						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle1(a - EDGELENGTH * 2,b,c))
						out_file<<-1<<" ";
					else
						out_file<<number-2<<" ";
				}
				if(tempx + 2 > WIDNUMBER - 1)
				{
					if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1<<" ";
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH)
						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle1(a + EDGELENGTH * 2,b,c))
						out_file<<-1<<" ";
					else
						out_file<<number + 2<<" ";
				}
				if(tempx-2 < 0 || tempy - 2 <0)
				{
					if(tempy == 0 && tempx >= 2)
					{
						if(inCircle2(a - EDGELENGTH * 2,b - EDGELENGTH,c + GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER -2<<" ";
					}
					else if(tempy == 1 && tempx >= 2)
					{
						if(inCircle2(a - EDGELENGTH * 2,b + EDGELENGTH,c + GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER -2<<" ";
					}
					else if(tempx == 0 && tempy > CUFFLENGTH + 2)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
					else if(tempx == 1 && tempy > CUFFLENGTH + 2)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle1(a - EDGELENGTH * 2,b + EDGELENGTH * 2,c))
						out_file<<-1<<" ";
					else
						out_file<<number - WIDNUMBER * 2 - 2<<" ";
				}
				if(tempy-2<0)
				{
					if(tempy == 0)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
					else if(tempy == 1)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle1(a ,b + EDGELENGTH * 2,c))
						out_file<<-1<<" ";
					else
						out_file<<number-WIDNUMBER * 2<<" ";
				}
				if(tempx+2 > WIDNUMBER - 1 || tempy-2<0)
				{
					if(tempy == 0 && tempx < WIDNUMBER - 2)
					{
						if(inCircle2(a + EDGELENGTH * 2,b - EDGELENGTH,c + GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER + 2<<" ";
					}
					else if(tempy == 1 && tempx < WIDNUMBER - 2)
					{
						if(inCircle2(a + EDGELENGTH * 2,b + EDGELENGTH,c + GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER + 2<<" ";
					}
					else if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH + 2)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH + 2)
						out_file<<number + HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle1(a + EDGELENGTH * 2,b + EDGELENGTH * 2,c))
						out_file<<-1<<" ";
					else
						out_file<<number - 2 * WIDNUMBER + 2<<" ";
				}
				out_file<<1<<endl;
				number++;
			}
		}
	}
	for(int tempy = 0;tempy < HEIGHTNUMBER ; tempy ++)
	{
		for(int tempx=0;tempx < WIDNUMBER;tempx ++)
		{
			if(inCircle2((float)tempx*EDGELENGTH + MODELX , MODELY - (float)tempy * EDGELENGTH , (float)MODELZ + GAPLENGTH))
			{
				out_file<<number<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";

				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";

				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<endl;
				number ++;
			}
			else
			{
				float a = (float)tempx*EDGELENGTH + MODELX;
				float b = MODELY - (float)tempy * EDGELENGTH;
				float c = (float)MODELZ + GAPLENGTH;
				out_file<<number<<" ";
				out_file<<(float)tempx*EDGELENGTH + MODELX <<" "<<MODELY - (float)tempy * EDGELENGTH <<" "<<(float)MODELZ + GAPLENGTH<<" ";
				if(tempy < HEIGHTNUMBER-1)
					out_file<<number+WIDNUMBER<<" ";
				else if(tempy = HEIGHTNUMBER - 1)
					out_file<<-1<<" ";
				if(tempy > 0)
				{
					if(inCircle2(a ,b + EDGELENGTH,c))
						out_file<<-1<<" ";
					else
						out_file<<number-WIDNUMBER<<" ";
				}
				else if(tempy == 0)
					out_file<<number - HEIGHTNUMBER * WIDNUMBER<<" ";
				if(tempx > 0)
				{
					if(inCircle2(a - EDGELENGTH ,b ,c))
						out_file<<-1<<" ";
					else
						out_file<< number - 1 <<" ";
				}
				else if(tempx == 0)
				{
					if(tempy > CUFFLENGTH)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				if(tempx < WIDNUMBER - 1)
				{
					if(inCircle2(a + EDGELENGTH ,b ,c))
						out_file<<-1<<" ";
					else
						out_file<<number + 1<<" ";
				}
				else if(tempx==WIDNUMBER - 1)
				{
					if(tempy > CUFFLENGTH)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}



				if(tempx == 0 || tempy == HEIGHTNUMBER - 1)
				{
					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else if(tempx > 0 && tempy < HEIGHTNUMBER - 1)
				{
					if(inCircle2(a - EDGELENGTH ,b - EDGELENGTH ,c))
						out_file<<-1<<" ";
					else
						out_file<<number + WIDNUMBER - 1<<" ";
				}
				if(tempx == WIDNUMBER - 1 || tempy == HEIGHTNUMBER - 1)
				{
					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else if(tempx < WIDNUMBER - 1 && tempy < HEIGHTNUMBER - 1)
				{
					if(inCircle2(a + EDGELENGTH ,b - EDGELENGTH ,c))
						out_file<<-1<<" ";
					else
						out_file<<number + WIDNUMBER + 1<<" ";
				}
				if(tempx == 0 || tempy == 0)
				{
					if(tempy == 0 && tempx != 0)
					{
						if(inCircle1(a - EDGELENGTH ,b ,c - GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number - HEIGHTNUMBER * WIDNUMBER - 1<<" ";
					}
					else if(tempx == 0 && tempy > CUFFLENGTH + 1)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else if(tempx > 0 && tempy > 0)
				{
					if(inCircle2(a - EDGELENGTH ,b + EDGELENGTH ,c))
						out_file<<-1<<" ";
					else
						out_file<<number-WIDNUMBER-1<<" ";
				}
				if(tempx == WIDNUMBER - 1 || tempy == 0)
				{
					if(tempy == 0 && tempx != WIDNUMBER - 1)
					{
						if(inCircle1(a + EDGELENGTH ,b ,c - GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1<<" ";
					}
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH + 1)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else if(tempx < WIDNUMBER - 1 && tempy > 0)
				{
					if(inCircle2(a + EDGELENGTH ,b + EDGELENGTH ,c))
						out_file<<-1<<" ";
					else
						out_file<<number - WIDNUMBER + 1<<" ";
				}


				if(tempx - 2 < 0 || tempy + 2 > HEIGHTNUMBER - 1)
				{
					if(tempx == 0 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
					else if(tempx == 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle2(a - EDGELENGTH*2 ,b - EDGELENGTH*2 ,c))
						out_file<<-1<<" ";
					else
						out_file<<number + WIDNUMBER + WIDNUMBER - 2<<" ";
				}
				if(tempy + 2 > HEIGHTNUMBER - 1)
				{
					out_file<<-1<<" ";
				}
				else
				{
					if(inCircle2(a ,b - EDGELENGTH*2 ,c))
						out_file<<-1<<" ";
					else
						out_file<<number+2 * WIDNUMBER<<" ";
				}
				if(tempx + 2 > WIDNUMBER - 1 || tempy + 2 > HEIGHTNUMBER - 1)
				{
					if(tempx ==  WIDNUMBER - 2 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else 
				{
					if(inCircle2(a + EDGELENGTH*2 ,b - EDGELENGTH*2 ,c))
						out_file<<-1<<" ";
					else
						out_file<<number+WIDNUMBER * 2 + 2<<" ";
				}
				if(tempx - 2 < 0)
				{
					if(tempx == 0 && tempy > CUFFLENGTH)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1<<" ";
					else if(tempx == 1 && tempy > CUFFLENGTH)
						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle2(a - EDGELENGTH*2 ,b ,c))
						out_file<<-1<<" ";
					else
						out_file<<number-2<<" ";
				}
				if(tempx + 2 > WIDNUMBER - 1)
				{
					if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1<<" ";
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH)
						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle2(a + EDGELENGTH*2 ,b ,c))
						out_file<<-1<<" ";
					else
						out_file<<number + 2<<" ";
				}
				if(tempx-2 < 0 || tempy - 2 <0)
				{
					if(tempy == 0 && tempx >= 2)
					{
						if(inCircle1(a - EDGELENGTH*2 ,b - EDGELENGTH,c - GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER -2<<" ";
					}
					else if(tempy == 1 && tempx >= 2)
					{
						if(inCircle1(a - EDGELENGTH*2 ,b + EDGELENGTH,c - GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER -2<<" ";
					}
					else if(tempx == 0 && tempy > CUFFLENGTH + 2)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
					else if(tempx == 1 && tempy > CUFFLENGTH + 2)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle2(a - EDGELENGTH*2 ,b + EDGELENGTH*2,c))
						out_file<<-1<<" ";
					else
						out_file<<number - WIDNUMBER * 2 - 2<<" ";
				}
				if(tempy-2<0)
				{
					if(tempy == 0)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
					else if(tempy == 1)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle2(a ,b + EDGELENGTH*2,c))
						out_file<<-1<<" ";
					else
						out_file<<number-WIDNUMBER * 2<<" ";
				}
				if(tempx+2 > WIDNUMBER - 1 || tempy-2<0)
				{
					if(tempy == 0 && tempx < WIDNUMBER - 2)
					{
						if(inCircle1(a + EDGELENGTH*2 ,b - EDGELENGTH,c - GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER + 2<<" ";
					}
					else if(tempy == 1 && tempx < WIDNUMBER - 2)
					{
						if(inCircle1(a + EDGELENGTH*2 ,b + EDGELENGTH,c - GAPLENGTH))
							out_file<<-1<<" ";
						else
							out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER + 2<<" ";
					}
					else if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH + 2)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH + 2)
						out_file<<number - HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
					else
						out_file<<-1<<" ";
				}
				else
				{
					if(inCircle2(a + EDGELENGTH*2 ,b + EDGELENGTH*2,c))
						out_file<<-1<<" ";
					else
						out_file<<number - 2 * WIDNUMBER + 2<<" ";
				}
				out_file<<1<<endl;
				number++;
			}
		}
	}
	for(int i = 0; i < PTCAMT - HEIGHTNUMBER*WIDNUMBER*2; i ++)
	{
				out_file<<number<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";

				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";

				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<" ";
				out_file<<-1<<endl;
				number ++;
	}
	number =  HEIGHTNUMBER*WIDNUMBER*2;
	return 0;
}

void inFileClothSquare() {
	ifstream in_file("ParticleList.txt",ios::in);
	if (!in_file) exit(-1);
	for(int TEMP=0;TEMP<PTCAMT;TEMP++)
	{
		int num;
//		int xyz[3];
		M3DVector3f xyz;
		int structadj[4]; //structual constraint
		int shearadj[4]; //Shear constraint
		int bendadj[8]; //Bending constraint
		in_file>>num;
//		printf("%d  ",num);
		for(int temp=0;temp<3;temp++) 
		{
			in_file>>xyz[temp];
		}
//		printf("%f %f %f\n",xyz[0],xyz[1],xyz[2]);
		for(int temp=0;temp<4;temp++)
		{
			in_file>>structadj[temp];
		}
//		printf("%d %d %d %d",structadj[0],structadj[1],structadj[2],structadj[3]);
		for(int temp=0;temp<4;temp++)
		{
			in_file>>shearadj[temp];
		}
		for(int temp=0;temp<8;temp++)
		{
			in_file>>bendadj[temp];
		}
		initPtc(&ClothLnk[TEMP],num,Mass,xyz,structadj,shearadj,bendadj);
	}
}

void shiftClothModelPosition(M3DVector3f shift) {
	for (int i = 0; i < PTCAMT; i ++) {
		m3dAddVectors3(ClothLnk[i].position, ClothLnk[i].position, shift);
	}
}

void getClothModelCenter() {
	int count = 0;
	for (int i = 0; i < PTCAMT; i ++) {
		if (ClothLnk[i].exist) {
			m3dAddVectors3(ClothCenter, ClothCenter, ClothLnk[i].position);
			count ++;
		}
	}
	m3dScaleVector3(ClothCenter, (GLfloat)1/count);
}

//void getValidPoints() {
//	for (int i = 0; i < PTCAMT; i ++) {
//		if (!ClothLnk[i].exist) {


//int outFileClothModel()
//{
//	ofstream out_file("ClothPoints.txt",ios::out);
//	if(!out_file)
//		exit(-1);
//	for(int tempy = 0;tempy < HEIGHTNUMBER ; tempy ++)
//	{
//		for(int tempx=0;tempx < WIDNUMBER;tempx++)
//		{
//			if(inCircle1((float)tempx*EDGELENGTH + MODELX , MODELY - (float)tempy * EDGELENGTH , (float)MODELZ))
//			{
//				number ++;
//			}
//			else
//			{
//				float a = (float)tempx*EDGELENGTH + MODELX;
//				float b = MODELY - (float)tempy * EDGELENGTH;
//				float c = (float)MODELZ;
//				out_file<<number<<" ";
//				out_file<<(float)tempx*EDGELENGTH + MODELX <<" "<<MODELY - (float)tempy * EDGELENGTH <<" "<<(float)MODELZ<<" ";
//				if(tempy < HEIGHTNUMBER-1)
//				{
//					if(inCircle1(a,b - EDGELENGTH,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number+WIDNUMBER<<" ";
//				}
//				else if(tempy = HEIGHTNUMBER - 1)
//					out_file<<-1<<" ";
//				if(tempy > 0)
//				{
//					if(inCircle1(a,b + EDGELENGTH,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number - WIDNUMBER<<" ";
//				}
//				else if(tempy == 0)
//					out_file<<number + HEIGHTNUMBER * WIDNUMBER<<" ";
//				if(tempx > 0)
//				{
//					if(inCircle1(a - EDGELENGTH,b,c))
//						out_file<<-1<<" ";
//					else
//						out_file<< number - 1 <<" ";
//				}
//				else if(tempx == 0)
//				{
//					if(tempy > CUFFLENGTH)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				if(tempx < WIDNUMBER - 1)
//				{
//					if(inCircle1(a + EDGELENGTH,b,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + 1<<" ";
//				}
//				else if(tempx==WIDNUMBER - 1)
//				{
//					if(tempy > CUFFLENGTH)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//
//
//
//				if(tempx == 0 || tempy == HEIGHTNUMBER - 1)
//				{
//					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx > 0 && tempy < HEIGHTNUMBER - 1)
//				{
//					if(inCircle1(a - EDGELENGTH ,b - EDGELENGTH,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + WIDNUMBER - 1<<" ";
//				}
//				if(tempx == WIDNUMBER - 1 || tempy == HEIGHTNUMBER - 1)
//				{
//					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx < WIDNUMBER - 1 && tempy < HEIGHTNUMBER - 1)
//				{
//					if(inCircle1(a + EDGELENGTH,b - EDGELENGTH,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + WIDNUMBER + 1<<" ";
//				}
//				if(tempx == 0 || tempy == 0)
//				{
//					if(tempy == 0 && tempx != 0)
//					{
//						if(inCircle2(a - EDGELENGTH ,b ,c + GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number + HEIGHTNUMBER * WIDNUMBER - 1<<" ";
//					}
//					else if(tempx == 0 && tempy > CUFFLENGTH + 1)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx > 0 && tempy > 0)
//				{
//					if(inCircle1(a - EDGELENGTH,b + EDGELENGTH,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number-WIDNUMBER-1<<" ";
//				}
//				if(tempx == WIDNUMBER - 1 || tempy == 0)
//				{
//					if(tempy == 0 && tempx != WIDNUMBER - 1)
//					{
//						if(inCircle2(a + EDGELENGTH,b,c + GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1<<" ";
//					}
//					else if(tempx == WIDNUMBER && tempy > CUFFLENGTH + 1)
//					{
//						if(inCircle1(a + EDGELENGTH,b + EDGELENGTH,c))
//							out_file<<-1<<" ";
//						else
//							out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					}
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx < WIDNUMBER - 1 && tempy>0)
//					out_file<<number - WIDNUMBER + 1<<" ";
//
//
//
//				if(tempx - 2 < 0 || tempy + 2 > HEIGHTNUMBER - 1)
//				{
//					if(tempx == 0 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
//					else if(tempx == 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle1(a - EDGELENGTH * 2,b - EDGELENGTH * 2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + WIDNUMBER + WIDNUMBER - 2<<" ";
//				}
//				if(tempy + 2 > HEIGHTNUMBER - 1)
//				{
//					out_file<<-1<<" ";
//				}
//				else
//					out_file<<number+2 * WIDNUMBER<<" ";
//				if(tempx + 2 > WIDNUMBER - 1 || tempy + 2 > HEIGHTNUMBER - 1)
//				{
//					if(tempx ==  WIDNUMBER - 2 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
//					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else 
//				{
//					if(inCircle1(a + EDGELENGTH * 2,b - EDGELENGTH * 2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number+WIDNUMBER * 2 + 2<<" ";
//				}
//				if(tempx - 2 < 0)
//				{
//					if(tempx == 0 && tempy > CUFFLENGTH)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1<<" ";
//					else if(tempx == 1 && tempy > CUFFLENGTH)
//						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle1(a - EDGELENGTH * 2,b,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number-2<<" ";
//				}
//				if(tempx + 2 > WIDNUMBER - 1)
//				{
//					if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1<<" ";
//					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH)
//						out_file<<number -1 + HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle1(a + EDGELENGTH * 2,b,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + 2<<" ";
//				}
//				if(tempx-2 < 0 || tempy - 2 <0)
//				{
//					if(tempy == 0 && tempx >= 2)
//					{
//						if(inCircle2(a - EDGELENGTH * 2,b - EDGELENGTH,c + GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER -2<<" ";
//					}
//					else if(tempy == 1 && tempx >= 2)
//					{
//						if(inCircle2(a - EDGELENGTH * 2,b + EDGELENGTH,c + GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER -2<<" ";
//					}
//					else if(tempx == 0 && tempy > CUFFLENGTH + 2)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
//					else if(tempx == 1 && tempy > CUFFLENGTH + 2)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle1(a - EDGELENGTH * 2,b + EDGELENGTH * 2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number - WIDNUMBER * 2 - 2<<" ";
//				}
//				if(tempy-2<0)
//				{
//					if(tempy == 0)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					else if(tempy == 1)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle1(a ,b + EDGELENGTH * 2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number-WIDNUMBER * 2<<" ";
//				}
//				if(tempx+2 > WIDNUMBER - 1 || tempy-2<0)
//				{
//					if(tempy == 0 && tempx < WIDNUMBER - 2)
//					{
//						if(inCircle2(a + EDGELENGTH * 2,b - EDGELENGTH,c + GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number + HEIGHTNUMBER * WIDNUMBER + WIDNUMBER + 2<<" ";
//					}
//					else if(tempy == 1 && tempx <= WIDNUMBER - 2)
//					{
//						if(inCircle2(a + EDGELENGTH * 2,b + EDGELENGTH,c + GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number + HEIGHTNUMBER * WIDNUMBER - WIDNUMBER + 2<<" ";
//					}
//					else if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH + 2)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
//					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH + 2)
//						out_file<<number + HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle1(a + EDGELENGTH * 2,b + EDGELENGTH * 2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number - 2 * WIDNUMBER + 2<<" ";
//				}
//				number++;
//				out_file<<endl;
//			}
//		}
//	}
//	for(int tempy = 0;tempy < HEIGHTNUMBER ; tempy ++)
//	{
//		for(int tempx=0;tempx < WIDNUMBER;tempx ++)
//		{
//			if(inCircle2((float)tempx*EDGELENGTH + MODELX , MODELY - (float)tempy * EDGELENGTH , (float)MODELZ + GAPLENGTH))
//			{
//				number ++;
//			}
//			else
//			{
//				float a = (float)tempx*EDGELENGTH + MODELX;
//				float b = MODELY - (float)tempy * EDGELENGTH;
//				float c = (float)MODELZ + GAPLENGTH;
//				out_file<<number<<" ";
//				out_file<<(float)tempx*EDGELENGTH + MODELX <<" "<<MODELY - (float)tempy * EDGELENGTH <<" "<<(float)MODELZ + GAPLENGTH<<" ";
//				if(tempy < HEIGHTNUMBER-1)
//					out_file<<number+WIDNUMBER<<" ";
//				else if(tempy = HEIGHTNUMBER - 1)
//					out_file<<-1<<" ";
//				if(tempy > 0)
//				{
//					if(inCircle2(a ,b + EDGELENGTH,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number-WIDNUMBER<<" ";
//				}
//				else if(tempy == 0)
//					out_file<<number - HEIGHTNUMBER * WIDNUMBER<<" ";
//				if(tempx > 0)
//				{
//					if(inCircle2(a - EDGELENGTH ,b ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<< number - 1 <<" ";
//				}
//				else if(tempx == 0)
//				{
//					if(tempy > CUFFLENGTH)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				if(tempx < WIDNUMBER - 1)
//				{
//					if(inCircle2(a + EDGELENGTH ,b ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + 1<<" ";
//				}
//				else if(tempx==WIDNUMBER - 1)
//				{
//					if(tempy > CUFFLENGTH)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//
//
//
//				if(tempx == 0 || tempy == HEIGHTNUMBER - 1)
//				{
//					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx > 0 && tempy < HEIGHTNUMBER - 1)
//				{
//					if(inCircle2(a - EDGELENGTH ,b - EDGELENGTH ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + WIDNUMBER - 1<<" ";
//				}
//				if(tempx == WIDNUMBER - 1 || tempy == HEIGHTNUMBER - 1)
//				{
//					if(tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 1)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx < WIDNUMBER - 1 && tempy < HEIGHTNUMBER - 1)
//				{
//					if(inCircle2(a + EDGELENGTH ,b - EDGELENGTH ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + WIDNUMBER + 1<<" ";
//				}
//				if(tempx == 0 || tempy == 0)
//				{
//					if(tempy == 0 && tempx != 0)
//					{
//						if(inCircle1(a - EDGELENGTH ,b ,c - GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number - HEIGHTNUMBER * WIDNUMBER - 1<<" ";
//					}
//					else if(tempx == 0 && tempy > CUFFLENGTH + 1)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx > 0 && tempy > 0)
//				{
//					if(inCircle2(a - EDGELENGTH ,b + EDGELENGTH ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number-WIDNUMBER-1<<" ";
//				}
//				if(tempx == WIDNUMBER - 1 || tempy == 0)
//				{
//					if(tempy == 0 && tempx != WIDNUMBER - 1)
//					{
//						if(inCircle1(a + EDGELENGTH ,b ,c - GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1<<" ";
//					}
//					else if(tempx == WIDNUMBER && tempy > CUFFLENGTH + 1)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else if(tempx < WIDNUMBER - 1 && tempy>0)
//				{
//					if(inCircle2(a + EDGELENGTH ,b + EDGELENGTH ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number - WIDNUMBER + 1<<" ";
//				}
//
//
//				if(tempx - 2 < 0 || tempy + 2 > HEIGHTNUMBER - 1)
//				{
//					if(tempx == 0 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
//					else if(tempx == 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle2(a - EDGELENGTH*2 ,b - EDGELENGTH*2 ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + WIDNUMBER + WIDNUMBER - 2<<" ";
//				}
//				if(tempy + 2 > HEIGHTNUMBER - 1)
//				{
//					out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle2(a ,b - EDGELENGTH*2 ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number+2 * WIDNUMBER<<" ";
//				}
//				if(tempx + 2 > WIDNUMBER - 1 || tempy + 2 > HEIGHTNUMBER - 1)
//				{
//					if(tempx ==  WIDNUMBER - 2 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 + WIDNUMBER +WIDNUMBER<<" ";
//					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH && tempy < HEIGHTNUMBER - 2)
//						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER +WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else 
//				{
//					if(inCircle2(a + EDGELENGTH*2 ,b - EDGELENGTH*2 ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number+WIDNUMBER * 2 + 2<<" ";
//				}
//				if(tempx - 2 < 0)
//				{
//					if(tempx == 0 && tempy > CUFFLENGTH)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1<<" ";
//					else if(tempx == 1 && tempy > CUFFLENGTH)
//						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle2(a - EDGELENGTH*2 ,b ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number-2<<" ";
//				}
//				if(tempx + 2 > WIDNUMBER - 1)
//				{
//					if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1<<" ";
//					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH)
//						out_file<<number -1 - HEIGHTNUMBER * WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle2(a + EDGELENGTH*2 ,b ,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number + 2<<" ";
//				}
//				if(tempx-2 < 0 || tempy - 2 <0)
//				{
//					if(tempy == 0 && tempx >= 2)
//					{
//						if(inCircle1(a - EDGELENGTH*2 ,b - EDGELENGTH,c - GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER -2<<" ";
//					}
//					else if(tempy == 1 && tempx >= 2)
//					{
//						if(inCircle1(a - EDGELENGTH*2 ,b + EDGELENGTH,c - GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER -2<<" ";
//					}
//					else if(tempx == 0 && tempy > CUFFLENGTH + 2)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
//					else if(tempx == 1 && tempy > CUFFLENGTH + 2)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle2(a - EDGELENGTH*2 ,b + EDGELENGTH*2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number - WIDNUMBER * 2 - 2<<" ";
//				}
//				if(tempy-2<0)
//				{
//					if(tempy == 0)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER<<" ";
//					else if(tempy == 1)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle2(a ,b + EDGELENGTH*2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number-WIDNUMBER * 2<<" ";
//				}
//				if(tempx+2 > WIDNUMBER - 1 || tempy-2<0)
//				{
//					if(tempy == 0 && tempx < WIDNUMBER - 2)
//					{
//						if(inCircle1(a + EDGELENGTH*2 ,b - EDGELENGTH,c - GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number - HEIGHTNUMBER * WIDNUMBER + WIDNUMBER + 2<<" ";
//					}
//					else if(tempy == 1 && tempx <= WIDNUMBER - 2)
//					{
//						if(inCircle1(a + EDGELENGTH*2 ,b + EDGELENGTH,c - GAPLENGTH))
//							out_file<<-1<<" ";
//						else
//							out_file<<number - HEIGHTNUMBER * WIDNUMBER - WIDNUMBER + 2<<" ";
//					}
//					else if(tempx == WIDNUMBER - 2 && tempy > CUFFLENGTH + 2)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER + 1 - WIDNUMBER * 2 <<" ";
//					else if(tempx == WIDNUMBER - 1 && tempy > CUFFLENGTH + 2)
//						out_file<<number - HEIGHTNUMBER * WIDNUMBER - 1 - WIDNUMBER * 2<<" ";
//					else
//						out_file<<-1<<" ";
//				}
//				else
//				{
//					if(inCircle2(a + EDGELENGTH*2 ,b + EDGELENGTH*2,c))
//						out_file<<-1<<" ";
//					else
//						out_file<<number - 2 * WIDNUMBER + 2<<" ";
//				}
//				number++;
//				out_file<<endl;
//			}
//		}
//	}
//	return 0;
//}

//read file
//void inFileClothModel() {
//	ifstream in_file("ClothPoints.txt",ios::in);
//	if (!in_file) exit(-1);
//	for(int TEMP=0;TEMP<PTCAMT;TEMP++)
//	{
//		int num;
////		int xyz[3];
//		M3DVector3f xyz;
//		int structadj[4]; //structual constraint
//		int shearadj[4]; //Shear constraint
//		int bendadj[8]; //Bending constraint
//		in_file>>num;
////		printf("%d  ",num);
//		for(int temp=0;temp<3;temp++) 
//		{
//			in_file>>xyz[temp];
//		}
////		printf("%f %f %f\n",xyz[0],xyz[1],xyz[2]);
//		for(int temp=0;temp<4;temp++)
//		{
//			in_file>>structadj[temp];
//		}
////		printf("%d %d %d %d",structadj[0],structadj[1],structadj[2],structadj[3]);
//		for(int temp=0;temp<4;temp++)
//		{
//			in_file>>shearadj[temp];
//		}
//		for(int temp=0;temp<8;temp++)
//		{
//			in_file>>bendadj[temp];
//		}
//		initPtc(&ClothLnk[TEMP],num,Mass,xyz,structadj,shearadj,bendadj);
//	}
//}

