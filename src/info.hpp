#ifndef INFO_HPP
#define INFO_HPP

namespace Info {
    const std::string name    = "Ecce";
    const std::string version = "rev. 380";
    const std::string author  = "D. Gruzdev, M. Melnikov";

    inline void show() {
        std::cout << "id name " << Info::name << " " << Info::version << "\n";
        std::cout << "id author " << Info::author << "\n";
    }
}

#endif /* INFO_HPP */
