#include <NullSpaceLib.h>
#include <U8g2lib.h>

#include <Adafruit_NeoPixel.h>
/********* ISTANZA CLASSE DISPLAY OLED I2C SH1106 128X64 ************/
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);



/********* LEDS NEO-PIXELS ************/
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, 5, NEO_GRB + NEO_KHZ800);

class Heart
{
  private:
    U8G2_SH1106_128X64_NONAME_F_HW_I2C* _u8g2;
    float scale;
    float mult;
    float xrot;
    int startt;
    TimerC* t_scale;
    TimerC* t_draw;
    Point<float> vPoints[360] ;
    bool first_run;
    PinHoleCamera* camera;
    Cube* cubemodel;
    Cube1* cube1model;
    Logo3DNSP* logo_nsp;
    Icospere* icospheremodel;
    float spincube;
    float spinlogo;
    int spinsign;
    float cube_oscillation;
    float oscillation_freq;
    float v_max;
    float u_max;
    float z_rain;
  public:
    TimerC* t_draw3;
    
  public:
    Heart(U8G2_SH1106_128X64_NONAME_F_HW_I2C* u8g2_)
    {
      _u8g2 = u8g2_;
      scale=1;
      mult=1.1;
      xrot=0;
      t_scale = new TimerC();
      t_draw = new TimerC();
      t_draw3 = new TimerC();
      t_scale->start();
      t_draw->start();
      t_draw3->start();
      startt=0;
      first_run=true;
      camera = new PinHoleCamera;
      cubemodel = new Cube;
      cube1model = new Cube1;
      logo_nsp = new Logo3DNSP;
      icospheremodel = new Icospere;
      cubemodel->Scale(2.0);
      cube1model->Scale(2.0);
      logo_nsp->Scale(2.2);
      icospheremodel->Scale(3.1);
      spincube=0;
      spinlogo=0;
      spinsign=1;
      cube_oscillation=0;
      oscillation_freq=0;
      v_max=0;
      u_max=0;
      z_rain=0;
      randomSeed(analogRead(0));
    }

    void applyRotZ(float angledeg, float& x, float& y)
    {
      float ang = angledeg*PI/180.0;
      float xp = cos(ang)*x - sin(ang)*y;
      float yp = sin(ang)*x + cos(ang)*y;

      x=xp;
      y=yp;
    }

    void applyRotX(float angledeg, float& x, float& y, float& z)
    {
      float ang = angledeg*PI/180.0;
      float xp = x;
      float yp = cos(ang)*y - sin(ang)*z;
      float zp = sin(ang)*y + cos(ang)*z;

      x=xp;
      y=yp;
      z=zp;
    }

    bool draw1(bool& end)
    {
      camera->setFocalLength(6);
       bool refresh=false;
       end=false;
        for (int t=0; t<xrot; ++t) 
        {
          float ang = t*PI/180.0;
          float x = scale * (16*sin(ang)*sin(ang)*sin(ang))  ;
		      float y = scale * (13*cos(ang) - 5*cos(2*ang) - 2*cos(3*ang) - cos(4*ang))  ;
          float z = 0;

          applyRotZ(-90, x ,y );
          //applyRotX(xrot, x, y, z);
          _u8g2->drawPixel(x+64,y+38);//32);
        }
        if (scale>1.5)
        {
          mult = 0.9;
        }
        else if(scale<1)
        {
          mult=1.1;
        }
        if(t_scale->getET()>150)
        {
          t_scale->reset();
          scale = scale*mult;
          refresh=true;
        }

        if(t_draw->getET()>5)
        {
          t_draw->reset();
          xrot+=5;
          refresh=true;
        }
               
        if (xrot>360)
        {
          xrot=0;
          end=true;
        }
        else if (xrot<-360)
        {
          xrot=0;
          end=true;
        }
        return refresh;
	
    }

