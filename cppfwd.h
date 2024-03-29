//---------------------------------------------------------------------------
#ifndef cppfwdH
#define cppfwdH
//---------------------------------------------------------------------------
#include <iosfwd>
//---------------------------------------------------------------------------
// STL用前方参照
namespace std
{
    template <class T> struct less;
    template <class T1, class T2> struct pair;
    template <class T> class allocator;
    template <class T, class Allocator> class vector;
    template <class T, class Allocator> class list;
    template <class T, class Allocator> class deque;
    template <class T, class Container> class stack;
    template <class Key, class Compare, class Allocator> class set;
    template <class Key, class T, class Compare, class Allocator> class map;
};
//---------------------------------------------------------------------------
namespace ChimeraSystem
{
    template <class T> class TChimeraAutoPtr;
};
//---------------------------------------------------------------------------
#define __startfunction
#define __endfunction
//---------------------------------------------------------------------------
#endif
