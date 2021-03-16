#include "graphics.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace genv;
using namespace std;

struct surface {
  std::vector<int> s;
  void generate() {
    for (size_t i = 0; i < 1280; i+=100) {
      s.push_back(400 + ( std::rand() % ( 719 - 400 + 1 ) ));
    }
    s[5] = s[6];
    s.push_back(719);
  }
  void draw() {
    int j = 0;
    gout << move_to(0,719);
    for (size_t i = 0; i < 1280; i+=100) {
      gout << color(255,255,255);
      if (i==600) {
        gout << color(0,255,0);
      }
      gout << line_to(i,s[j]);
      j++;
    }
    gout << line_to(1279,719);
  }
  std::vector<int> bounce_back_radar_waves() {
    return s;
  }
};

struct phys_vector {
  int x = 0;
  int y = 0;
};

phys_vector calc_resultant(std::vector<phys_vector> s) {
  phys_vector v;
  for (size_t i = 0; i < s.size(); i++) {
    v.x += s[i].x;
    v.y += s[i].y;
  }
  return v;
}

class lander {
  private: int pos_x=100;
  int pos_y=100;
  std::vector<int> s;
  phys_vector speed;
  public:void force_act(phys_vector v) {
    speed.x+=v.x;
    speed.y+=v.y;
  }
  public:void update_speed() {
    pos_x+=speed.x;
    pos_y+=speed.y;
  }
  public:void draw() {
    gout << move_to(0,0);
    gout << color(0,0,0);
    gout << box_to(1279,719);
    gout << color(0,255,0);
    gout << move_to(pos_x-10,pos_y-10);
    gout << box(5,5);

  }
 public:void radar_scan(surface moon) {
   s = moon.bounce_back_radar_waves();
 }
 public:bool detect_collision() {
   int x1 = (pos_x/100)*100;
   int y1 = s[pos_x/100];

   int x2 = ((pos_x/100)+1)*100;
   int y2 = s[pos_x/100+1];
   float m = ((float)y2-(float)y1) / ((float)x2-(float)x1);
   float b = y2-m*x2;
   if ((pos_y)-(m*pos_x+b)>5) {
     return true;
   }
   if ((pos_y)-(m*(pos_x-5)+b)>5) {
     return true;
   }
   return false;
 }
 public:bool detect_land() {
   int x1 = (pos_x/100)*100;
   int y1 = s[pos_x/100];

   int x2 = ((pos_x/100)+1)*100;
   int y2 = s[pos_x/100+1];
   float m = ((float)y2-(float)y1) / ((float)x2-(float)x1);
   float b = y2-m*x2;
   if (x1==500&&(speed.x+speed.y<13)) {
     if ((pos_y)-(m*pos_x+b)>5) {
       return true;
     }
     if ((pos_y)-(m*(pos_x-5)+b)>5) {
       return true;
     }
   }
   return false;
 }
};


int main(int argc, char const *argv[]) {
  gout.open(1280, 720);
  srand(time(NULL));
  lander * eagle = new lander;
  surface moon;
  moon.generate();
  eagle->draw();
  gout << refresh;
  event ev;
  gin.timer(170);
  int exit = 0;
  bool exit_over = false;
  bool start_over = false;
  bool hints_over = false;
  while (gin >> ev) {
    gout << color(0,0,0);
    gout << move_to(0,0);
    gout << box_to(1279,719);
    gout << move_to(600,300);
    if (start_over) {
      gout << color(0,255,0);
    }
    else {
      gout << color(255,255,255);
    }
    gout << text("Start");
    gout << move_to(600,400);
    if (hints_over) {
      gout << color(0,255,0);
    }
    else {
      gout << color(255,255,255);
    }
    gout << text("Hints");
    gout << move_to(600,500);
    if (exit_over) {
      gout << color(0,255,0);
    }
    else {
      gout << color(255,255,255);
    }
    gout << text("Exit");
    gout << refresh;
    if (ev.type==ev_mouse) {
      if (ev.pos_y>490) {
        exit_over = true;
      }
      else {
        exit_over = false;
      }
    }
    if (ev.type==ev_mouse) {
      if (ev.pos_y>390&&ev.pos_y<400) {
        hints_over = true;
      }
      else {
        hints_over = false;
      }
    }
    if (ev.type==ev_mouse) {
      if (ev.pos_y>290&&ev.pos_y<300) {
        start_over = true;
      }
      else {
        start_over = false;
      }
    }
    if (ev.type==ev_mouse && ev.button==btn_left) {
      if (start_over) {
        break;
      }
      if (hints_over) {
        gout << move_to(0,0);
        gout << color(0,0,0);
        gout << box_to(1279,719);
        gout << color(255,255,255);
        gout << move_to(600,300);
        gout << text("In game controls: Up:W Left Thruster:A Right Thruster:D");
        gout << move_to(600,400);
        gout << text("Press W to go back to menu");
        gout <<refresh;
        while (gin >> ev) {
          if (ev.type==ev_key && ev.keycode==119) {
            break;
          }
        }
      }
      if (exit_over) {
        return 0;
      }
    }
  }
  while (gin >> ev&&exit==0) {
    if (ev.type==ev_key && ev.keycode==119) {
      phys_vector v;
      v.x = 0;
      v.y = -2;
      eagle->force_act(v);
    }
    if (ev.type==ev_key && ev.keycode==97) {
      phys_vector v;
      v.x = 1;
      v.y = 0;
      eagle->force_act(v);
    }
    if (ev.type==ev_key && ev.keycode==100) {
      phys_vector v;
      v.x = -1;
      v.y = 0;
      eagle->force_act(v);
    }
    if (ev.type==ev_timer) {
      phys_vector v;
      v.x = 0;
      v.y = 1;
      bool dontcheck = false;
      eagle->radar_scan(moon);
      if (eagle->detect_land()) {
        while (gin >>ev) {
          gout << move_to(100,100);
          gout << text("Good landing ! press 'W' to restart");
          gout << refresh;
          if (ev.type==ev_key&&ev.keycode==119) {
            std::cout << "1" << '\n';
            delete eagle;
            eagle = new lander;
            dontcheck = true;
            break;
          }
        }
      }
      if (!dontcheck&&eagle->detect_collision()) {
        while (gin >>ev) {
          gout << move_to(100,100);
          gout << text("Game Over, you crashed ! press 'W' to restart");
          gout << refresh;
          if (ev.type==ev_key&&ev.keycode==119) {
            std::cout << "2" << '\n';
            delete eagle;
            eagle = new lander;
            break;
          }
        }
      }
      else {
        std::cout << "3" << '\n';
        eagle->force_act(v);
        eagle->update_speed();
        eagle->draw();
        moon.draw();
        gout << refresh;
      }
    }
  }
  return 0;
}
