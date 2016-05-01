#ifndef MAKEUNIQUEADDER_H_
#define MAKEUNIQUEADDER_H_

#include <iostream>
#include <memory>

#ifdef __cpp_lib_make_unique
//    std::cout << "we have make_unique" << std::endl;
#else
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
#endif

#endif /* MAKEUNIQUEADDER_H_ */
