# Random.h

```c++
class UniformIntRandom;
```

> It is a interface class which is used to generate random numbers.
  You should define these functions by yourself.
>
> ---
>
> ```c++
> virtual const int32_t rand(const int32_t& from, const int32_t& to) = 0;
> ```
>
> > It should return a number which is not less than `from` and no greater than
    `to`.
>
> ---
>
> ```c++
> virtual void set_seed(const int32_t& seed) = 0;
> ```
>
> > Set the `seed` of random number, which is very important.