#ifndef __AgMLStructure_h__
#define __AgMLStructure_h__
#ifndef __CINT__

#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <typeinfo>
#include <assert.h>
#include <functional>

/*!

  Code to manage the creation, filling and selection of AgML structures.

  Highly templated.

  Relies on dictionaries generated by the AgML compiler.

 */


///
/// Base class for runtime assignment to structure members
///
class AgMLDetpBase {
public:
  virtual void Write() = 0;
  virtual int  Index(){ return -1; }
  virtual void Print() = 0;
};
///
/// Base class for AgML structures.  Allows assignment of one or more
/// structure members at run time by user.
///
class AgMLStructureBase  
{
public:
  virtual void fill() = 0;
  virtual void Register( AgMLDetpBase *detp ){ 
    _detp.push_back(detp); 
  }
private:
protected:
  std::vector< AgMLDetpBase * > _detp;
};

///
/// 
///
/// Predicates for searching a list of structs to find a struct with a given member.
/// Usage:
/// 
/// struct A {
///    int a;
///    float b;
///    string c;
/// };
///
/// A myA;
/// isEqual<A,float> __b__ ( &myA, &myA.b )
/// 
///
/// vector<A *> vec;
/// auto x = find_if( vec.begin(), vec.end(), __b__ = 3.0 );
/// if ( x ) myA = *x;
///
/// @tparam S The structure to be searched
/// @tparam T The type of the member variable to be queried
/// @param  str Pointer an instance of the structure
/// @param  mem Pointer to the member w/in the instance of the structure to be searched
///
template<typename S, typename T>
struct isEqual {
  isEqual( S* str, T* mem ) : _offset((long long)(mem) - (long long)(str)),_value(0) { }
  bool operator()( const S* str ){ 
    return _value == *(T*)((long long)(str)+_offset); 
  }
  isEqual &operator=( const T xvalue ) {
    _value = xvalue;
    return *(this);
  };
  T value(){ return _value; }
protected:
  long long _offset;
  T         _value;
};

///
/// Partial specialization for strings as member variables.
///
template<typename S>
struct isEqual<S, std::string> {
  isEqual( S* str, std::string* mem ) : _offset((long long)(mem) - (long long)(str)),_value("") { }
  bool operator()( const S* str ){ 
    return _value == *(std::string*)((long long)(str)+_offset); 
  }

  // Assignment operator used to set value and return functor to allow
  //   find_if( v.begin(), v.end(), __isequal__ = value ) 
  isEqual &operator=( const std::string value ) {
    _value = value;
    return *(this);
  };
protected:
  long long _offset;
  std::string    _value;
};




///
/// Another prefill template class.  This time using the info stubs generated by
/// AgML
///
///@tparam S Dictionary describing the memory layout of the structure generated by AgML
///@tparam M Dictionary describing the member variable 
///@tparam T Type for the value which is assigned
///
template< typename S, // struct dictionary
	  typename M, // member dictionary
	  typename T  // typename for the value
	  >
class AgMLDetp : public AgMLDetpBase {
public:

  static AgMLDetp *New()
  {
    AgMLDetp *base = new AgMLDetp();
    S::structure -> Register(base);
    base -> struct_addr =  0;
    base -> member_addr =  0;
    base -> index       = -1;
    return base;
  };

  void Add( const T value, const int idx=-1 )
  {
    // store struct base address
    struct_addr = S::address;
    member_addr = M::address();
    // And copy data value to local storage
    memcpy( &member_value, &value, sizeof( value ) );
    // Set index for filling
    index = idx;
    std::cout << "ADD -- ";  Print();
  };

  AgMLDetp &operator=( const T &value )
  {
    member_value = value;
    index = -1;
  };

  int Index(){ return index; }

  void Write()
  {
    //std::cout << "AgMLDetp write: " << S::name() << "." << M::name() << " = " << member_value << std::endl;
    // write value to target address
    //    std::cout << "Write " << S::name() <<"."<<M::name()<<" (@"<<M::address()<<") = " << member_value << std::endl;
    memcpy( M::address(), &member_value, sizeof(member_value) );
  }

  void Print()
  {
    std::cout << "DETP: " << S::name() << ":" << M::name() << " index=" << index << std::endl;
  }

private:
protected:

public:
  T        member_value; // Value to write to target address

private:
protected:
  S        struct_info; // Description of the structure
  M        member_info; // Description of the data memeber

  void *struct_addr; // Address of the target structure
  void *member_addr; // address of the member variable

  int index;         // index to store the value for multiple fills

  AgMLDetp(){ };
  
};



///
/// The AgMLStructure class provides the interface between the single
/// instance of the structure which is accessible to the developer, and
/// the vector of structures which stores different values for the 
/// structure specified in the Fill blocks.
///
/// The fill method takes the current values stored in the structure and
/// pushes them into a vector, after checking to see if there is a run-
/// time assignment from an external module.  When a runtime assignment
/// is found, it overwrites the value assigned in the geometry file.
///
/// The use method takes a predicate which is aware of the memory layout
/// of the struct managed by AgMLStructure, and the value which is being
/// searched for.  It is used to search the vector for the first version 
/// of the structure whose requested member is equal to the value assigned
/// to the predicate.
///
/// @tparam T type of the struct
/// @tparam I dictionary for the struct
///
template<
  typename T, // Type of the structure
  typename I  // Structure information
  >
class AgMLStructure : public AgMLStructureBase
{
public:

  AgMLStructure( const char *_name ) : AgMLStructureBase(), name(_name) { };

  void fill() {
    table.push_back( new T( current ) );
    int index = table.size() - 1; // current FILL index
    int n = _detp.size();
    for ( int i=0;i<n;i++ )
      {
	AgMLDetpBase *detp = _detp[i];
	//detp->Print();
	// Only write if detp is the right index (including default)
	if ( detp->Index() < 0 || index == detp->Index() ) // always resolve
	  {
	    // This writes to current
	    detp->Write();
	    // Update the in memory table
	    (*table.back()) = current;
	  }


      }

  };


  template<class UnaryPredicate>
  bool use( UnaryPredicate p ){ 
    //    std::cout << "Use " << name << " " << typeid(p).name() << " == " << p.value() << " ";
    auto x = std::find_if( table.begin(), table.end(), p );
    if ( x == table.end() ) {
      //      std::cout << "[WARNING]: USE "<< name << " " << typeid(p).name() << " == " << p.value() << " failed." << std::endl << std::flush;  assert(0);
      return false;
    }
    if ( *x ) {
      current = *(*x);
      //      std::cout << "[success]"<< std::endl;
      return true;
    };
    //    std::cout << "[WARNING]: USE "<< name << " " << typeid(p).name() << " == " << p.value() << " failed." << std::endl << std::flush;  assert(0);
    return false; 
  };


  // //
  // // Could simplify selextion by using c++11 lambdas...
  // //
  // //   HEXG.use( []( hexg_t h ){ return h.type == 1; } )
  // //
  // template<class Struct>
  // bool use2( std::function<bool(Struct)> predicate ) {
  //   auto x = std::find_if( table.begin(), table.end(), predicate );
  //   if ( x == table.end() ) return false;

  //   if ( *x ) { 
  //     current = *(*x);
  //     return true;
  //   };

  //   return false;
  // };






  /// Cast to a refence to the current version
  operator T&(){ return current; }

  /// Print the structure
  void print()
  {
    
  }

private:
protected:
  std::vector<T *> table;   // table of elements
  T           current; // current table element
  I           info;    // information about the structure
  std::string name;
};
#endif
#endif
