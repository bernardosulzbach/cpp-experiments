# C++ experiments

This repository hosts a small set of experiments related to C++ behavior.

The result of several of the experiments is implementation-sensitive _by design_, as they are meant to allow one to better understand how their C++ implementation behaves.

## `std::shared_ptr` overhead

Unlike `std::unique_ptr`, which can have zero memory overhead, `std::shared_ptr` also needs a control block in the heap.
This is a short investigation of this overhead when using GCC 10.2.1 on x86 64-bit.

```
Creating a shared pointer to an 8-byte integer with std::make_shared.
  Made an allocation of size 24.
Creating a shared pointer to an 8-byte integer after a manual allocation.
  Made an allocation of size 8.
  Made an allocation of size 24.
Creating a shared pointer to a 128-byte array with std::make_shared.
  Made an allocation of size 144.
Creating a shared pointer to a 128-byte array after a manual allocation.
  Made an allocation of size 128.
  Made an allocation of size 24.
```

From these results we can see that GCC 10.2.1 requires 24 bytes for the control block of a `std::shared_ptr`.
This struck me as strange as two 8-byte integers should only need a 16-byte struct for them.
It is made even stranger by the fact that when allocating the object and the control block together (through `std::make_shared`), GCC only needs 16 extra bytes for the control block.

Using GDB and `ptype /o <TYPE>` is a way of get the layout of types in memory.
We start by investigating the implementation of `shared_ptr`.

```
(gdb) ptype /o std::__shared_ptr<int, (__gnu_cxx::_Lock_policy)2>
/* offset    |  size */  type = class std::__shared_ptr<int, (__gnu_cxx::_Lock_policy)2> [with _Tp = int] : public std::__shared_ptr_access<_Tp, (__gnu_cxx::_Lock_policy)2, false, false> {
                         private:
/*    0      |     8 */    _Tp *_M_ptr;
/*    8      |     8 */    class std::__shared_count<(__gnu_cxx::_Lock_policy)2> {
                             private:
/*    8      |     8 */        std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2> *_M_pi;

                               /* total size (bytes):    8 */
                           } _M_refcount;

                           /* total size (bytes):   16 */
                         }
```

It has a pointer to the object it holds and a pointer to the control block, as one would expect.
Let's see the control block now.

```
(gdb) ptype /o std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2>
/* offset    |  size */  type = class std::_Sp_counted_base<(__gnu_cxx::_Lock_policy)2> : public std::_Mutex_base<(__gnu_cxx::_Lock_policy)2> {
                         private:
/*    8      |     4 */    _Atomic_word _M_use_count;
/*   12      |     4 */    _Atomic_word _M_weak_count;

                           /* total size (bytes):   16 */
                         }
```

It inherits from `std::mutex`, so we have to look at that as well.

```
(gdb) ptype /o std::_Mutex_base<(__gnu_cxx::_Lock_policy)2>
/* offset    |  size */  type = class std::_Mutex_base<(__gnu_cxx::_Lock_policy)2> {
    <no data fields>

                           /* total size (bytes):    1 */
                         }
```

Because `std::mutex` requires its own address and has an alignment of 8, the first 8 bytes of `std::_Sp_counted_base` are reserved for it, with the next 4 bytes reserved for the use count, and the last 4 bytes reserved for the weak count.

This explains why the control block takes at least 16 bytes.
It doesn't explain, however, why the implementation makes a 24-byte allocation for it when the `shared_ptr` is created through the constructor.

