
#ifndef REFLECTION_WRAPPER_H
#define REFLECTION_WRAPPER_H

#include <functional>
#include <memory>

namespace reflection {

    template<typename Result, typename ...Args>
    class IMethod {
    public:
        virtual ~IMethod() {};

        Result operator()(Args... args) {
            return invoke(args...);
        }

    private:
        virtual Result invoke(Args... args) = 0;
    };

    template<typename Result, typename ...Args>
    class SimpleMethod : public IMethod<Result, Args...> {
    public:
        SimpleMethod(Result (*func)(Args ...)) : method(func) {}

    private:
        virtual Result invoke(Args... args) override {
            return method(args ...);
        }

        Result (*method)(Args ...);
    };

    template<typename Object, typename Result, typename ...Args>
    class MemberMethod : public IMethod<Result, Args...> {
    public:
        MemberMethod(Object *obj, Result (Object::*func)(Args ...)) :
                obj(obj), method(func) {}

    private:
        virtual Result invoke(Args... args) override {
            return ((*obj).*method)(args ...);
        }

        std::shared_ptr<Object> obj;

        Result (Object::*method)(Args ...);
    };

    template<typename Result, typename ...Args>
    struct Wrapper {
    public:
        Wrapper() {}

        template<typename Object>
        Wrapper(Object *obj, Result (Object::*func)(Args...)) {
            iMethod = std::shared_ptr<IMethod<Result, Args...>>(
                    new MemberMethod<Object, Result, Args...>(obj, func));
        }

        Wrapper(Result (*func)(Args...)) {
            iMethod = std::shared_ptr<IMethod<Result, Args...>>(
                    new SimpleMethod<Result, Args...>(func));
        }

        ~Wrapper() {
            iMethod.reset();
        }

        Result operator()(Args... args) {
            return (*iMethod)(args...);
        }

        Wrapper &operator=(Result (*func)(Args...)) {
            iMethod = std::shared_ptr<IMethod<Result, Args...>>(
                    new SimpleMethod<Result, Args...>(func));
            return *this;
        }

    private:
        std::shared_ptr<IMethod<Result, Args...>> iMethod;
    };

    template<typename Object, typename Result, typename ...Args>
    auto
    wrapper(Object &x, Result(Object::*fun)(Args...)) -> std::shared_ptr<Wrapper<Result, Args...>> {
        return std::make_shared<Wrapper<Result, Args ...>>(&x, fun);
    }

    template<typename Result, typename ...Args>
    auto wrapper(Result (*fun)(Args...)) -> std::shared_ptr<Wrapper<Result, Args...>> {
        return std::make_shared<Wrapper<Result, Args ...>>(fun);
    }

}
#endif //REFLECTION_WRAPPER_H
