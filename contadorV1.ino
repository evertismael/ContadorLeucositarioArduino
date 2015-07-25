// Version 1.0 del contador Leucocitario
// 2015.07.23
// se muestra el funcionamiento del prototipo para 4 tipos diferentes de celulas.

#include <UTFT.h>
#include <UTouch.h>
// Declarando nuestro tamaños de letras (fonts)
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

UTFT myGLCD(ITDB28,38,39,40,41);
UTouch  myTouch( 6, 5, 4, 3, 2);

int num=3;
String el[] = {"Conteo","Estadistic","Ayuda"};
int numcels=7;
String cel[]={"LINFOCITOS..","BASOFILOS...","MONOCITOS...","NEUTROFILOS.","EOSINOFILOS.","LINFOCITOS T","LINFOCITOS B"};
int contCel[]={10,15,15,20,10,5,10,20,0,0}; // array de contadores
// coordenadas del touch
int x, y;



int viewState = -1; // la vista principal es 0; vista de conteo es 1.

void headerFooter()
{
  // dibujando bordes superior e inferior
  myGLCD.setColor(23,19,83);
  myGLCD.fillRect(0,0,319,15);
  myGLCD.fillRect(0,224,319,239);
  // rellenando los datos.
  myGLCD.setBackColor(23,19,83);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("CONTADOR CELULAR",CENTER,2);
  myGLCD.print("EPC-Developers: evertismael@gmail.com",CENTER,225);
}
void navigator(String elementos[],int numElementos)
{
  // despliega botones en la parte izquierda de la pantalla para la navegacion.
  myGLCD.setBackColor(13,85,156);
  for(int i=0;i<numElementos;i++)
  {
    myGLCD.setColor(13,85,156);
    myGLCD.fillRoundRect(17+i*100,20,115+i*100,50);
    myGLCD.setColor(255,255,255);
    myGLCD.drawRoundRect(18+i*100,21,116+i*100,48);
  }
  myGLCD.setColor(0,0,0);
  myGLCD.setFont(SmallFont);
  for(int i=0;i<numElementos;i++)
  {
    myGLCD.print(elementos[i],30+i*100,27);
  }
}
void conteoView(String celulas[],int numcel,int conCel[])
{ String mensaje;
  // esta es la funcion que despliega la vista principal.
  myGLCD.setColor(200,80,20);
  myGLCD.fillRect(1,18,318,221);
  myGLCD.setBackColor(200,80,20);
  myGLCD.setColor(0,0,0);
  myGLCD.setFont(SmallFont);
  int total=0;
  for(int i=0; i<numcel;i++)
  {
    mensaje=String(i+1)+":"+celulas[i] + ".: " + String(conCel[i]);
    myGLCD.print(mensaje,50,i*14+100);
    total+=conCel[i];
  }
  mensaje= "TOTAL: " + String(total);
  myGLCD.print(mensaje,150,198);
  myGLCD.setFont(BigFont);
  myGLCD.print("- CONTEO -",CENTER,60);
}

void estadistView(String celulas[], int contCel[],int numcelulas)
{
  String mensaje;
  // esta es la funcion que despliega la vista principal.
  myGLCD.setColor(200,80,20);
  myGLCD.fillRect(1,18,318,221);
  myGLCD.setBackColor(200,80,20);
  myGLCD.setColor(0,0,0);  
  myGLCD.setFont(BigFont);
  myGLCD.print("RESULTADOS",30,60);
  myGLCD.setFont(SmallFont);
  float total=0;
  for(int i=0; i<numcelulas;i++)
  {
    total+=contCel[i];
  }
  int h=250,k=120,r=60;
  // empezamos a dibujar la torta;
  float percent=0,m1=0,m2=0;
  int cuadrante=1,sig=-1;
  
  for(int q=0;q<numcelulas;q++)   // por cada celula
  {
    //para el color:
    switch(q)
    {
      case 0:  myGLCD.setColor(200,0,0);
               break;
      case 1:  myGLCD.setColor(0,0,200);
               break;
      case 2:  myGLCD.setColor(0,200,0);
               break;
      case 3:  myGLCD.setColor(220,220,0);
               break;
      case 4:  myGLCD.setColor(220,0,220);
               break;
      case 5:  myGLCD.setColor(0,220,220);
               break;
      case 6:  myGLCD.setColor(0,0,0);
               break;
      case 7:  myGLCD.setColor(255,255,255);
               break;
      case 8:  myGLCD.setColor(100,0,100);
               break;
    }
    percent+=float(float(contCel[q])/total);
    m2=tan(percent*2*PI);
    sig*=-1;
    String mm=String(q+1) + ": " + celulas[q]+ " : " + String(int(ceil(contCel[q]*100/total))) + "%";
    myGLCD.print(mm,10,100+14*q);
    if(((m1>=0 && m2>=0)||(m1<0 && m2<0))&&(percent<0.98))
    {
      switch(cuadrante)
      {
        case 1: cuadrante = cuadrante_1_4(cuadrante,h,k,r,m1,m2);
                break;
        case 2: cuadrante = cuadrante_2_3(cuadrante,h,k,r,m1,m2);
                break;
        case 3: cuadrante = cuadrante_2_3(cuadrante,h,k,r,m1,m2);
                break;
        case 4: cuadrante = cuadrante_1_4(cuadrante,h,k,r,m1,m2);
                break;
        default: break;
      }
    }
    else
    {
      switch(cuadrante)
      {
        case 1: cuadrante = cuadrante_1_4(cuadrante,h,k,r,m1,99999);
                myGLCD.drawLine(h,k,h,k+r);
                cuadrante = cuadrante_2_3(cuadrante,h,k,r,-99999,m2);
                break;
        case 2: cuadrante = cuadrante_2_3(cuadrante,h,k,r,m1,-0.0001);
                myGLCD.drawLine(h-r,k,h,k);
                cuadrante = cuadrante_2_3(cuadrante,h,k,r,0.002,m2);
                break;
        case 3: cuadrante = cuadrante_2_3(cuadrante,h,k,r,m1,99999);
                myGLCD.drawLine(h,k-r,h,k);
                cuadrante = cuadrante_1_4(cuadrante,h,k,r,-99999,m2);
                break;
        case 4: cuadrante = cuadrante_1_4(cuadrante,h,k,r,m1,-0.0001);
                break;
        default: break;
      }
    }
    m1=m2;
  }
    myGLCD.drawCircle(h,k,r);
}

