//class Shape {
//public:
//    virtual double calculateArea() = 0;
//    virtual double calculatePerimeter() = 0;
//    virtual ~Shape() {}
//};




class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}
    double calculateArea() override {
        return radius * radius * M_PI;
    }
    double calculatePerimeter() override {
        return 2 * M_PI * radius;
    }
};

class Rectangle : public Shape {
private:
    double length;
    double width;

public:
    Rectangle(double l, double w) : length(l), width(w) {}
    double calculateArea() override {
        return length * width;
    }
    double calculatePerimeter() override {
        return 2 * length + 2 * width;
    }
};

class Triangle : public Shape {
private:
    double side1;
    double side2;
    double side3;

public:
    Triangle(double s1, double s2, double s3) : side1(s1), side2(s2), side3(s3) {}
    double calculateArea() override {
        double p = 0.5 * (side1 + side2 + side3);
        return sqrt(p * (p - side1) * (p - side2) * (p - side3));
    }
    double calculatePerimeter() override {
        return side1 + side2 + side3;
    }
};

void printShapeInfo(Shape* shape) {
    if (dynamic_cast<Circle*>(shape)){
        std::cout << "Shape Type: Circle" << std::endl;
    }
    else if (dynamic_cast<Rectangle*>(shape)){
        std::cout << "Shape Type: Rectangle" << std::endl;
    }
    else if (dynamic_cast<Triangle*>(shape)){
        std::cout << "Shape Type: Triangle" << std::endl;
    }
    std::cout << std::fixed << std::setprecision(2);
	std::cout << "Area: " << shape -> calculateArea() << std::endl;
    std::cout << "Perimeter: " << shape -> calculatePerimeter() << std::endl;
}

int main() {
	std::string line;
    while (std::getline(std::cin, line)) {

        Shape* shape = nullptr;

        std::istringstream iss(line);
        std::string ShapeName;
        iss >> ShapeName;
        if (ShapeName == "circle"){
            double radius;
            iss >> radius;
            shape = new Circle(radius);
            printShapeInfo(shape);
            delete shape;
        }
        else if (ShapeName == "rectangle"){
            double length, width;
            iss >> length >> width;
            shape = new Rectangle(length, width);
            printShapeInfo(shape);
            delete shape;
        }
        else if (ShapeName == "triangle"){
            double side1, side2, side3;
            iss >> side1 >> side2 >> side3;
            shape = new Triangle(side1, side2, side3);
            printShapeInfo(shape);
            delete shape;
        }
    }
    return 0;
}
