#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

class webserver {
 private:
  int n{0};

 public:
  webserver();
  webserver(int t);
  ~webserver();
  void print();
};

#endif