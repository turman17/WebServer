#ifndef CIRCULARBUFFER_HPP
# define CIRCULARBUFFER_HPP

# include <exception>

template<typename T, std::size_t N>
class CircularBuffer {
public:
	CircularBuffer() : m_head(0), m_tail(0), m_size(0) {}

	~CircularBuffer() {}

	bool	push_back(const T& newItem) {
		
		if (isFull()) {
			return false;
		}
		m_container[m_tail] = newItem;
		m_tail = (m_tail + 1) % N;
		m_size++;
		return true;
	}

	bool	pop_front() {

		if (isEmpty()) {
			return false;
		}
		m_head = (m_head + 1) % N;
		m_size--;
		return true;
	}

	T		front() {
		if (m_size < 1) {
			throw IndexOutOfBounds();
		}
		return (m_container[0]);
	}

	bool	isEmpty() {
		return (m_size == 0);
	}

	bool	isFull() {
		return (m_size == N);
	}

	std::size_t size() {
		return (m_size);
	}

	T& operator[](int index) {
		
		index = index + m_head;
		if (index >= m_size) {
			throw IndexOutOfBounds();
		}
    	return m_container[index];
	}

	class IndexOutOfBounds : public std::exception {
	public:
		const char* what() const throw() {
			return ("IndexOutOfBounds");
		}
	};

private:
	T			m_container[N];
	int			m_head;
	int			m_tail;
	std::size_t	m_size;
};

#endif
