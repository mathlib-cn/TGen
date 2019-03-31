#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int jiou(int x)
{
  if(x%2==0)
    return 1;
  else
    return 0;
}

double computeULP(double y)
{
  double x=0,res=0,powermin=0,powermax=0,powermiddle=0;
  int expmin=0,expmax=0,expmiddle=0,jioupanduan=0;
  x= fabs(y);

  //printf("res=%.8e\n",pow(2,logb(x)+1-53));

  if(x<pow(2,-1021))
    res=pow(2,-1074);
  else if(x>(1-pow(2,-53))*pow(2,1024))
    res=pow(2,971);
  else
  {
    powermin=pow(2,-1021);
    expmin=-1021;
    powermax=pow(2,1024);
    expmax= 1024;
  }
  
  while(expmax - expmin >1)
  {
    jioupanduan=jiou(expmin+expmax);
    if(jioupanduan ==1)
      expmiddle=(expmax+expmin)/2;
    else
      expmiddle=(expmax+expmin+1)/2;

    powermiddle = pow(2,expmiddle);

    if(x>=powermiddle)
    {
      powermin = powermiddle;
      expmin = expmiddle;
    }
    else
    {
      powermax = powermiddle;
      expmax=expmiddle;
    }

    if(x==powermin)  
      res=pow(2,expmin-53);
    else
      res=pow(2,expmin-52);
  }

  return res;

}

