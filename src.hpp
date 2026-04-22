#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>

struct Permutation { // 置换
    int* mapping; // 我觉得这里存啥你应该知道
    size_t size; // 上面那个数组的长度

    Permutation(size_t size) {
        // 初始化一个长度为size的置换，内容为恒等置换。
        this->size = size;
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) mapping[i] = static_cast<int>(i);
    }

    Permutation(const int* mapping, size_t size) {
        // 初始化一个置换，内容为mapping，大小为size
        this->size = size;
        this->mapping = new int[size];
        for (size_t i = 0; i < size; ++i) this->mapping[i] = mapping[i];
    }

    ~Permutation() {
        // 析构函数，合理管理你的内存😈
        delete[] mapping;
        mapping = nullptr;
        size = 0;
    }

    void apply(int* permutation) const {
        // 对于大小为size的数组permutation，将置换应用到它上，直接修改permutation指向的数组。
        if (!permutation || size == 0) return;
        int* tmp = new int[size];
        for (size_t i = 0; i < size; ++i) tmp[i] = permutation[static_cast<size_t>(mapping[i])];
        for (size_t i = 0; i < size; ++i) permutation[i] = tmp[i];
        delete[] tmp;
    }

    Permutation operator*(const Permutation& other) const {
        // 返回两个置换的乘积，即this*other，其中 (this*other)(i) = this(other(i))。
        Permutation result(size);
        for (size_t i = 0; i < size; ++i) {
            result.mapping[i] = mapping[static_cast<size_t>(other.mapping[i])];
        }
        return result;
    }

    Permutation inverse() const {
        // 返回该置换的逆置换
        Permutation inv(size);
        for (size_t i = 0; i < size; ++i) inv.mapping[static_cast<size_t>(mapping[i])] = static_cast<int>(i);
        return inv;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        // 这里重载了输出流，使得输出更方便。这里不需要修改代码。
        os << "[";
        for (size_t i = 0; i < p.size; ++i) {
            os << p.mapping[i];
            if (i < p.size - 1) {
                os << " ";
            }
        }
        os << "]";
        return os;
    }
};

struct Transposition { // 对换
    int a, b;

    Transposition() {
        // 初始化对换为恒等对换
        a = 0;
        b = 0;
    }

    Transposition(int a, int b)  {
        // 初始化对换为{a,b}
        this->a = a;
        this->b = b;
    }

    void apply(int* permutation, size_t size) const {
        // 将对换应用permutation上，直接修改permutation指向的数组。保证size>a且size>b。
        if (!permutation || a == b) return;
        if (static_cast<size_t>(a) >= size || static_cast<size_t>(b) >= size) return;
        int t = permutation[a];
        permutation[a] = permutation[b];
        permutation[b] = t;
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        // 这里重载了输出流，使得输出更方便。这里不需要修改代码。
        os << "{" << t.a << " " << t.b << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        // 将对换转换为大小为size的置换。保证size>a且size>b。返回一个置换。
        Permutation p(size);
        if (a != b && static_cast<size_t>(a) < size && static_cast<size_t>(b) < size) {
            p.mapping[a] = b;
            p.mapping[b] = a;
        }
        return p;
    }
};

struct Cycle { // 轮换
    int* elements;// 我也觉得这里存啥你应该知道
    size_t size;

    Cycle(const int* elements, size_t size) {
        // 初始化一个轮换，内容为elements，大小为size
        this->size = size;
        this->elements = new int[size];
        for (size_t i = 0; i < size; ++i) this->elements[i] = elements[i];
    }

    ~Cycle() {
        // 析构函数，合理管理你的内存😈
        delete[] elements;
        elements = nullptr;
        size = 0;
    }

    void apply(int* permutation, size_t size) const {
        // 将轮换应用大小为size的permutation上，直接修改permutation指向的数组。保证size>=elements[i]。
        if (!permutation || this->size == 0) return;
        for (size_t i = 0; i < this->size; ++i) if (static_cast<size_t>(elements[i]) >= size) return;
        if (this->size == 1) return;
        int tmp = permutation[elements[0]];
        for (size_t i = 0; i + 1 < this->size; ++i) {
            permutation[elements[i]] = permutation[elements[i + 1]];
        }
        permutation[elements[this->size - 1]] = tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cycle& c) {
        // 这里重载了输出流，使得输出更方便。这里不需要修改代码。
        os << "{";
        for (size_t i = 0; i < c.size; ++i) {
            os << c.elements[i];
            if (i < c.size - 1) {
                os << " ";
            }
        }
        os << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        // 将轮换转换为大小为size的置换。保证size>=elements[i]。返回一个置换。
        Permutation p(size);
        if (this->size == 0) return p;
        for (size_t i = 0; i < this->size; ++i) if (static_cast<size_t>(elements[i]) >= size) return p;
        for (size_t i = 0; i < this->size; ++i) {
            int from = elements[i];
            int to = elements[(i + 1) % this->size];
            p.mapping[from] = to;
        }
        return p;
    }
};


#endif
