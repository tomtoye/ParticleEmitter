#pragma once

#ifndef __MANAGER_H__
#define __MANAGER_H__
//This is a basic manager class that behaves like a singleton

template <typename T>
class Manager
{
protected:
	Manager();
	virtual ~Manager();

private:
	//a private pointer to an instance of this class
	static T* m_pInstance;
public:
	static T* CreateInstance();
	static T* GetInstance();
	static void DestroyInstance();

};

template <typename T> T* Manager<T>::CreateInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new T();
	}
	return m_pInstance;
}

template <typename T> T* Manager<T>::GetInstance()
{
	return m_pInstance;
}

template <typename T> void Manager<T>::DestroyInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}

template <typename T> T* Manager<T>::m_pInstance = nullptr;

template <typename T> Manager<T>::Manager(){}
template <typename T> Manager<T>::~Manager() {}

#endif //__MANAGER_H__
