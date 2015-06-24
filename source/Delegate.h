#ifndef _DELEGATE_H_
#define _DELEGATE_H_

#include <deque>
#include <map>
#include "Utility.h"

class BaseDelegate
{
	//------- method --------
public:
	BaseDelegate() {}
	virtual ~BaseDelegate() {}
	virtual void operator()() = 0;
};

template<class T>
class Delegate :public BaseDelegate
{
public:
	typedef void(T::*EventFunc)();

	//--------- field --------
private:
	T*					obj;
	EventFunc			func;

	//--------- method ----------
public:
	Delegate()
	{
		obj = nullptr;
		func = nullptr;
	}
	~Delegate() {}
	void operator()()
	{
		if (obj)
		{
			(obj->*func)();
		}
	}
	static BaseDelegate* Create(T* obj, EventFunc func)
	{
		Delegate* inst = new Delegate();
		inst->Set(obj, func);
		return inst;
	}

	void Set(T* obj, EventFunc func)
	{
		this->obj = obj;
		this->func = func;
	}
};

// �f���Q�[�g�Ǘ��N���X
class DelegateMgr
{
	//---------- field -------------
private:
	std::deque<BaseDelegate*> delegateQueue;
	std::map<int, BaseDelegate*> delegateList;

	//------------ method -----------
public:
	~DelegateMgr()
	{
		delegateQueue.clear();
		for (auto it = delegateList.begin(); it != delegateList.end();)
		{
			SafeDelete(it->second);
			it = delegateList.erase(it);
		}
	}
	// �L���[����������o�^
	DelegateMgr& operator=(int idx)
	{
		delegateQueue.clear();
		delegateQueue.push_back(delegateList[idx]);
		return *this;
	}
	// �ǉ��o�^
	DelegateMgr& operator+=(int idx)
	{
		delegateQueue.push_back(delegateList[idx]);
		return *this;
	}
	// �L���[���ꊇ���s�A�L���[����ɂ���
	void operator()()
	{
		while (!delegateQueue.empty())
		{
			(*delegateQueue.front())();
			delegateQueue.pop_front();
		}
	}
	// �L���[��ێ����ꊇ���s
	void RetentionRun()
	{
		auto it = delegateQueue.begin();
		while (it!=delegateQueue.end())
		{
			(**it++)();
		}
	}
	// �L���[�����
	void Clear()
	{
		delegateQueue.clear();
	}

	template<class T>
	int Cleate(int idx, T* obj, void(T::*func)())
	{
		delegateList[idx] = Delegate<T>::Create(obj, func);
		return idx;
	}


};

#endif //#ifndef _DELEGATE_H_