    bool draw2(bool& end)
    {
      camera->setFocalLength(6);
       bool refresh=false;
       end=false;
        for (int t=startt; t<340+startt; ++t) 
        {
          float ang = t*PI/180.0;
          float x = scale * (16*sin(ang)*sin(ang)*sin(ang))  ;
		      float y = scale * (13*cos(ang) - 5*cos(2*ang) - 2*cos(3*ang) - cos(4*ang))  ;
          float z = 0;

          applyRotZ(-90, x ,y );
          //applyRotX(xrot, x, y, z);
          _u8g2->drawPixel(x+64,y+38);
        }
        if (scale>1.5)
        {
          mult = 0.9;
        }
        else if(scale<1)
        {
          mult=1.1;
        }
        if(t_scale->getET()>150)
        {
          t_scale->reset();
          scale = scale*mult;
          refresh=true;
        }

        if(t_draw->getET()>5)
        {
          t_draw->reset();
          startt+=5;
          refresh=true;
        }
               
        if(startt>340)
        {
          startt=0;
          end=true;
        }
        return refresh;
	
    }

    bool draw3(bool& end)
    {
      camera->setFocalLength(6);
       bool refresh=false;
       end=false;
        for (int t=0; t<360; ++t) 
        {
          float ang = t*PI/180.0;
          float x = scale * (16*sin(ang)*sin(ang)*sin(ang))  ;
		      float y = scale * (13*cos(ang) - 5*cos(2*ang) - 2*cos(3*ang) - cos(4*ang))  ;
          float z = 0;

          applyRotZ(-90, x ,y );
          //applyRotX(xrot, x, y, z);
          _u8g2->drawPixel(x+64,y+38);
        }
        if (scale>1.5)
        {
          mult = 0.9;
        }
        else if(scale<1)
        {
          mult=1.1;
        }
        if(t_scale->getET()>150)
        {
          t_scale->reset();
          scale = scale*mult;
          refresh=true;
        }

        if(t_draw->getET()>5)
        {
          t_draw->reset();
          refresh=true;
        }
               
        if(t_draw3->getET()>3000)
        {
          t_draw3->reset();
          end=true;
        }
        return refresh;
	
    }

    bool draw4(bool& end)
    {
      camera->setFocalLength(6);
       bool refresh=false;
       end=false;
       if(first_run)
       {
        first_run=false;
        for (int t=0; t<360; ++t) 
        {
          float ang = t*PI/180.0;
          float x = scale * (16*sin(ang)*sin(ang)*sin(ang))  ;
          float y = scale * (13*cos(ang) - 5*cos(2*ang) - 2*cos(3*ang) - cos(4*ang))  ;
          float z = 0;

          vPoints[t].vx = random(-1000, 1000)/1000.0;
          vPoints[t].vy = random(-1000, 1000)/1000.0;
          
          vPoints[t].x=x;
          vPoints[t].y=y;
          vPoints[t].nextStep();
          x=vPoints[t].x;
          y=vPoints[t].y;
          applyRotZ(-90, x ,y );
          //applyRotX(xrot, x, y, z);
          _u8g2->drawPixel(x+64,y+38);
        }
       }
       else
       {
        for (int t=0; t<360; ++t) 
        {
          //vPoints[t].x += random(-2000, 2000)/1000.0;
          //vPoints[t].y += random(-2000, 2000)/1000.0;
          vPoints[t].nextStep();
          float x = vPoints[t].x;
          float y = vPoints[t].y;
          applyRotZ(-90, x ,y );
          //applyRotX(xrot, x, y, z);
          _u8g2->drawPixel(x+64,y+38);
        }
       }
     

        if(t_draw->getET()>5)
        {
          t_draw->reset();
          refresh=true;
        }
               
        if(t_draw3->getET()>4000)
        {
          t_draw3->reset();
          first_run=true;
          end=true;
          for (int t=0; t<360; ++t) 
          {
            vPoints[t].Reset();
          }
        }
        return refresh;
	
    }

