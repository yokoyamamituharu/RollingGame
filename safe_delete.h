#pragma once
// コンパイラによる型チェック、名前空間の指定も可能
template <class T>
inline void safe_delete(T*& p) {
	delete p;
	p = nullptr;
}

template <class T>
inline void safe_delete_array(T p[]) {
	delete[] p;
	p = nullptr;
}