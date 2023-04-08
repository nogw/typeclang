#ifndef TYPECLASS_H
#define TYPECLASS_H

#define typeclass(funcs)                                                       \
  struct {                                                                     \
    funcs;                                                                     \
  }

#define typeclass_instance(Typeclass)                                          \
  struct {                                                                     \
    void *self;                                                                \
    Typeclass const *tc;                                                       \
  }

#endif
