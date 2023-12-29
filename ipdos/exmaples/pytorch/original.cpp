#include <torch/torch.h>
#include <iostream>

int main() {
    // 创建一个线性层，输入特征数为10，输出特征数为5
    torch::nn::Linear linear(10, 5);
    
    // 创建一个随机的10维向量
    auto input = torch::randn({1, 10});
    
    // 使用线性层进行变换
    auto output = linear->forward(input);

    std::cout << output << std::endl;

    return 0;
}
