//
// Created by Balalarga on 07.03.2022.
//

#include <map>
#include <functional>


class Operations
{
public:
    Operations() = delete;

    using Unary = std::function<double(double)>;
    using Binary = std::function<double(double, double)>;

    static Unary UnaryFromString(const std::string& name);
    static const std::map<std::string, Unary>& GetUnaries();
    
    static Binary BinaryFromString(const std::string& name);
    static const std::map<std::string, Binary>& GetBinaries();

    static double RvCross(double a, double b);
    static double RvUnion(double a, double b);


private:
    static std::map<std::string, Unary> _unaryOperations;
    static std::map<std::string, Binary> _binaryOperations;
};
