/* (c) Magnus Auvinen. See licence.txt in the root of the distribution for more information. */
/* If you are missing that file, acquire a complete release at teeworlds.com.                */
#ifndef ENGINE_KERNEL_H
#define ENGINE_KERNEL_H

#include <base/system.h>

class IKernel;
class IInterface;

class IInterface
{
	// friend with the kernel implementation
	friend class CKernel;
	IKernel *m_pKernel;
protected:
	IKernel *Kernel() { return m_pKernel; }
public:
	IInterface() : m_pKernel(0) {}
	virtual ~IInterface() {}

	//virtual unsigned InterfaceID() = 0;
	//virtual const char *InterfaceName() = 0;
};

#define MACRO_INTERFACE(Name, ver) \
	public: \
		static const char *InterfaceName() { return Name; } \
	private:

		//virtual unsigned InterfaceID() { return INTERFACE_ID; }
		//virtual const char *InterfaceName() { return name; }


// This kernel thingie makes the structure very flat and basiclly singletons.
// I'm not sure if this is a good idea but it works for now.
class IKernel
{
	// hide the implementation
	virtual bool RegisterInterfaceImpl(const char *InterfaceName, IInterface *pInterface) = 0;
	virtual bool RegisterInterfaceImplNum(const char* InterfaceName, IInterface* pInterface, int Num) = 0;
	virtual bool ReregisterInterfaceImpl(const char *InterfaceName, IInterface *pInterface) = 0;
	virtual IInterface *RequestInterfaceImpl(const char *InterfaceName) = 0;
	virtual IInterface* RequestInterfaceImplNum(const char* InterfaceName, int Num) = 0;
public:
	static IKernel *Create();
	virtual ~IKernel() {}

	// templated access to handle pointer convertions and interface names
	template<class TINTERFACE>
	bool RegisterInterface(TINTERFACE *pInterface)
	{
		return RegisterInterfaceImpl(TINTERFACE::InterfaceName(), pInterface);
	}
	template<class TINTERFACE>
	bool ReregisterInterface(TINTERFACE *pInterface)
	{
		return ReregisterInterfaceImpl(TINTERFACE::InterfaceName(), pInterface);
	}
	template<class TINTERFACE>
	bool RegisterInterfaceNum(TINTERFACE* pInterface, int Num)
	{
		return RegisterInterfaceImplNum(TINTERFACE::InterfaceName(), pInterface, Num);
	}

	// Usage example:
	//		IMyInterface *pMyHandle = Kernel()->RequestInterface<IMyInterface>()
	template<class TINTERFACE>
	TINTERFACE *RequestInterface()
	{
		return reinterpret_cast<TINTERFACE *>(RequestInterfaceImpl(TINTERFACE::InterfaceName()));
	}

	// Usage example:
	//		IMyInterface *pMyHandle = Kernel()->RequestInterface<IMyInterface>(0)
	template<class TINTERFACE>
	TINTERFACE* RequestInterfaceNum(int Num)
	{
		return reinterpret_cast<TINTERFACE*>(RequestInterfaceImplNum(TINTERFACE::InterfaceName(), Num));
	}
};

#endif
