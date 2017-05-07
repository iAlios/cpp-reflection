# cpp-reflection
simple reflection in cpp


    class Object {
    public:
        Object() {}

        virtual void print() = 0;

        virtual ~Object() {}
    };

    #define REGISTER_CLASS_CONSTRUCTOR(CLASS) \
        REGISTER_CONSTRUCTOR_WITH_BASE(Object, CLASS)

    class A : public Object {
    public :
        A() { cout << hex << (long) this << " A constructor!" << endl; }

        ~A() { cout << hex << (long) this << " A destructor!" << endl; }

        virtual void print() override {
            LOGI("this is a, ====hello world...====");
        }

    };

    REGISTER_CLASS_CONSTRUCTOR(A);

    class B : public Object {
    public :
        B() { cout << hex << (long) this << " B constructor!" << endl; }

        ~B() { cout << hex << (long) this << " B destructor!" << endl; }

        virtual void print() override {
            LOGI("this is b, ====hello world...====");
        }

    };

    REGISTER_CLASS_CONSTRUCTOR(B);

then you can use it like this:

    shared_ptr<Wrapper<shared_ptr<Object>>> constructor = ObjectFactory::getInstance().findClass(
            "A");
    shared_ptr<Object> a = (*constructor)();
    a->print();
    shared_ptr<Wrapper<shared_ptr<Object>>> constructor = ObjectFactory::getInstance().findClass(
            "B");
    shared_ptr<Object> b = (*constructor)();
    b->print();
