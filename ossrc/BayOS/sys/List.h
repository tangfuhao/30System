#ifndef __LIST_H__
#define __LIST_H__

/* �ϒ��z��N���X */
template <class T> class List {
public:
	List();
	List(int size);
	List(int size, int increase);
	~List();

public:
	void add(T element);
	T    get(int index) const;
	T    operator[](int index);
	T    removeAt(int index);
	T    remove(T element);
	void removeAll();
	int  size() const;
	bool isEmpty() const;
	int  indexOf(T element) const;

private:
	T*  data_;        /* �����z��     */
	int size_;        /* �z��̃T�C�Y */
	int elements_;    /* �v�f��       */
	int increase_;    /* ������       */

	/* ������ */
	void init(int size, int increase);
};

/* �R���X�g���N�^ */
template <class T> List<T>::List()
{
	init(5, 5);
	return;
}

/* �R�s�[�R���X�g���N�^ */
template <class T> List<T>::List(int size)
{
	init(size, 5);
	return;
}

/* �R�s�[�R���X�g���N�^ */
template <class T> List<T>::List(int size, int increase)
{
	init(size, increase);
	return;
}

/* �f�X�N�g���N�^ */
template <class T> List<T>::~List()
{
	/* �������̉�� */
	delete[] data_;
	return;
}

/* �󂩂ǂ�����Ԃ� */
template <class T> bool List<T>::isEmpty() const
{
	return elements_ == 0;
}

/* �v�f��ǉ����� */
template <class T> void List<T>::add(T element)
{
	/* �z�񂪈�t�ɂȂ��� */
	if (size_ == elements_) {

		/* �z���2�{�ɂ��� */
		size_ += increase_;
		T* temp = new T[size_];

		/* �œK���H */
		int numElements = elements_;

		/* �I���W�i���̔z���V�����z��ɃR�s�[ */
		for (int i = 0; i < numElements; i++) {
			temp[i] = data_[i];
		}

		delete[] data_;
		data_ = temp;
	}

	/* �v�f�̒ǉ� */
	data_[elements_] = element;
	elements_++;
	return;
}

/* i�Ԗڂ̗v�f�𓾂� */
template <class T> T List<T>::get(int index) const
{
	/* �͈̓`�F�b�N */
	if (index < 0 || index >=elements_) {
		return (T)NULL;
	}
	return data_[index];
}

/* �z��̂悤�ɃA�N�Z�X���� */
template <class T> T List<T>::operator[](int index)
{
	return (this->get(index));
}

/* �T�C�Y�𓾂� */
template <class T> int List<T>::size() const
{
	return elements_;
}

/* i�Ԗڂ̗v�f���폜���� */
template <class T> T List<T>::removeAt(int index)
{
	/* �͈̓`�F�b�N */
	if (index < 0 || index >=elements_) {
		return (T)NULL;
	}

	/* �폜����v�f���R�s�[ */
	T toRemove = data_[index];

	/* �󂫂𖄂߂� */
	int numElements = elements_;
	for (int i = index; i < numElements - 1; i++) {
		data_[i] = data_[i + 1];
	}
	elements_--;
	return toRemove;
}

template <class T> T List<T>::remove(T element)
{
	/* �œK���H */
	int size = this->size();

	for (int i = 0; i < size; i++) {
		/* �폜����v�f���������� */
		if (data_[i] == element) {
			return (removeAt(i));
		}
	}

	return (T)NULL;
}

template <class T> void List<T>::removeAll()
{
	/* �������̉�� */
	delete[] data_;
	init(5, 5);
}

template <class T> void List<T>::init(int size, int increase)
{
	/* �v�f�� */
	elements_ = 0;

	/* �T�C�Y�Ƒ�������ݒ� */
	size_     = size     > 0 ? size : 5;
	increase_ = increase > 0 ? increase : 5;

	/* �����z������ */
	data_ = new T[size_];
	return;
}

template <class T> int List<T>::indexOf(T element) const
{
	/* �œK���H */
	int size = this->size();

	/* �v�f��T�� */
	for (int i = 0; i < size; i++) {
		/* �v�f���������� */
		if (data_[i] == element) {
			return i;
		}
	}

	/* ������Ȃ����� */
	return -1;
}

#endif
