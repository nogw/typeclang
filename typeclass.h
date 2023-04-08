#ifndef IT_TYPECLASS_H
#define IT_TYPECLASS_H

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
