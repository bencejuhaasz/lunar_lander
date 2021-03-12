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
    s.push_back(719);
  }
  void draw() {
    int j = 0;
    gout << color(255,255,255);
    gout << move_to(0,719);
    for (size_t i = 0; i < 1280; i+=100) {
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
    gout << move_to(pos_x, pos_y);
    gout << line_to(pos_x+20, pos_y);
    gout << line_to(pos_x+20, pos_y+20);
    gout << line_to(pos_x, pos_y+20);
    gout << line_to(pos_x, pos_y);
  }
 public:void radar_scan(surface moon) {
   s = moon.bounce_back_radar_waves();
 }
 public:bool detect_collision() {
   std::cout << s[pos_x/100] << " " << s[(pos_x/100)+1] << '\n';
   int x1 = (pos_x/100)*100;
   int y1 = s[pos_x/100];

   int x2 = ((pos_x/100)+1)*100;
   int y2 = s[pos_x/100+1];
   float m = (y2-y1) / (x2-x1);
   float b = y2-m*x2;
   std::cout << pos_y << " " << (m*pos_x+b) << '\n';
   if ((pos_y+20)>(m*pos_x+b)) {
     return true;
   }
   return false;
 }
};


int main(int argc, char const *argv[]) {
  gout.open(1280, 720);

  lander * eagle = new lander;
  surface moon;
  moon.generate();
  eagle->draw();
  gout << refresh;
  event ev;
  gin.timer(150);
  int exit = 0;
  while (gin >> ev&&exit==0) {
    if (ev.type==ev_key && ev.keycode==119) {
      cout << "up\n";
      phys_vector v;
      v.x = 0;
      v.y = -2;
      eagle->force_act(v);
    }
    if (ev.type==ev_key && ev.keycode==97) {
      cout << "left\n";
      phys_vector v;
      v.x = 1;
      v.y = 0;
      eagle->force_act(v);
    }
    if (ev.type==ev_key && ev.keycode==100) {
      std::cout << "right" << '\n';
      phys_vector v;
      v.x = -1;
      v.y = 0;
      eagle->force_act(v);
    }
    if (ev.type==ev_timer) {
      phys_vector v;
      v.x = 0;
      v.y = 1;
      eagle->force_act(v);
      eagle->update_speed();
      eagle->draw();
      moon.draw();
      eagle->radar_scan(moon);
      if (eagle->detect_collision()) {
        while (gin >>ev) {
          gout << move_to(600,300);
          gout << text("Game Over, press 'up' to restart");
          gout << refresh;
          if (ev.type==ev_key&&ev.keycode==119) {
            delete eagle;
            eagle = new lander;
            break;
          }
        }
      }
    }
  gout << refresh;
  }
  return 0;
}
