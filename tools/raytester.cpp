#include <iostream>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
using namespace std;




long double normalize(long double x, long double y, long double z)
{
   return sqrt(x*x+y*y+z*z);
}


/*
 * first test case a small sphere in front of a very large one
 */
void test1 (int width, int height, int u, int v,float p)
{
 
  long double x = (((2.0*u)-width)/width);
  long double y = (((2.0*v)-height)/height);
  long double norm = normalize(x,y,-1);
  long double z = -1/norm;
  x = x/norm;
  y = y/norm;

  long double tmp = (((2.0*(u+1))-width)/width);
  tmp = tmp/normalize(tmp,y,-1);
  //get size of pixel
  tmp = abs(x-tmp);

  //print sphere information
  cout.precision(17);
  cout<<"test1-p_"<<p<<"\n";
  cout<<"s1 location "<<x*20<<","<<y*20<<","<<z*20<<" color 1.0,0.0,0.0  radius 5\n";
  cout<<"s2 location "<<x*10<<","<<y*10<<","<<z*10<<" color 0.0,1.0,0  radius "<<tmp*p<<"\n";
  cout<<"end\n";
  

}

/*
 * second test case a distant sphere
 */
void test2 (int width, int height, int u, int v,float d)
{
  long double x = (((2.0*u)-width)/width);
  long double y = (((2.0*v)-height)/height);
  long double norm = normalize(x,y,-1);
  long double z = -1/norm;
  x = x/norm;
  y = y/norm;

  //print sphere information
  cout.precision(17);
  cout<<"test2-d_"<<d<<"\n";
  cout<<"s1 location "<<x*d<<","<<y*d<<","<<z*d<<"  color 0.0,1.0,0.0  radius 2\n";
  cout<<"end\n";
  

}


/*
 * third test colision with sphere edge
 */
void test3 (int width, int height, int u, int v,float p)
{
  long double x = (((2.0*u)-width)/width);
  long double y = (((2.0*v)-height)/height);
  long double norm = normalize(x,y,-1);
  long double z = -1/norm;
  x = x/norm;
  y = y/norm;
  
  long double tmp = (((2.0*(u+1))-width)/width);
  tmp = tmp/normalize(tmp,y,-1);
  //get size of pixel
  tmp = abs(x-tmp);

  //print sphere information
  cout.precision(17);
  cout<<"test3-p_"<<p<<"\n";
  cout<<"s1 location "<<x*100<<","<<y*100+25<<","<<z*100<<" color 0.0,1.0,0.0  radius "<<25+(tmp*p) <<"\n";
  cout<<"end\n";
  
}
/*
 * fourth test nested speres
 */
void test4 (int width, int height, int u, int v,float p)
{
  long double x = (((2.0*u)-width)/width);
  long double y = (((2.0*v)-height)/height);
  long double norm = normalize(x,y,-1);
  long double z = -1/norm;
  x = x/norm;
  y = y/norm;
  

  //print sphere information
  cout.precision(17);
  cout<<"test4-p_"<<p<<"\n";
  cout<<"s1 location "<<x*10<<","<<y*10<<","<<z*10<<"  color 1.0,0.0,0.0  radius "<<4 <<"\n";
  cout<<"s2 location "<<x*10<<","<<y*10<<","<<z*10<<"  color 0.0,1.0,0.0  radius "<<4-(4*p) <<"\n";
  cout<<"end\n";
  

}


/*
 * called with a single command argv 
 * check for user errors
 * generate a test case for a ray tracer
 * test is in clip space with a 45 degree fov for both x and y
 */
int main(int argc, char* argv[])
{
  
  //test for input arg
  if(argc < 4)
  {
    cout<<"invalid input\n";
    return 1;
  }


  //set width height and get u v cord
  int width = atoi(argv[1]);
  int height = width;
  srand(time(0));
  int u = rand() % (width/3) +(width/3);
  int v =  u;
  //used for generating fov and points if I add variable  fov
  //long double fovx = argv[2];
  //long double fovy = height/width * fovx;
  //long double x = ((2*u-width)/width) *tan(fovx);
  //long double y = ((2*v-height)/height) *tan(fovy);


  try{
    //start printing test info
    if(atoi(argv[2])==1)
    {
      cout<<u<<","<<height-v<<"\n";
      cout<<height<<endl;
      test1(width,height,u,v,atof(argv[3]));
    
      return 1;
    }
  
    
    else if(atoi(argv[2])==2)
    {
      cout<<u<<","<<height-v<<"\n";
      cout<<height<<endl;
      test2(width,height,u,v,atof(argv[3])); 

      return 1;
    }
  
    else if(atoi(argv[2])==3)
    {
      cout<<u<<","<<height-v<<"\n";
      cout<<height<<endl;
      test3(width,height,u,v,atof(argv[3]));
    
      return 1;
    }
   
    else if(atoi(argv[2])==4)
    {
      cout<<u<<","<<height-v<<"\n";
      cout<<height<<endl;
      test4(width,height,u,v,atof(argv[3]));
    
      return 1;
    }
    else if(*argv[2]=='m')
    {
        cout<<u<<","<<height-v<<"\n";
        cout<<height<<endl;
        int num = atoi(argv[3]);

        //first test
        long double value = num*5;
        for(int i = 0; i < num; i++)
        {
          test1(width,height,u,v,value);
          value = value/2;
        }


        //second test
        value = 100;
        long double stepsize = ((num*100)-100)/num;
        for(int i = 0; i < num; i++)
        {
          test2(width,height,u,v,value);
          value = value + stepsize;
        }


        //third test
        value = 0;
        stepsize = num/4;
        for(int i = 0; i < num; i++)
        {
          test3(width,height,u,v,value);
          value = value + stepsize;
        }

        //fourth test
        value = 0;
        stepsize = num*2;
        for(int i = 0; i < num; i++)
        {
          test4(width,height,u,v,1/value);
          value = value + stepsize;
        }


        return 1;
    }
  }catch(const std::exception& e){
    cout<<"invalid input\n";
    return 1;
  }

}