    bool drawCube(bool& end)
    {
      camera->setFocalLength(6);
      bool refresh=false;
      end=false;
      Point<float> vertici_uv[8];
      for (int t=0; t<8; ++t) 
      {
        Point<float> xyz_out;
        float u,v;
        camera->RotoTraslation(0,spincube,-30,cube_oscillation,0.3,8.5,cubemodel->vert[t],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;
        vertici_uv[t].x=u;
        vertici_uv[t].y=v;
        //_u8g2->drawPixel(u, v);
        //Serial.println("x: " +String(vertici_uv[t].x,1)+ " y: "+String(vertici_uv[t].y,1));
       
      }
      //Serial.println("-------");
      

      _u8g2->drawLine(vertici_uv[4].x, vertici_uv[4].y, vertici_uv[0].x, vertici_uv[0].y);
      _u8g2->drawLine(vertici_uv[4].x, vertici_uv[4].y, vertici_uv[5].x, vertici_uv[5].y);
      _u8g2->drawLine(vertici_uv[4].x, vertici_uv[4].y, vertici_uv[6].x, vertici_uv[6].y);
      _u8g2->drawLine(vertici_uv[5].x, vertici_uv[5].y, vertici_uv[7].x, vertici_uv[7].y);


      _u8g2->drawLine(vertici_uv[5].x, vertici_uv[5].y, vertici_uv[1].x, vertici_uv[1].y);
      _u8g2->drawLine(vertici_uv[1].x, vertici_uv[1].y, vertici_uv[3].x, vertici_uv[3].y);
      _u8g2->drawLine(vertici_uv[1].x, vertici_uv[1].y, vertici_uv[0].x, vertici_uv[0].y);
      _u8g2->drawLine(vertici_uv[0].x, vertici_uv[0].y, vertici_uv[2].x, vertici_uv[2].y);


      _u8g2->drawLine(vertici_uv[2].x, vertici_uv[2].y, vertici_uv[3].x, vertici_uv[3].y);
      _u8g2->drawLine(vertici_uv[2].x, vertici_uv[2].y, vertici_uv[6].x, vertici_uv[6].y);
      _u8g2->drawLine(vertici_uv[6].x, vertici_uv[6].y, vertici_uv[7].x, vertici_uv[7].y);
      _u8g2->drawLine(vertici_uv[7].x, vertici_uv[7].y, vertici_uv[3].x, vertici_uv[3].y);
   

       
      if(t_draw->getET()>5)
      {
        t_draw->reset();

        spincube+=spinsign*3;
        if(spincube>358)
        {
          spinsign*=-1;
          
        }
        else if(spincube<-358)
        {
          spinsign*=-1;
          
        }
        cube_oscillation = 0+1.5*sin(oscillation_freq);
        oscillation_freq+=0.1;
        refresh=true;
      }
              
      if(t_draw3->getET()>10000)
      {
        t_draw3->reset();
        end=true;
      }
      return refresh;
	
    }

    bool drawCube1(bool& end)
    {
      camera->setFocalLength(6);
      bool refresh=false;
      end=false;
      Point<float> xyz_out;
      float u,v;
      Triangle<float> tri;
      for (int t=0; t<N_FACE_Cube1; ++t) 
      {
        
        camera->RotoTraslation(0,spincube,-30,cube_oscillation,0.3,8.5,cube1model->vert[cube1model->faces[t].x],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;

        tri.v1.x = u;
        tri.v1.y = v;

        camera->RotoTraslation(0,spincube,-30,cube_oscillation,0.3,8.5,cube1model->vert[cube1model->faces[t].y],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;
        tri.v2.x = u;
        tri.v2.y = v;

        camera->RotoTraslation(0,spincube,-30,cube_oscillation,0.3,8.5,cube1model->vert[cube1model->faces[t].z],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;
        tri.v3.x = u;
        tri.v3.y = v;

        _u8g2->drawLine(tri.v1.x, tri.v1.y, tri.v2.x, tri.v2.y);
        _u8g2->drawLine(tri.v2.x, tri.v2.y, tri.v3.x, tri.v3.y);
        _u8g2->drawLine(tri.v3.x, tri.v3.y, tri.v1.x, tri.v1.y);
        
       
      }

      if(t_draw->getET()>5)
      {
        t_draw->reset();

        spincube+=spinsign*3;
        if(spincube>358)
        {
          spinsign*=-1;
          
        }
        else if(spincube<-358)
        {
          spinsign*=-1;
          
        }
        cube_oscillation = 0+1.5*sin(oscillation_freq);
        oscillation_freq+=0.1;
        refresh=true;
      }
              
      if(t_draw3->getET()>10000)
      {
        t_draw3->reset();
        end=true;
      }
      return refresh;
	
    }

     bool drawIcospere(bool& end)
    {
      camera->setFocalLength(6);
      bool refresh=false;
      end=false;
      Point<float> xyz_out;
      float u,v;
      Triangle<float> tri;
      for (int t=0; t<N_FACE_Icospere; ++t) 
      {
        
        camera->RotoTraslation(0,spincube,-30,cube_oscillation,0.3,8,icospheremodel->vert[icospheremodel->faces[t].x],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;

        tri.v1.x = u;
        tri.v1.y = v;

        camera->RotoTraslation(0,spincube,-30,cube_oscillation,0.3,8,icospheremodel->vert[icospheremodel->faces[t].y],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;
        tri.v2.x = u;
        tri.v2.y = v;

        camera->RotoTraslation(0,spincube,-30,cube_oscillation,0.3,8,icospheremodel->vert[icospheremodel->faces[t].z],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;
        tri.v3.x = u;
        tri.v3.y = v;

        _u8g2->drawLine(tri.v1.x, tri.v1.y, tri.v2.x, tri.v2.y);
        _u8g2->drawLine(tri.v2.x, tri.v2.y, tri.v3.x, tri.v3.y);
        _u8g2->drawLine(tri.v3.x, tri.v3.y, tri.v1.x, tri.v1.y);
        
       
      }

      if(t_draw->getET()>5)
      {
        t_draw->reset();

        spincube+=spinsign*3;
        if(spincube>358)
        {
          spinsign*=-1;
          
        }
        else if(spincube<-358)
        {
          spinsign*=-1;
          
        }
        cube_oscillation = 0+1.5*sin(oscillation_freq);
        oscillation_freq+=0.1;
        refresh=true;
      }
              
      if(t_draw3->getET()>10000)
      {
        t_draw3->reset();
        end=true;
      }
      return refresh;
	
    }

    bool drawLogo3D(bool& end, bool wireframe=false)
    {
      camera->setFocalLength(4);
      bool refresh=false;
      end=false;
      Point<float> xyz_out;
      float u,v;
      Triangle<float> tri;
      for (int t=0; t<N_FACE_Logo3DNSP; ++t) 
      {
        camera->RotoTraslation(90,0,spinlogo,0,0.4,6,logo_nsp->vert[logo_nsp->faces[t].x],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        tri.v1.x = u;
        tri.v1.y = v;

        camera->RotoTraslation(90,0,spinlogo,0,0.4,6,logo_nsp->vert[logo_nsp->faces[t].y],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        tri.v2.x = u;
        tri.v2.y = v;

        camera->RotoTraslation(90,0,spinlogo,0,0.4,6,logo_nsp->vert[logo_nsp->faces[t].z],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        tri.v3.x = u;
        tri.v3.y = v;

        if(wireframe)
        {
          _u8g2->drawLine(tri.v1.x, tri.v1.y, tri.v2.x, tri.v2.y);
          _u8g2->drawLine(tri.v2.x, tri.v2.y, tri.v3.x, tri.v3.y);
          _u8g2->drawLine(tri.v3.x, tri.v3.y, tri.v1.x, tri.v1.y);
        }
        else
        {
          _u8g2->drawTriangle(tri.v1.x, tri.v1.y, tri.v2.x, tri.v2.y, tri.v3.x, tri.v3.y);
        }
        
      }
      /*
      for (int t=0; t<N_VERT_Logo3DNSP; ++t) 
      {
        
        camera->RotoTraslation(90,0,spinlogo,0,0,6,logo_nsp->vert[t],xyz_out);
        camera->ProjectK(u, v, xyz_out);
        
        if(u>127)
          u=127;
        else if(u<0)
          u=0;
        if(v>63)
          v=63;
        else if(v<0)
          v=0;

        _u8g2->drawPixel(u, v);
        //Serial.println("x: " +String(vertici_uv[t].x,1)+ " y: "+String(vertici_uv[t].y,1));
       
      }
      */
      //Serial.println("-------");
      
      if(t_draw->getET()>5)
      {
        t_draw->reset();
        spinlogo+=3;
        if(spinlogo>360)
        {
          spinlogo = 0;
        }
        refresh=true;
      }
              
      if(t_draw3->getET()>15000)
      {
        t_draw3->reset();
        end=true;
      }
      return refresh;
	
    }

    bool draw5(bool& end, float pitch=180)
    {
      camera->setFocalLength(6);
      bool refresh=false;
      Point<float> xyz_out;
      Point<float> xyz_in;
      float u,v;
       end=false;
        for (int vi=0; vi<v_max; vi+=10) 
        {
          for (int ui=0; ui<u_max; ui+=10)
          {
            float angv = vi*PI/180.0;
            float angu = ui*PI/180.0;
            xyz_in.x = sin(angv)*(15*sin(angu)-4*sin(3*angu))/10.0;
            xyz_in.y = 8*cos(angv)/10.0;
            xyz_in.z = sin(angv)*(15*cos(angu)-5*cos(2*angu)-2*cos(3*angu)-cos(2*angu))/10.0;
            camera->RotoTraslation(90,pitch,90,0,0,8,xyz_in,xyz_out);
            camera->ProjectK(u, v, xyz_out);
            _u8g2->drawPixel(u,v);
            /*
            Serial.print(vi);Serial.print(" ");
            Serial.print(ui);Serial.print(" ");
            Serial.print(u);Serial.print(" ");
            Serial.println(v);Serial.println("------------- ");
            */
          }        
          
        }
        

        if(t_draw->getET()>5)
        {
          t_draw->reset();
          u_max+=5;
          v_max+=5;
          if(v_max>180)
          {
            v_max=180;
          }
          if(u_max>360)
          {
            u_max=360;
          }
          refresh=true;
        }

        if(v_max>=180 && u_max>=360)
        {
          v_max=0;
          u_max=0;
          end=true;
        }
               
        return refresh;
	
    }

    bool draw6(bool& end, bool& isfirst, Point<float>* xyz_in)
    {
      camera->setFocalLength(6);
      bool refresh=false;
      Point<float> xyz_out;
      float u,v;
      end=false;
      if(isfirst)
      {
          for(int t=0;t<300;++t)
          {
              xyz_in[t].x = random(-2000,2000)/2000.0;
              xyz_in[t].y = random(-2000,2000)/2000.0;
              xyz_in[t].z = random(-10000,0)/1000.0;
              camera->RotoTraslation(0,0,0,0,0,z_rain,xyz_in[t],xyz_out);
              camera->ProjectK(u, v, xyz_out);
              _u8g2->drawPixel(u,v);
          }
          isfirst=false;
      }
      else
      {
        for(int t=0;t<300;++t)
        {
          camera->RotoTraslation(0,0,0,0,0,z_rain,xyz_in[t],xyz_out);
          camera->ProjectK(u, v, xyz_out);
          _u8g2->drawPixel(u,v);
        }
        for(int t=0;t<40;++t)
        {
          camera->RotoTraslation(0,0,0,0,0,z_rain,xyz_in[t],xyz_out);
          camera->ProjectK(u, v, xyz_out);

          Point<float> xyz_in2, xyz_out2;
          xyz_in2.x=xyz_in[t].x;
          xyz_in2.y=xyz_in[t].y;
          xyz_in2.z=xyz_in[t].z-0.5;
          float u2,v2;
          camera->RotoTraslation(0,0,0,0,0,z_rain,xyz_in2,xyz_out2);
          camera->ProjectK(u2, v2, xyz_out2);
          /*
          if(u>127)
            u=127;
          else if(u<0)
            u=0;
          if(v>63)
            v=63;
          else if(v<0)
            v=0;
          if(u2>127)
            u2=127;
          else if(u2<0)
            u2=0;
          if(v2>63)
            v2=63;
          else if(v2<0)
            v2=0;
          */

          if(u>127 || u<0 || v>63 || v<0 || u2>127 || u2<0 || v2>63 || v2<0)
          {
            continue;
          }
          _u8g2->drawLine(u,v,u2,v2);
        }
      }

    
      if(t_draw->getET()>5)
      {
        t_draw->reset();
        z_rain+=0.05;
        refresh=true;
      }
      if(t_draw3->getET()>12000)
      {
        t_draw3->reset();
        isfirst=true;
        z_rain=0;
        end=true;
      }
      return refresh;
    }


};

Heart* heart = new Heart(&u8g2);

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.setBitmapMode(1); //set the background to transparant
  strip.begin();
  strip.setBrightness(50); //[0-255]
  strip.clear(); // metti ad off tutti i leds
  strip.setPixelColor(0, strip.Color(204,0,204)); 
  //strip.setPixelColor(0, strip.Color(153,0,153));
  strip.show();
}


bool end=false;
bool refresh=false;
int state=0;
float pitch=180;
bool isfirst=true;
Point<float> xyz_in[300];
void loop() {
   
  u8g2.clearBuffer();
  switch(state)
  {
    case 0:
    {
      //refresh = heart->drawCube(end);
      //refresh = heart->drawCube1(end);
      //refresh = heart->drawLogo3D(end);
      /*
      refresh = heart->draw5(end, pitch);
      if(end)
      {
        pitch+=5;
      }
      */
      /*
      refresh = heart->draw6(end,isfirst,&xyz_in[0]);
      if(end)
      {
        heart->t_draw3->reset();
      }
      */
      state=100;
      break;
    }
    case 100:
    {
      refresh = heart->draw1(end);
      if(end)
        state=200;
      break;
    }
    case 200:
    {
      refresh = heart->draw2(end);
      if(end)
      {
        heart->t_draw3->reset();
        state=300;
      }
        
      break;
    }
    case 300:
    {
      refresh = heart->draw3(end);
      if(end)
      {
        heart->t_draw3->reset();
        state=400;
      }
      break;
    }
    case 400:
    {
      refresh = heart->draw4(end);
      if(end)
      {
        heart->t_draw3->reset();
        state=450;
      }
      break;
    }
    case 450:
    {
      refresh = heart->draw6(end,isfirst,&xyz_in[0]);
      if(end)
      {
        heart->t_draw3->reset();
        state=500;
      }
      break;
    }
    case 500:
    {
      refresh = heart->drawCube(end);
      if(end)
      {
        heart->t_draw3->reset();
        state=600;
      }
      break;
    }
    case 600:
    {
      refresh = heart->drawCube1(end);
      if(end)
      {
        heart->t_draw3->reset();
        state=650;
      }
      break;
    }
    case 650:
    {
      refresh = heart->drawIcospere(end);
      if(end)
      {
        heart->t_draw3->reset();
        state=700;
      }
      break;
    }
    case 700:
    {
      refresh = heart->drawLogo3D(end, false);
      if(end)
      {
        heart->t_draw3->reset();
        state=800;
      }
      break;
    }
    case 800:
    {
      refresh = heart->drawLogo3D(end, true);
      if(end)
      {
        heart->t_draw3->reset();
        state=100;
      }
      break;
    }
  }
  //u8g2.setFont(u8g2_font_ncenB14_tr);
  //u8g2.drawStr(4,16,String(state).c_str());
  if (refresh)
    u8g2.sendBuffer();
  delay(1);

}
