#ifndef BASE_H
#define BASE_H


class Resource {
};

void emptyState(Resource &resource){
  return;
}

template <class T> class State{
public:
  T resource;
  void (*enter)(T &resource);
  void (*loop)(T &resource);
  void (*exit)(T &resource);
  State(){
    enter = loop = exit = &emptyState;
  }
  State(T &_resource, void(*_enter)(T &resource), void(*_loop)(T &resource), void(*_exit)(T &resource)){
    resource = _resource;
    enter = _enter;
    loop = _loop;
    exit = _exit;
  }
  State<T>& operator=(const State<T>& other){
    if(this != &other){
      resource = other.resource;
      enter = other.enter;
      loop = other.loop;
      exit = other.exit;
    }
    return *this;
  }
};

template <class T> class StateMachine{
public:
  void setState(State<T> &nextState){
    (*currentState.exit)(currentState.resource);
    currentState = nextState;
    (*currentState.enter)(currentState.resource);
  }
  void doState(){
    (*currentState.loop)(currentState.resource);
  }
private:

  State<T> currentState;
};

#endif
