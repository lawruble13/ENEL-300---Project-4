#ifndef FSM
#define FSM

	template <class T> class State{
		public:
			T *resource;
			void (*enter)(T&);
			void (*loop)(T&);
			void (*exit)(T&);
			State<T>(){}
			State<T>(T *_resource, void(*_enter)(T&), void(*_loop)(T&), void(*_exit)(T&)){
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
		private:
			State<T> currentState;
		public:
			void setState(State<T> &nextState){
				(*(currentState.exit))(*(currentState.resource));
				currentState = nextState;
				(*(currentState.enter))(*(currentState.resource));
			}
			void doState(){
				(*(currentState.loop))(*(currentState.resource));
			}
			StateMachine<T>(){
				currentState = State<T>(NULL, [](T& t){}, [](T& t){}, [](T& t){});
			}
			StateMachine<T>(State<T> &firstState){
				currentState = firstState;
				(*(currentState.enter))(*(currentState.resource));
			}
			StateMachine<T>& operator=(const StateMachine<T>& other){
				if(this != &other){
					currentState = other.currentState;
				}
				return *this;
			}
	};

#endif
