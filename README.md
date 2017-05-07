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
    
    #define REGISTER_CLASS_CONSTRUCTOR_MULTI_ARGS(CLASS, ...) \
        REGISTER_CONSTRUCTOR_WITH_BASE_AND_MULTI_ARGS(Object, CLASS, ##__VA_ARGS__)

    struct Foo : public Object {

        Foo(int a) {
            LOGI("the bar's result is %d", a);
        }

        int bar(int x, float y, bool z) {
            LOGI("the bar's result is %d, %f, %d", x, y, z);
            return 0;
        }

        virtual void print() override {
            LOGI("Foo: this is a, ====hello world...====");
        }

        static int bar1(int x, float y, bool z) {
            LOGI("the bar1's result is %d, %f, %d", x, y, z);
            return 0;
        }

    };

    REGISTER_CLASS_CONSTRUCTOR_MULTI_ARGS(Foo, int);


then you can use it like this:

    shared_ptr<Wrapper<shared_ptr<Object>>> constructor = ObjectFactory::getInstance().findClass(
            "A");
    shared_ptr<Object> a = (*constructor)();
    a->print();
    constructor = ObjectFactory::getInstance().findClass("B");
    shared_ptr<Object> b = (*constructor)();
    b->print();    
    shared_ptr<Wrapper<shared_ptr<Object>, int>> constructor1 = ObjectFactoryint::getInstance().findClass(
            "Foo");
    shared_ptr<Object> foo = (*constructor1)(1);
    foo->print();
