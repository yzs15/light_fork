#include <torch/torch.h>
#include <iostream>

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    torch::Tensor tensor1 = torch::eye(3);
    auto mid = std::chrono::high_resolution_clock::now();
    torch::Tensor tensor2 = torch::eye(3);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = mid-start;
    std::cout << "Time to initialize OpenCV and load image: " << diff.count() << " s\n";
    diff = end - mid;
    std::cout << "Time to initialize OpenCV and load image: " << diff.count() << " s\n";
//   std::cout << tensor << std::endl;
}