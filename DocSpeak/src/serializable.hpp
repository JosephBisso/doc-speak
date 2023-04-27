using namespace docspeak;

template<class T, class U>
requires BookItem<T, U>
std::shared_ptr<Book<T, U>> Book<T, U>::s_instance;

template<typename T>
size_t Item<T>::s_number_of_items = 0;