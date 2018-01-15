#pragma once
 
 template <class T>
 class cSingleton
 {
 public:
  cSingleton(T& obj)
  {
   if(instance)
	   throw;
   instance = &obj;
  };
  ~cSingleton()
  {
   if(!instance)
	   throw;
   instance = 0;
  };
  static T& getInstance()
  {
   if(!instance)
	   throw;
   return *instance;
  };
 private:
  static T* instance;
  cSingleton(const cSingleton& src);
  cSingleton& operator=(const cSingleton& src);
 };

 template <class T>  T* cSingleton<T>::instance = NULL;



