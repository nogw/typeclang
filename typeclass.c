#include "typeclass.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Eq

typedef typeclass(bool (*const equals)(void *self, void *other)) EqClass;
typedef typeclass_instance(EqClass) Eq;

#define eq(impl) eq_##impl
#define neq(impl) neq_##impl
#define impl_eq(T, equals_f)                                                   \
  static inline bool equals_f##__(void *self, void *other) {                   \
    bool (*const equals_)(T * self, T * other) = (equals_f);                   \
    (void)equals_;                                                             \
    return equals_f(self, other);                                              \
  }                                                                            \
  static EqClass const eq_class_##equals_f = {.equals = (equals_f##__)};       \
  static inline bool eq_##T(T *a, T *b) {                                      \
    Eq const ea = (Eq){.tc = &eq_class_##equals_f, .self = a};                 \
    Eq const eb = (Eq){.tc = &eq_class_##equals_f, .self = b};                 \
    return ea.tc->equals(ea.self, eb.self);                                    \
  }                                                                            \
  static inline bool neq_##T(T *a, T *b) { return !eq_##T(a, b); }

// Ord

typedef typeclass(int (*const compare)(void *self, void *other)) OrdClass;
typedef typeclass_instance(OrdClass) Ord;

#define lt(impl) lt_##impl
#define gt(impl) gt_##impl
#define lte(impl) lte_##impl
#define gte(impl) gte_##impl

#define impl_ord(T, compare_f)                                                 \
  static inline int compare_f##__(void *self, void *other) {                   \
    int (*const compare_)(T * self, T * other) = (compare_f);                  \
    (void)compare_;                                                            \
    return compare_f(self, other);                                             \
  }                                                                            \
  static OrdClass const ord_class_##compare_f = {.compare = (compare_f##__)};  \
  static inline Ord ord_init##T(void *self, OrdClass const *tc) {              \
    return (Ord){.self = self, .tc = tc};                                      \
  }                                                                            \
  static inline bool lt_##T(T *a, T *b) {                                      \
    return ord_init##T(a, &ord_class_##compare_f).tc->compare(a, b) < 0;       \
  }                                                                            \
  static inline bool gt_##T(T *a, T *b) {                                      \
    return ord_init##T(a, &ord_class_##compare_f).tc->compare(a, b) > 0;       \
  }                                                                            \
  static inline bool lte_##T(T *a, T *b) {                                     \
    return ord_init##T(a, &ord_class_##compare_f).tc->compare(a, b) <= 0;      \
  }                                                                            \
  static inline bool gte_##T(T *a, T *b) {                                     \
    return ord_init##T(a, &ord_class_##compare_f).tc->compare(a, b) >= 0;      \
  }

// Show

typedef typeclass(char *(*const impl)(void *self)) ShowClass;
typedef typeclass_instance(ShowClass) Show;

#define show(impl) show_##impl
#define impl_show(T, show_f)                                                   \
  static inline char *show_f##T(void *self) {                                  \
    char *(*const show_)(T * self) = (show_f);                                 \
    (void)show_;                                                               \
    return show_f(self);                                                       \
  }                                                                            \
  static inline Show show_##T(T *x) {                                          \
    static ShowClass const show_class_##T = {.impl = (show_f##T)};             \
    return (Show){.tc = &show_class_##T, .self = x};                           \
  }

void print(Show showable) {
  char *const s = showable.tc->impl(showable.self);
  puts(s);
  free(s);
}

// Yuuup

typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  const char *name;
  int age;
} Person;

static char *point_show(Point *self) {
  char *buf = malloc(64);

  const size_t buf_size = 64;
  const char *fmt = "Point(%d, %d)";

  int res = snprintf(buf, buf_size, fmt, self->x, self->y);
  return (buf && res >= 0 && (size_t)res < buf_size) ? buf : NULL;
}

static bool point_equals(Point *self, Point *other) {
  return self->x == other->x && self->y == other->y;
}

int point_compare(Point *p1, Point *p2) {
  if (p1->x < p2->x)
    return -1;
  else if (p1->x > p2->x)
    return 1;
  else if (p1->y < p2->y)
    return -1;
  else if (p1->y > p2->y)
    return 1;
  else
    return 0;
}

static char *person_show(Person *self) {
  char *buf = malloc(64);

  const size_t buf_size = 64;
  const char *fmt = "User(%s, %d)";

  int res = snprintf(buf, buf_size, fmt, self->name, self->age);
  return (buf && res >= 0 && (size_t)res < buf_size) ? buf : NULL;
}

static bool person_equals(Person *self, Person *other) {
  return self->name == other->name && self->age == other->age;
}

int person_compare(Person *self, Person *other) {
  int name_cmp = strcmp(self->name, other->name);
  return (name_cmp != 0) ? name_cmp : self->age - other->age;
}

impl_ord(Point, point_compare);
impl_show(Point, point_show);
impl_eq(Point, point_equals);

impl_ord(Person, person_compare);
impl_eq(Person, person_equals);
impl_show(Person, person_show);

int main(void) {
  Person person0 = {"nogw", 18};
  Person person1 = {"nogw", 18};

  Point point0 = {16, 0};
  Point point1 = {17, 0};

  print(show(Person)(&person0));
  print(show(Point)(&point0));

  printf("%i\n", eq(Person)(&person0, &person1));
  printf("%i\n", neq(Point)(&point1, &point0));

  printf("%i\n", gte(Person)(&person0, &person1));
  printf("%i\n", lt(Point)(&point0, &point1));

  return 0;
}
