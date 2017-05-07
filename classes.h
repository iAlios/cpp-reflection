#ifndef REFLECTION_CLASSES_H
#define REFLECTION_CLASSES_H

#include <string>
#include <memory>
#include <map>
#include <stdarg.h>

#include "wrapper.h"

namespace reflection {

#define CLASS_NAME(CLASS) #CLASS

#define CLASS_CREATE_FUNC(CLASS) &CLASS::create

    template<typename T, typename K = std::string, typename ...Args>
    class ClassFactory {
    public:
        ~ClassFactory() {}

        template<typename N>
        struct Register {
            Register(const K &key) {
                ClassFactory::getInstance().mObjectConstructor[key] = std::make_shared<Wrapper<std::shared_ptr<T>, Args ...>>(
                        CLASS_CREATE_FUNC(N));
            }
        };

        template<typename N>
        struct RegisterConstructor {

            RegisterConstructor(const K &key) {
                auto func = [](Args... args) -> std::shared_ptr<T> {
                    return std::make_shared<N>(std::forward<Args>(args)...);
                };
                ClassFactory::getInstance().mObjectConstructor[key] =
                        std::make_shared<Wrapper<std::shared_ptr<T>, Args ...>>(func);
            }
        };

        bool registerClass(K &name, std::shared_ptr<Wrapper<std::shared_ptr<T>, Args...>> func) {
            if (func) {
                if (mObjectConstructor.count(name) <= 0) {
                    mObjectConstructor.insert(make_pair(name, func));
                }
            }
            return true;
        };

        auto findClass(const K &key) -> std::shared_ptr<Wrapper<std::shared_ptr<T>, Args...>> {
            if (mObjectConstructor.count(key) > 0) {
                return mObjectConstructor[key];
            }
            return NULL;
        }

        static ClassFactory &getInstance() {
            static ClassFactory mClassFactory;
            return mClassFactory;
        }

    private:
        ClassFactory() {};

        ClassFactory(const ClassFactory &) = delete;

        ClassFactory(ClassFactory &&) = delete;

        ClassFactory &operator=(const ClassFactory &) = delete;

        std::map<K, std::shared_ptr<Wrapper<std::shared_ptr<T>, Args...>>> mObjectConstructor;
    };

    /**
     * 首字母大写，空白分隔单词
     * @param a 输入参数
     */
    inline void changeUpper(char a[]) {
        int i, n;
        n = strlen(a);
        for (i = 0; i < n; i++) {
            if ((a[0] >= 'a') && (a[0] <= 'z')) {
                a[0] = a[0] - 32;
                i++;
            }
            if ((a[i] == ' ') || (a[i] == '\t') || (a[i] == '\r') || (a[i] == '\n')) {
                if ((a[i + 1] >= 'a') && (a[i + 1] <= 'z')) {
                    a[i + 1] = a[i + 1] - 32;
                }
            }
        }
    }

#define FACTORY_NAME(BASE) \
    typedef ClassFactory<BASE> BASE##Factory

#define FACTORY_NAME_WITH_MULTI_ARGS(BASE, ...) \
    typedef ClassFactory<BASE, std::string, ##__VA_ARGS__> BASE##Factory##__VA_ARGS__

#define REGISTER_METHOD_WITH_BASE(BASE, CLASS) \
    FACTORY_NAME(BASE); \
    ClassFactory<BASE>::Register<CLASS> __##CLASS(CLASS_NAME(CLASS))

#define REGISTER_METHOD_WITH_BASE_AND_MULTI_ARGS(BASE, CLASS, ...) \
    FACTORY_NAME_WITH_MULTI_ARGS(BASE, ##__VA_ARGS__); \
    ClassFactory<BASE, std::string, ##__VA_ARGS__>::Register<CLASS> __##CLASS(CLASS_NAME(CLASS))

#define REGISTER_CONSTRUCTOR_WITH_BASE(BASE, CLASS) \
    FACTORY_NAME(BASE); \
    ClassFactory<BASE>::RegisterConstructor<CLASS> ____##CLASS(CLASS_NAME(CLASS))

#define REGISTER_CONSTRUCTOR_WITH_BASE_AND_MULTI_ARGS(BASE, CLASS, ...) \
    FACTORY_NAME_WITH_MULTI_ARGS(BASE, ##__VA_ARGS__); \
    ClassFactory<BASE, std::string, ##__VA_ARGS__>::RegisterConstructor<CLASS> ____##CLASS(CLASS_NAME(CLASS))

}
#endif //REFLECTION_CLASSES_H
