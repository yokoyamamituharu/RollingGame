#pragma once
// �R���p�C���ɂ��^�`�F�b�N�A���O��Ԃ̎w����\
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