# typeclang

it's good friday, so why not implement some typeclasses in c? preprocessor is really fun

```c
impl_show(Point, point_show);
impl_ord(Point, point_compare);
impl_eq(Point, point_equals);

impl_show(Person, person_show);
impl_ord(Person, person_compare);
impl_eq(Person, person_equals);

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
```
