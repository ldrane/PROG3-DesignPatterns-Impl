#include <iostream>
#include <memory>
#include <cmath>
#include <string>

class Sin;
class Cos;

// === CLASE BASE ABSTRACTA ===
class Expression {
public:
    virtual ~Expression() = default;
    virtual double evaluate(double x) = 0;
    virtual std::shared_ptr<Expression> derivative() = 0;
    virtual std::string toString() = 0;
};

// === HOJAS ===
class Constant : public Expression {
private:
    double value;

public:
    Constant(double val) : value(val) {}

    double evaluate(double x) override {
        return value;
    }

    std::shared_ptr<Expression> derivative() override {
        return std::make_shared<Constant>(0);
    }

    std::string toString() override {
        //Con la siguiente implementación, busco que el programa distinga si el número es entero (para mostrarlo sin decimales)
        // o decimal (para mostrar el valor completo), esto con el objetivo de ser más preciso y fiel al valor real, y evitar
        // pérdida de información,

        if (value == static_cast<int>(value))
            return std::to_string(static_cast<int>(value));
        else
            return std::to_string(value);
    }
};

class Variable : public Expression {
public:
    double evaluate(double x) override {
        return x;
    }

    std::shared_ptr<Expression> derivative() override {
        return std::make_shared<Constant>(1);
    }

    std::string toString() override {
        return "x";
    }
};

// === OPERACIONES BINARIAS Y UNARIAS ===


// Clase base para operaciones binarias
class BinaryOperation : public Expression {
protected:
    std::shared_ptr<Expression> left, right;
public:
    BinaryOperation(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r): left(l), right(r) {}
};

// Clase base para funciones unarias
class UnaryFunction : public Expression {
protected:
    std::shared_ptr<Expression> arg;
public:
    UnaryFunction(std::shared_ptr<Expression> a) : arg(a) {}
};

//Funciones binarias

class Add : public BinaryOperation {
public:
    Add(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r): BinaryOperation(l, r){}

    double evaluate(double x) override {
        return left->evaluate(x) + right->evaluate(x);
    }

    std::shared_ptr<Expression> derivative() override {
        // (f+g)' = f' + g'
        return std::make_shared<Add>(left->derivative(), right->derivative());
    }

    std::string toString() override {
        return "(" + left->toString() + " + " + right->toString() + ")";
    }
};

class Multiply : public BinaryOperation {
public:
    Multiply(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r): BinaryOperation(l, r) {}

    double evaluate(double x) override {
        return left->evaluate(x) * right->evaluate(x);
    }

    std::shared_ptr<Expression> derivative() override {
        // (f*g)' = f'*g + f*g'
        return std::make_shared<Add>(
                std::make_shared<Multiply>(left->derivative(), right),
                std::make_shared<Multiply>(left, right->derivative())
        );
    }

    std::string toString() override {
        return "(" + left->toString() + " * " + right->toString() + ")";
    }
};

//Potencia (La construcción la realizé asumiendo que el exponente es constante)
class Power : public BinaryOperation {
public:
    Power(std::shared_ptr<Expression> b, std::shared_ptr<Expression> e): BinaryOperation(b, e){}

    double evaluate(double x) override {
        return std::pow(left->evaluate(x), right->evaluate(x));
    }

    std::shared_ptr<Expression> derivative() override {
        // (f^n)' = n * f^(n-1) * f' ---> sólo valido si n es constante
        return std::make_shared<Multiply>(
                std::make_shared<Multiply>(
                        right,
                        std::make_shared<Power>(
                                left,
                                std::make_shared<Add>(right, std::make_shared<Constant>(-1))
                        )
                ),
                left->derivative()
        );
    }

    std::string toString() override {
        return "(" + left->toString() + "^" + right->toString() + ")";
    }
};


//Funciones unarias

class Sin : public UnaryFunction {
public:
    Sin(std::shared_ptr<Expression> a) : UnaryFunction(a) {}

    double evaluate(double x) override {
        return std::sin(arg->evaluate(x));
    }

    std::shared_ptr<Expression> derivative() override; //Su definición está más abajo. No lo definí acá para evitar problemas al no haberse definido clase cos aún.

    std::string toString() override {
        return "sin(" + arg->toString() + ")";
    }
};

class Cos : public UnaryFunction {
public:
    Cos(std::shared_ptr<Expression> a) : UnaryFunction(a) {}

    double evaluate(double x) override {
        return std::cos(arg->evaluate(x));
    }

    std::shared_ptr<Expression> derivative() override;

    std::string toString() override {
        return "cos(" + arg->toString() + ")";
    }
};

//Derivada de seno
std::shared_ptr<Expression> Sin::derivative(){ //Su definición está más abajo. No lo definí acá para evitar problemas al no haberse definido clase cos aún.
    // (sin f)' = cos(f) * f'
    return std::make_shared<Multiply>(
            std::make_shared<Cos>(arg),
            arg->derivative()
    );
}

std::shared_ptr<Expression> Cos::derivative() {
    // (cos f)' = -sin(f) * f'
    return std::make_shared<Multiply>(
            std::make_shared<Multiply>(
            std::make_shared<Constant>(-1),
            std::make_shared<Sin>(arg)
            ),
            arg->derivative()
    );
}



int main() {
    auto x = std::make_shared<Variable>();

    // f(x) = (x² + 3*x) * sin(x)
    auto expr = std::make_shared<Multiply>(
            std::make_shared<Add>(
                    std::make_shared<Power>(x, std::make_shared<Constant>(2)),
                    std::make_shared<Multiply>(std::make_shared<Constant>(3), x)
            ),
            std::make_shared<Sin>(x)
    );

    auto deriv = expr->derivative();

    std::cout << "f(x) = " << expr->toString() << std::endl;
    std::cout << "f'(x) = " << deriv->toString() << std::endl;
    std::cout << "f(2) = " << expr->evaluate(2) << std::endl;

    return 0;
}