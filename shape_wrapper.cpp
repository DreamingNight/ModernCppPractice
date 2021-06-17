class shape {};
class circle : public shape {};
class triangle : public shape {};

class shape_wrapper {
public:
  explicit shape_wrapper(shape *ptr)
}