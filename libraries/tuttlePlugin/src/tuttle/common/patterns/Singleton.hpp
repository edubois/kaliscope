#ifndef Singleton_HPP
#define Singleton_HPP

/**
 * @brief Singleton<ClassSingleton> Can transform a class into Singleton (by inheriting this class)
 *
 * @par Purpose
 * Forcing (limit) the presence of a single instance of a class
 *
 * @par Usage
 * The class T, to use as Singleton, must derive from this class (: public Singleton <T>).
 * The macro MAKE_SINGLETON (T) create necessary elements (including constructors).
 */
template <class T>
class Singleton
{
private:
	static T* inst;

	Singleton( const Singleton& ) {}
	Singleton& operator=( const Singleton& ) {}

protected:
	Singleton() {}
	virtual ~Singleton() = 0;

public:
	/**
	 * @brief return the unique instance of Singleton<T> class
	 * @return T the unique instance of Singleton<T> class
	 */
	static T& instance()
	{
		if( !inst )
			inst = new T;
		return *inst;
	}

	/**
	 * @brief destroy the unique instance of Singleton<T> class
	 */
	static void destroy()
	{
		delete inst;
		inst = NULL;
	}

};

template <class T>
T * Singleton<T>::inst = NULL;

template <class T>
Singleton<T>::~Singleton() {}

///macro to implement singleton. Use it in derived class declaration
#define MAKE_SINGLETON( Class ) \
	public: \
		friend class Singleton < Class >; \
	private: \
		Class() {} \
		~Class() {}

///macro to implement singleton. Use it in derived class declaration
#define MAKE_SINGLETON_WITHCONSTRUCTORS( Class ) \
	public: \
		friend class Singleton < Class >; \
	private: \
		Class(); \
		~Class();

#endif
