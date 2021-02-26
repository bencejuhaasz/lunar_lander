#include "graphics.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>

using namespace genv;
using namespace std;

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

struct lander {
  private: int pos_x=100;
  int pos_y=100;
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
};


int main(int argc, char const *argv[]) {
  gout.open(1280, 720);

  lander eagle;
  eagle.draw();
  gout << refresh;
  event ev;
  gin.timer(150);

  while (gin >> ev) {
    if (ev.type==ev_key && ev.keycode==119) {
      cout << "up\n";
      phys_vector v;
      v.x = 0;
      v.y = -2;
      eagle.force_act(v);
    }
    if (ev.type==ev_key && ev.keycode==97) {
      cout << "left\n";
      phys_vector v;
      v.x = 1;
      v.y = 0;
      eagle.force_act(v);
    }
    if (ev.type==ev_key && ev.keycode==100) {
      std::cout << "right" << '\n';
      phys_vector v;
      v.x = -1;
      v.y = 0;
      eagle.force_act(v);
    }
    if (ev.type==ev_timer) {
      eagle.update_speed();
      eagle.draw();
      gout << move_to(0,0);
      gout << refresh;
    }
    if (ev.type==ev_timer) {
      phys_vector v;
      v.x = 0;
      v.y = 1;
      eagle.force_act(v);
    }

  }
  return 0;
}
