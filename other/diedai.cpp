#include <iostream>
#include <ceres/ceres.h>
#include <cmath>
#include <vector>

// 常数定义
const double g = 9.8;  // 重力加速度，单位 m/s^2
const double C_d = 0.47;  // 阻力系数（假设为球形物体）
const double m = 1.0;  // 物体质量，单位 kg
const double rho = 1.225;  // 空气密度，单位 kg/m^3
const double A = 0.01;  // 物体横截面积，单位 m^2

// 计算空气阻力
double dragForce(double velocity) {
    return 0.5 * C_d * rho * A * velocity * velocity;
}

// 定义优化目标函数
struct BallisticCostFunction {
    BallisticCostFunction(double x_t, double y_t, double x_s, double y_s, double v0)
        : x_t_(x_t), y_t_(y_t), x_s_(x_s), y_s_(y_s), v0_(v0) {}

    template <typename T>
    bool operator()(const T* const theta, T* residual) const {
        // 角度theta (弧度)
        T vx = v0_ * ceres::cos(theta[0]);  // 水平方向速度
        T vy = v0_ * ceres::sin(theta[0]);  // 垂直方向速度

        // 使用数值积分法模拟物体轨迹
        T x = T(x_s_);
        T y = T(y_s_);
        T t = T(0);  // 时间
        T dt = T(0.01);  // 时间步长

        // 数值积分直到物体到达地面或最大时间
        while (y > T(0) && t < T(100)) {
            // 计算当前速度的大小
            T velocity = ceres::sqrt(vx * vx + vy * vy);
            T drag = dragForce(velocity);  // 空气阻力

            // 计算加速度
            T ax = -drag * vx / (m * velocity);  // 水平方向加速度
            T ay = -g - drag * vy / (m * velocity);  // 垂直方向加速度

            // 更新速度
            vx += ax * dt;
            vy += ay * dt;

            // 更新位置
            x += vx * dt;
            y += vy * dt;

            // 更新时间
            t += dt;
        }

        // 计算物体与目标位置的误差
        residual[0] = ceres::sqrt((T(x_t_) - x) * (T(x_t_) - x) + (T(y_t_) - y) * (T(y_t_) - y));
        return true;
    }

private:
    double x_t_, y_t_, x_s_, y_s_, v0_;
};

int main() {
    // 输入参数
    double v0 = 50.0;  // 初始速度，单位 m/s
    double x_s = 0.0;  // 自己的初始位置，单位 m
    double y_s = 0.0;
    double x_t = 100.0;  // 目标位置，单位 m
    double y_t = 0.0;

    // 设置 Ceres 求解器问题
    ceres::Problem problem;

    // 初始发射角度猜测 (单位: 弧度)
    double initial_theta = 45.0 * M_PI / 180.0;  // 初始猜测为 45 度

    // 创建成本函数并添加到问题中
    problem.AddResidualBlock(
        new ceres::AutoDiffCostFunction<BallisticCostFunction, 1, 1>(
            new BallisticCostFunction(x_t, y_t, x_s, y_s, v0)),
        nullptr,  // 不使用权重
        &initial_theta);  // 要优化的参数

    // 设置求解器选项
    ceres::Solver::Options options;
    options.minimizer_type = ceres::LINE_SEARCH;
    options.max_num_iterations = 100;

    // 求解问题
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    // 输出结果
    std::cout << "最优发射角度: " << initial_theta * 180.0 / M_PI << " 度" << std::endl;
    std::cout << "优化摘要：" << std::endl;
    std::cout << summary.FullReport() << std::endl;

    return 0;
}
