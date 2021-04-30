#ifndef APP_H
#define APP_H

typedef struct App {
  const char *name;
  void *(*setup)();
  void (*loop)(void *);
  void (*exit)(void *);
};

#endif