int cuadrante_1_4(int cuadrante,int h, int k, int r, float m1, float m2)
{
  float d1=0,d2=0,rr=0;
  int x,y;
  if(cuadrante==1)
  {
    for(int i=h;i<=r+h;i++)
    {
      for(int j=k;j<=r+k;j++)
      {
        x=(i-h);
        y=(j-k);
        d1=(m1*x-y)*(-1);
        d2=(m2*x-y)*(-1);
        rr=x*x+y*y;
        if(d1>=0 && d2<=0 && rr<r*r)
        {
          myGLCD.drawPixel(i,j);
        }
      }
    }
    if(m2>99995)
    {
      return 2;
    }
  }
  if(cuadrante==4)
  {
    for(int i=h;i<=r+h;i++)
    {
      for(int j=-r+k;j<=k;j++)
      {
        x=(i-h);
        y=(j-k);
        d1=(m1*x-y)*(-1);
        d2=(m2*x-y)*(-1);
        rr=x*x+y*y;
        if(d1>=0 && d2<=0 && rr<r*r)
        {
          myGLCD.drawPixel(i,j);
        }
      }
    }
    if(m2>-0.001)
    {
    return 1;
    }
  }
  return cuadrante;
}

int cuadrante_2_3(int cuadrante,int h, int k, int r, float m1, float m2)
{
  float d1=0,d2=0,rr=0;
  int x,y;
  if(cuadrante==2)
  {
    for(int i=h-r;i<=h;i++)
    {
      for(int j=k;j<=r+k;j++)
      {
        x=(i-h);
        y=(j-k);
        d1=(m1*x-y)*(-1);
        d2=(m2*x-y)*(-1);
        rr=x*x+y*y;
        if(d1<=0 && d2>=0 && rr<r*r)
        {
          myGLCD.drawPixel(i,j);
        }
      }
    }
    if(m2>-0.01)
    {
      return 3;
    }
  }
  if(cuadrante==3)
  {
    for(int i=h-r;i<=h;i++)
    {
      for(int j=k-r;j<=k;j++)
      {
        x=(i-h);
        y=(j-k);
        d1=(m1*x-y)*(-1);
        d2=(m2*x-y)*(-1);
        rr=x*x+y*y;
        if(d1<=0 && d2>=0 && rr<r*r)
        {
          myGLCD.drawPixel(i,j);
        }
      }
    }
    if(m2>9999)
    {
    return 4;
    }
  }
  return cuadrante;
}





void setup()
{
  myGLCD.InitLCD();
  myGLCD.clrScr();
  Serial.begin(115200);
  // variables del touch
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);
  headerFooter();
  navigator(el,3);

}
void loop()
{
  while(1)
  {
    if(myTouch.dataAvailable())
    {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
      //
      Serial.print(x);
      Serial.print(" ; ");
      Serial.println(y);
      //dentro del area del navigator.
      if(y>190 && y<210)
      {
        if(x>18 && x<110) // boton de conteo.
        {
          if(viewState!=0) // se redibuja el conteo siempre y cuando este en una vista diferente
          {
            conteoView(cel,numcels,contCel);
            navigator(el,num);
            viewState=0;
          }
        }
        if(x>123 && x<212) // boton de estadisticas
        {
         if(viewState!=1)  // se redibuja las estadisticas siempre y cuando este en una vista diferente.
         {
           estadistView(cel, contCel,numcels);
           navigator(el,num);
           viewState=1;
         }
        }
        if(x>225 && x<317) // boton de ayuda
        {
          
        }
      }
      
      
      
    }
  }
}

