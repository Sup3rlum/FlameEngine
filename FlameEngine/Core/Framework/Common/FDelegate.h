#pragma once


template<typename TFuncType>
class FDelegateBase;

template<typename TReturn, typename ...TArgs>
class FDelegateBase<TReturn(TArgs...)>
{

protected:

	using stub_type = TReturn(*)(void* this_ptr, TArgs...);

	struct InvocationElement
	{
		InvocationElement() = default;
		InvocationElement(void* this_ptr, stub_type aStub) : object(this_ptr), stub(aStub) {}

		void Clone(InvocationElement& target) const
		{
			target.stub = stub;
			target.object = object;
		}
		bool operator ==(const InvocationElement& another) const
		{
			return another.stub == stub && another.object == object;
		}
		bool operator !=(const InvocationElement& another) const
		{
			return another.stub != stub || another.object != object;
		}

		void* object = nullptr;
		stub_type stub = nullptr;
	};

};




template <typename TFuncType> class FDelegate;

template<typename TReturn, typename ...TArgs>
class FDelegate<TReturn(TArgs...)> final : private FDelegateBase<TReturn(TArgs...)>
{
public:

	template<typename TUserClass>
	struct FMemFuncSignature
	{
		typedef TReturn(TUserClass::* FuncPtr)(TArgs...);
	};

	template<typename TUserClass>
	struct FMemFuncSignatureConst
	{
		typedef TReturn(TUserClass::* FuncPtr)(TArgs...) const;
	};


	struct FStaticFuncSignature
	{
		typedef TReturn(*FuncPtr)(TArgs...);
	};



	FDelegate() = default;

	bool IsNull() const { return invocation.stub == nullptr; }
	bool operator ==(void* ptr) const
	{
		return (ptr == nullptr) && this->isNull();
	}
	bool operator !=(void* ptr) const
	{
		return (ptr != nullptr) || (!this->isNull());
	}

	FDelegate(const FDelegate& another) { another.invocation.Clone(invocation); }

	template <typename TLambda>
	FDelegate(const TLambda& lambda)
	{
		assign((void*)(&lambda), lambda_stub<TLambda>);
	}

	FDelegate& operator =(const FDelegate& another)
	{
		another.invocation.Clone(invocation);
		return *this;
	}

	template <typename TLambda>
	FDelegate& operator =(const TLambda& instance)
	{
		assign((void*)(&instance), lambda_stub<TLambda>);
		return *this;
	}

	bool operator == (const FDelegate& another) const { return invocation == another.invocation; }
	bool operator != (const FDelegate& another) const { return invocation != another.invocation; }

	template <class TClass, typename FMemFuncSignature<TClass>::FuncPtr TMethod>
	static FDelegate Make(TClass* instance)
	{
		return FDelegate(instance, method_stub<TClass, TMethod>);
	}

	template <class TClass, typename FMemFuncSignatureConst<TClass>::FuncPtr TMethod>
	static FDelegate Make(TClass const* instance)
	{
		return FDelegate(const_cast<TClass*>(instance), const_method_stub<TClass, TMethod>);
	}

	template <typename FStaticFuncSignature::FuncPtr TMethod>
	static FDelegate Make()
	{
		return FDelegate(nullptr, function_stub<TMethod>);
	}


	template <typename TLambda>
	static FDelegate Make(const TLambda& instance)
	{
		return FDelegate((void*)(&instance), lambda_stub<TLambda>);
	}




	TReturn operator()(TArgs... arg) const
	{
		return (*invocation.stub)(invocation.object, arg...);
	}

private:

	FDelegate(void* anObject, typename FDelegateBase<TReturn(TArgs...)>::stub_type aStub)
	{
		invocation.object = anObject;
		invocation.stub = aStub;
	}

	void assign(void* anObject, typename FDelegateBase<TReturn(TArgs...)>::stub_type aStub)
	{
		this->invocation.object = anObject;
		this->invocation.stub = aStub;
	}

	template <class TClass, typename FMemFuncSignature<TClass>::FuncPtr TMethod>
	static TReturn method_stub(void* this_ptr, TArgs... params)
	{
		TClass* p = static_cast<TClass*>(this_ptr);
		return (p->*TMethod)(params...);
	}

	template <class TClass, typename FMemFuncSignatureConst<TClass>::FuncPtr TMethod>
	static TReturn const_method_stub(void* this_ptr, TArgs... params)
	{
		TClass* const p = static_cast<TClass*>(this_ptr);
		return (p->*TMethod)(params...);
	}

	template <typename FStaticFuncSignature::FuncPtr TMethod>
	static TReturn function_stub(void* this_ptr, TArgs... params)
	{
		return (TMethod)(params...);
	}

	template <typename TLambda>
	static TReturn lambda_stub(void* this_ptr, TArgs... arg)
	{
		TLambda* p = static_cast<TLambda*>(this_ptr);
		return (p->operator())(arg...);
	}

	typename FDelegateBase<TReturn(TArgs...)>::InvocationElement invocation;

};
