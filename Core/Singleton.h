

#pragma once


template<class T>
class Singleton
{
public:
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

    static T& Get() 
    { 
        if(mInstance == nullptr)
        {
            mInstance = new T();
        }
        return *mInstance; 
    }
private:
    static T* mInstance;
};



template<class T>
T* Singleton<T>::mInstance = nullptr;